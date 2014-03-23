#ifndef SNAKE_H
#define SNAKE_H

#include <ncurses.h>

#define TRUE	1
#define FALSE	0
typedef int BOOL;

#define STDSCR_BEGIN_Y	0
#define STDSCR_BEGIN_X	0

#define PAINT_SCREEN	' '
#define PAINT_WALL		'0' //ACS_CKBOARD //ACS_DEGREE//ACS_CKBOARD //ACS_DIAMOND
#define PAINT_SNAKE		'o'
#define PAINT_FOOD		ACS_DIAMOND	//'*'

#define NUM_FOOD		1	//
//#define NUM_WALL		5	//

#define TIME_UNIT_USEC	1000					// 1 ms	
#define TIME_DELAY		1000*TIME_UNIT_USEC		// 1s  the delay time when the game start
#define TIME_INC		100*TIME_UNIT_USEC		//time increment

#define KEY_ESC			27

#define MSG_GAME_START	"GAME START"
#define MSG_GAME_OVER	"GAME OVER!"

typedef enum color{
	COLOR_SCREE=1,
	COLOR_WALL,
	COLOR_SNAKE,
	COLOR_FOOD,
	COLOR_MSG,
}color_t;

typedef enum dir{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
}dir_t;//the move direction of the snake

typedef enum status{
	DEAD,
	ALIVE,
	PAUSE,
}status_t;// the status of the snake

typedef struct segment{
	int y;
	int x;
	struct segment *prev;
	struct segment *next;
}segment_t;

typedef struct snake{
	dir_t dir;
	status_t status;
	unsigned int length;		//length of the snake
	unsigned int speed;			//the speed more quick ,the delay time more less
	segment_t *head;
	segment_t *tail;
}snake_t;




typedef enum obj_type{
	OBJ_FOOD,
	OBJ_WALL,
}obj_type_t;

typedef struct object{
	int y;
	int x;
	obj_type_t type;
	struct object *next;
}object_t;

typedef struct screen{
	WINDOW	*win;
	int nlines;
	int nclos;
	int begin_y;
	int begin_x;
	object_t *object;
}screen_t;

void init_snake(screen_t *screen, snake_t *snake);
void handle_snake(screen_t *screen, snake_t *snake);
void paint_snake(screen_t *screen, snake_t *snake);
void exit_snake(screen_t *screen, snake_t *snake);

int key_event(screen_t *screen,snake_t *snake);
screen_t *get_cur_screen_size(screen_t *screen);
int add_segment(snake_t *snake);
int add_object(int y, int x,obj_type_t type, screen_t *screen);
BOOL del_object(object_t *object, screen_t *screen);
void end_snake(snake_t *snake);
void pause_snake(snake_t *snake);
int within_screen(int y, int x, screen_t *screen);
object_t *on_screen_object(int y, int x,screen_t *screen);
int on_snake_body(int y, int x,snake_t *snake);
BOOL move_snake(screen_t *screen, snake_t *snake);
BOOL can_move(screen_t *screen, snake_t *snake);
void new_pos(int *y, int *x,screen_t *screen, snake_t *snake);

//clear the screen
//draw msg
//draw wall
//draw snake
//draw food
void clear_screen(screen_t *screen);
void draw_screen(screen_t *screen);
void draw_snake(screen_t *screen, snake_t *snake);

void start_menu(screen_t *screen, snake_t *snake);
#if 0
void end_menu(screen_t *screen);
#endif

void free_snake(snake_t *snake);
void free_screen(screen_t *screen);

#endif
