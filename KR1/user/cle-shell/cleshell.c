/* vi: set sw=4 ts=4: */
/*
 * Termios command line History and Editting.
 *
 * Copyright (c) 1986-2003 may safely be consumed by a BSD or GPL license.
 * Written by:   Vladimir Oleynik <dzo@simtreas.ru>
 *
 * Used ideas:
 *      Adam Rogoyski    <rogoyski@cs.utexas.edu>
 *      Dave Cinege      <dcinege@psychosis.com>
 *      Jakub Jelinek (c) 1995
 *      Erik Andersen    <andersen@codepoet.org> (Majorly adjusted for busybox)
 *
 * This code is 'as is' with no warranty.
 *
 *
 */

/*
   Usage and Known bugs:
   Terminal key codes are not extensive, and more will probably
   need to be added. This version was created on Debian GNU/Linux 2.x.
   Delete, Backspace, Home, End, and the arrow keys were tested
   to work in an Xterm and console. Ctrl-A also works as Home.
   Ctrl-E also works as End.

   Small bugs (simple effect):
   - not true viewing if terminal size (x*y symbols) less
     size (prompt + editor`s line + 2 symbols)
   - not true viewing if length prompt less terminal width
 */


#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <signal.h>
#include <limits.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/* Maximum length of the linked list for the command line history */
#define MAX_HISTORY    15
#define BUFSIZ		  128
#define setTermSettings(fd,argp) tcsetattr(fd,TCSANOW,argp)
#define getTermSettings(fd,argp) tcgetattr(fd, argp);
#define Isprint(c) ( (c) >= ' ' && (c) != ((unsigned char)'\233') )

typedef __sighandler_t sighandler_t;


static char *history[MAX_HISTORY+1]; /* history + current */
static int n_history = 0;				  /* saved history lines */
static int cur_history;				  /* current pointer to history line */

/* Current termio and the previous termio before starting sh */
static struct termios initial_settings, new_settings;

static volatile int cmdedit_termw = 80;        /* actual terminal width */
static volatile int handlers_sets = 0; 		  /* Set next bites: */

enum {
	SET_ATEXIT = 1,         /* when atexit() has been called
				   and get euid,uid,gid to fast compare */
	SET_WCHG_HANDLERS = 2,  /* winchg signal handler */
	SET_RESET_TERM = 4,     /* if the terminal needs to be reset upon exit */
};

enum {
	ESC = 27,
	DEL = 127,
};


static int cmdedit_x;           /* real x terminal position */
static int cmdedit_y;           /* pseudoreal y terminal position */
static int cmdedit_prmt_len;    /* lenght prompt without colores string */

static int cursor;              /* required global for signal handler */
static int len;                 /* --- "" - - "" - -"- --""-- --""--- */
static char *command_ps;        /* --- "" - - "" - -"- --""-- --""--- */
static char prompt[64];


static void cmdedit_setwidth(int w, int redraw_flg);

static void win_changed(int nsig)
{
	struct winsize win = { 0, 0, 0, 0 };
	static sighandler_t previous_SIGWINCH_handler;  /* for reset */

	/*   emulate      || signal call */
	if (nsig == -SIGWINCH || nsig == SIGWINCH) {
		ioctl(0, TIOCGWINSZ, &win);
		if (win.ws_col > 0) {
			cmdedit_setwidth(win.ws_col, nsig == SIGWINCH);
		}
	}
	/* Unix not all standart in recall signal */

	if (nsig == -SIGWINCH)          /* save previous handler   */
		previous_SIGWINCH_handler = signal(SIGWINCH, win_changed);
	else if (nsig == SIGWINCH)      /* signaled called handler */
		signal(SIGWINCH, win_changed);  /* set for next call       */
	else                                            /* nsig == 0 */
		/* set previous handler    */
		signal(SIGWINCH, previous_SIGWINCH_handler);    /* reset    */
}


static void cmdedit_reset_term(void)
{
	if ((handlers_sets & SET_RESET_TERM) != 0) {
	/* sparc and other have broken termios support: use old termio handling. */
		setTermSettings(fileno(stdin), (void *) &initial_settings);
		handlers_sets &= ~SET_RESET_TERM;
	}
	if ((handlers_sets & SET_WCHG_HANDLERS) != 0) {
		/* reset SIGWINCH handler to previous (default) */
		win_changed(0);
		handlers_sets &= ~SET_WCHG_HANDLERS;
	}
	fflush(stdout);
}


