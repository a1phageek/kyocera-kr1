#ifndef CLESHELL_H
#define CLESHELL_H

int     cmdedit_read_input(char* promptStr, char* command);

#ifdef CONFIG_FEATURE_COMMAND_SAVEHISTORY
void    load_history ( const char *fromfile );
void    save_history ( const char *tofile );
#endif

#endif /* CLESHELL_H */
