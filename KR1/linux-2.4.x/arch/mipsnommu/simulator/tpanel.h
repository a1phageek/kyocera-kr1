/*
 * tow level for TC3f143f feature
 */
#define	TPANEL_SAVE_NOTREADY	0x8000
#define	TPANEL_PEN_DOWN		0x4000
#define	TPANEL_PEN_UP		0x2000
#define	TPANEL_PEN_TIMER	0x1000

typedef	struct wait_queue *wait_queue_head_t1;
//#define	init_waitqueue_head(head)	*(head) = NULL

#define	TP_CLEAN_BUF   	_IO('x', 0x01)

struct tpanel_struct {
	int	current_flag;
	int	save_flag;
	int	pre_flag;
	unsigned int	curr_x;
	unsigned int	curr_y;
	unsigned int	save_x;
	unsigned int	save_y;
	int	active;
	struct fasync_struct *fasyncptr;
	struct sibinfo *sib;
	struct sibirq *sibirq;
	wait_queue_head_t1 wait;
};

struct tpanel_buf_struct {
	int	flag;
	unsigned int	x;
	unsigned int	y;
};


struct tp_data *tp_data;
extern struct tpanel_struct tpanel;