/* special for recount position for scroll and remove terminal margin effect */
static void cmdedit_set_out_char(int next_char)
{

	int c = (int)((unsigned char) command_ps[cursor]);

	if (c == 0)
		c = ' ';        /* destroy end char? */
	putchar(c);
	if (++cmdedit_x >= cmdedit_termw) {		
		/* terminal is scrolled down */
		cmdedit_y++;
		cmdedit_x = 0;

		if (!next_char)
			next_char = '\n';
		/* destroy "(auto)margin" */
		putchar(next_char);
		putchar('\b');
	}
	cursor++;
}

/* Move to end line. Bonus: rewrite line from cursor */
static void input_end(void)
{
	while (cursor < len)
		cmdedit_set_out_char(0);
}

/* Go to the next line */
static void goto_new_line(void)
{
	input_end();
	putchar('\n');
}


static inline void out1str(const char *s)
{
	if ( s )
		fputs(s, stdout);
}

static inline void beep(void)
{
	putchar('\007');
}

/* Move back one charactor */
/* special for slow terminal */
static void input_backward(int num)
{
	if (num > cursor)
		num = cursor;
	cursor -= num;          /* new cursor (in command, not terminal) */

	if (cmdedit_x >= num) {         /* no to up line */
		cmdedit_x -= num;
		if (num < 4)
			while (num-- > 0)
				putchar('\b');

		else
			printf("\033[%dD", num);
	} else {
		int count_y;

		if (cmdedit_x) {
			putchar('\r');          /* back to first terminal pos.  */
			num -= cmdedit_x;       /* set previous backward        */
		}
		count_y = 1 + num / cmdedit_termw;
		printf("\033[%dA", count_y);
		cmdedit_y -= count_y;
		/*  require  forward  after  uping   */
		cmdedit_x = cmdedit_termw * count_y - num;
		printf("\033[%dC", cmdedit_x);  /* set term cursor   */
	}
}

static void cle_show_prompt(void)
{
	out1str(prompt);
	cmdedit_x = cmdedit_prmt_len;   /* count real x terminal position */
	cursor = 0;
	cmdedit_y = 0;                  /* new quasireal y */
}



/* draw promt, editor line, and clear tail */
static void redraw(int y, int back_cursor)
{
	if (y > 0)                              /* up to start y */
		printf("\033[%dA", y);
	putchar('\r');
	cle_show_prompt();
	input_end();                            /* rewrite */
	printf("\033[J");                       /* destroy tail after cursor */
	input_backward(back_cursor);
}

/* Delete the char in front of the cursor */
static void input_delete(void)
{
	int j = cursor;

	if (j == len)
		return;

	strcpy(command_ps + j, command_ps + j + 1);
	len--;
	input_end();                    /* rewtite new line */
	cmdedit_set_out_char(0);        /* destroy end char */
	input_backward(cursor - j);     /* back to old pos cursor */
}

/* Delete the char in back of the cursor */
static void input_backspace(void)
{
	if (cursor > 0) {
		input_backward(1);
		input_delete();
	}
}


/* Move forward one charactor */
static void input_forward(void)
{
	if (cursor < len)
		cmdedit_set_out_char(command_ps[cursor + 1]);
}


static void cmdedit_setwidth(int w, int redraw_flg)
{
	cmdedit_termw = cmdedit_prmt_len + 2;
	if (w <= cmdedit_termw) {
		cmdedit_termw = cmdedit_termw % w;
	}
	if (w > cmdedit_termw) {
		cmdedit_termw = w;

		if (redraw_flg) {
			/* new y for current cursor */
			int new_y = (cursor + cmdedit_prmt_len) / w;

			/* redraw */
			redraw((new_y >= cmdedit_y ? new_y : cmdedit_y), len - cursor);
			fflush(stdout);
		}
	}
}

static void cle_init(void)
{
	if ((handlers_sets & SET_WCHG_HANDLERS) == 0) {
		/* emulate usage handler to set handler and call yours work */
		win_changed(-SIGWINCH);
		handlers_sets |= SET_WCHG_HANDLERS;
	}

	if ((handlers_sets & SET_ATEXIT) == 0) {
		handlers_sets |= SET_ATEXIT;
		atexit(cmdedit_reset_term);     /* be sure to do this only once */
	}
}


static void get_previous_history(void)
{
	if(command_ps[0] != 0 || history[cur_history] == 0) {
		free(history[cur_history]);
		history[cur_history] = strdup(command_ps);
	}
	cur_history--;
}

static int get_next_history(void)
{
	int ch = cur_history;

	if (ch < n_history) {
		get_previous_history(); /* save the current history line */
		return (cur_history = ch+1);
	} else {
		beep();
		return 0;
	}
}


static ssize_t cle_read(int fd, void *buf, size_t count)
{
        ssize_t n;

        do {
            n = read(fd, buf, count);
        } while (n < 0 && errno == EINTR);

        return n;
}


void cle_keytrap_on()
{
	getTermSettings(0, (void *) &initial_settings);
	memcpy(&new_settings, &initial_settings, sizeof(struct termios));
	new_settings.c_lflag &= ~ICANON;        /* unbuffered input */
	
	/* Turn off echoing and CTRL-C, so we can trap it */
	new_settings.c_lflag &= ~(ECHO | ECHONL | ISIG);
	setTermSettings(0, (void *) &new_settings);
	handlers_sets |= SET_RESET_TERM;
}


void cle_keytrap_off()
{
	setTermSettings(0, (void *) &initial_settings);
	handlers_sets &= ~SET_RESET_TERM;
	cmdedit_reset_term();
}



int cle_read_cmd(char *command, char sk)
{
	unsigned char c = 0;
	int break_out = 0;

	/* prepare before init handlers */
	cmdedit_y = 0;  /* quasireal y, not true work if line > xt*yt */
	len = 0; cursor = 0;
	command_ps = command;
	//command[0] = 0;
	
	cle_keytrap_on();
	cle_init();	/* Now initialize things */
	
	/* Print out the command prompt */
	cle_show_prompt();

	/* use the command from the kernel */
	if (sk == '\t' || sk =='?') {
		len = strlen(command);
		/* redraw and go to end line */
		redraw(cmdedit_y, 0);
	}
	else command[0] = 0;

	while (break_out == 0) {

		fflush(stdout);                 /* buffered out to fast */
		if (cle_read(0, &c, 1) < 1)
			/* if we can't read input then exit */
			return -1;

		switch (c) {
		case '\n':
		case '\r': /* Enter */
			goto_new_line();
			break_out = 1;
			break;
				
		case '\b':
		case DEL: /* Control-h and DEL */
			input_backspace();
			break;
					
		case ESC:  	/* escape sequence follows */
			{
				if (cle_read(0, &c, 1) < 1)
					return -1;
				/* different vt100 emulations */
				if (c == '[' || c == 'O') {
					if (cle_read(0, &c, 1) < 1)
						return -1;
				}
				switch (c) {
				case 'A':  /* Up Arrow -- Get previous command from history */
					if (cur_history > 0) {
						get_previous_history();
						goto rewrite_line;
					} else beep();
					break;
					
				case 'B': 	/* Down Arrow -- Get next command in history */
					if (!get_next_history())
						break;
					/* Rewrite the line with the selected history item */
	rewrite_line:
					/* change command */
					len = strlen(strcpy(command, history[cur_history]));
					/* redraw and go to end line */
					redraw(cmdedit_y, 0);
					break;
					
				case 'C':	/* Right Arrow -- Move forward one character */
					input_forward(); break;
					
				case 'D': 	/* Left Arrow -- Move back one character */
					input_backward(1); break;
					
				case '3':
					/* Delete */
					input_delete();
					break;
				case '1':
				case 'H':
					/* Home (Ctrl-A) */
					input_backward(cursor);
					break;
				case '4':
				case 'F':
					/* End (Ctrl-E) */
					input_end();
					break;
				default:
					if (!(c >= '1' && c <= '9'))
						c = 0;
					beep();
				}
				if (c >= '1' && c <= '9')
					do
						if (cle_read(0, &c, 1) < 1)
							return -1;
					while (c != '~');
			}
			break;
			
		case '?':  /* ? : for help */
			printf("\n");
			/* pass through */

		case '\t': /* Tab */
			break_out = 1;

		default:        /* If it's regular input, do the normal thing */
			
			/* Skip non-printable characters */
			if (!Isprint(c) && c !='\t' )  /* Ignore TAB */        
				break;
			/* Need to leave space for enter */
			if (len >= (BUFSIZ - 2))        
				break;
			len++;

			/* Append if at the end of the line */
			if (cursor == (len - 1)) {      
				*(command + cursor) = c;
				*(command + cursor + 1) = 0;
				if (c != '\t' && c != '\?')
					cmdedit_set_out_char(0);
			} else { /* Insert otherwise */             
				int sc = cursor;

				memmove(command + sc + 1, command + sc, len - sc);
				*(command + sc) = c;
				sc++;
				/* rewrite from cursor */
				input_end();
				/* to prev x pos + 1 */
				input_backward(cursor - sc);
			}
			break;
		}
	}
	
	cle_keytrap_off();

	 /* Handle command history log */
	/* cleanup may be saved current command line */
	free(history[MAX_HISTORY]);
	history[MAX_HISTORY] = 0;
	if (len > 0 && c!='\t') {
		int i = n_history;
		
		/* After max history, remove the oldest command */
		if (i >= MAX_HISTORY) {
		    free(history[0]);
		    for(i = 0; i < (MAX_HISTORY-1); i++)
		        history[i] = history[i+1];
		}
		history[i++] = strdup(command);
		cur_history = i;
		n_history = i;
		/* prevent history from keeping TAB & ? */
		if (c=='\t' || c=='?')
			history[i-1][len-1] = '\0';
	}
	if(break_out == 1) {
      		command[len++] = '\n';          /* set '\n' */
        	command[len] = 0;
    	}
	return len;
	
}


void show_kernel_info(int fd, char *cmd, char ch)
{
	char buf[1024+10]; /* should sync with kernel: rtl-cle.c */
	char *sc, *ds;
	int len;

	len = read(fd, buf, 256);
	buf[len] = 0;

	/* get prompt */
	for(sc=buf, ds=prompt; *sc!='\0' && *sc !='\t'; sc++, ds++)
		*ds = *sc;
	sc++; *ds++=' '; *ds = 0; 

	/* reset file offset */
	lseek(fd, 0, SEEK_SET);
	
	/* TAB , ?: copy the info from the kernel to command buffer */
	if ((len>0) && (ch == '\t' || ch == '?')) {
		strcpy(cmd, sc);		
	} else *cmd = 0;
	
}


int main(int argc, char *argv[])
{
	char cmd[BUFSIZ], c='\0';
	int len, fd;

	/* open the PROCfs */
	if ((fd=open("/proc/rtcle", O_RDWR)) == -1) {
		printf("cleshell: cannot open /proc/rtcle\n");
		return -1;
	}

	/* Get the prompt from the kernel */
	if (write(fd, "\n", 2)  <= 0) {
		printf("cleshell: write() error!\n");
		return -1;
	}
	show_kernel_info(fd, cmd, 0);
	cmdedit_prmt_len = strlen(prompt);

	/* Start the command editor */
	while  ( (len=cle_read_cmd(cmd, c)) > 0) {
		/* if "exit" command, end the program */
		if (!strcmp(cmd, "exit\n"))
			break;
		/* if only '\n' is returned, no future processing for the command */
		if (cmd[0] == '\n' ) {
			c = 0;
			continue;
		}
		/* otherwise, write the command to the kernel through the PROCfs */
		if (write(fd, cmd, len) <= 0) {
			printf("cleshell: write() error!\n");
			break;
		}
		c = cmd[len-2];
		/* after write command to kernel, try to read data from kernel */
		show_kernel_info(fd, cmd, c);
		if (*cmd == 0 && c!='\t')
			c = '\0';
	}
	/* close the PROCfs */
	close(fd);
	return 0;
	
}




