#include <ncurses.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "snake.h"

void init_snake(screen_t *screen, snake_t *snake)
{
	//init ncurses
	screen_t *scr=screen;
	snake_t *snk=snake;
	assert(NULL!=scr&&NULL!=snk);

	initscr();
	cbreak();
	noecho();
	curs_set(FALSE);
	clear();
	
	if(has_colors()){
		start_color();
		init_pair(COLOR_SCREE,COLOR_WHITE,COLOR_BLACK);
		init_pair(COLOR_WALL,COLOR_BLUE,COLOR_BLACK);
		init_pair(COLOR_SNAKE,COLOR_GREEN,COLOR_BLACK);
		init_pair(COLOR_FOOD,COLOR_YELLOW,COLOR_BLACK);
		init_pair(COLOR_MSG,COLOR_YELLOW,COLOR_BLACK);
		
	}

	//init screen
	scr->win=stdscr;
	scr->begin_y=STDSCR_BEGIN_Y;
	scr->begin_x=STDSCR_BEGIN_X;
	get_cur_screen_size(scr);
	scr->object=NULL;
	
	int i=0;
	int j=0;
	for(i=scr->begin_x; i<scr->nclos; i++){
		for(j=scr->begin_y; j<scr->nlines; j++){
			if(i==scr->begin_x||i==scr->nclos-1||j==scr->begin_y||j==scr->nlines-1){
				//mvwprintw(scr->win,j,i,'%d','*');
			//	mvwaddch(scr->win,j,i,'*');
				add_object(j,i,OBJ_WALL,scr);
			}
		}
	}

	int y=1,x=1;
	for(i=0; i<NUM_FOOD; i++){
		new_pos(&y,&x,scr,snk);
		add_object(y,x,OBJ_FOOD,scr);
	}

	add_object(scr->begin_y+1,scr->begin_x+1,OBJ_FOOD,scr);
	add_object(scr->begin_y+1,scr->nclos-2,OBJ_FOOD,scr);
	add_object(scr->nlines-2,scr->begin_x+1,OBJ_FOOD,scr);
	add_object(scr->nlines-2,scr->nclos-2,OBJ_FOOD,scr);
	keypad(scr->win,TRUE);
	//nodelay(scr->win,TRUE);//It will not work, when the second parameter is selected to be true
	
	//init snake
	snk->dir=NONE;
	snk->status=ALIVE;
	snk->length=1;
	snk->speed=TIME_DELAY;
	segment_t *segment=(segment_t *)malloc(sizeof(segment_t));
	if(NULL==segment){
		printf("Failed to allocate space for the pointer of segment_t!\n");
		end_snake(snk);
		return ;
		//exit(EXIT_FAILURE);
	}
	snk->head=segment;
	snk->head->prev=NULL;
	snk->head->next=NULL;

	snk->tail=segment;//snk->head;
	snk->tail->prev=NULL;
	snk->tail->next=NULL;

	new_pos(&snk->head->y,&snk->head->x,scr,snk);

	start_menu(scr,snk);
	nodelay(scr->win,TRUE);//It will not work, when the second parameter is selected to be true
	
}

void handle_snake(screen_t *screen, snake_t *snake)
{
	screen_t *scr=screen;
	snake_t *snk=snake;
	assert(NULL!=scr&&NULL!=snk);
	int y,x;
	key_event(scr,snk);
	
	if(FALSE!=move_snake(scr,snk)){
		object_t *cur=NULL;
		cur=on_screen_object(snk->head->y,snk->head->x,scr);
		if(NULL!=cur){//on screen(wall or food)
			switch(cur->type){
				case OBJ_WALL:
					end_snake(snk);
					break;
				case OBJ_FOOD:
					add_segment(snk);
					if(del_object(cur,scr)==TRUE){
						new_pos(&y,&x,scr,snk);
						add_object(y,x,OBJ_FOOD,scr);
					}
					break;
			}

		}
	}
	
}

void paint_snake(screen_t *screen, snake_t *snake)
{
	//clear the screen
	//draw msg
	//draw wall and food
	//draw snake
	screen_t *scr=screen;
	snake_t *snk=snake;
	assert(NULL!=scr&&NULL!=snk);
	
	clear_screen(scr);
	draw_screen(scr);
	draw_snake(scr,snk);
	wrefresh(scr->win);
	
}

void exit_snake(screen_t *screen, snake_t *snake)
{
	screen_t *scr=screen;
	snake_t  *snk=snake;

	if(NULL!=scr){
		free_screen(scr);
		scr=NULL;
	}

	if(NULL!=snk){
		free_snake(snk);
		snk=NULL;
	}
		
	endwin();
}

int key_event(screen_t *screen,snake_t *snake)
{
	screen_t *scr=screen;
	snake_t *snk=snake;
	
	assert(NULL!=snk);
	int ch=wgetch(scr->win);
	
	switch(ch){
		case 'w':
		case 'W':
		case 'i':
		case 'I':
		case KEY_UP:
			if(DOWN!=snk->dir){
				if(PAUSE!=snk->status){
					snk->dir=UP;
				}
			}
			break;

		case 's':
		case 'S':
		case 'k':
		case 'K':
		case KEY_DOWN:
			if(UP!=snk->dir){
				if(PAUSE!=snk->status){
					snk->dir=DOWN;
				}
			}
			break;

		case 'a':
		case 'A':
		case 'j':
		case 'J':
		case KEY_LEFT:
			if(RIGHT!=snk->dir){
				if(PAUSE!=snk->status){
					snk->dir=LEFT;
				}
				
			}
			break;

		case 'd':
		case 'D':
		case 'l':
		case 'L':
		case KEY_RIGHT:
			if(LEFT!=snk->dir){
				if(PAUSE!=snk->status){
					snk->dir=RIGHT;
				}
			}
			break;

		case 'q':
		case 'Q':
		case KEY_ESC:
			end_snake(snk);
			break;

		case 'p':
		case 'P':
		case ' ':
			pause_snake(snk);
			break;
			
	}
	
	return ch;
}

screen_t *get_cur_screen_size(screen_t *screen)
{
	screen_t *scr=screen;
	if(NULL==scr){
		return NULL;
	}
#if 0
	scr->win=stdscr;
	scr->begin_y=STDSCR_BEGIN_Y;
	scr->begin_x=STDSCR_BEGIN_X;
#endif
	getmaxyx(scr->win,scr->nlines,scr->nclos);
//	scr->object=NULL;
//	keypad(scr->win,TRUE);
//	box(scr->win,PAINT_WALL,PAINT_WALL);
//	wrefresh(scr->win);
	return scr;
}

int add_segment(snake_t *snake)
{
	snake_t *snk=snake;
	assert(NULL!=snk);
	segment_t *newseg=(segment_t *)malloc(sizeof(segment_t));
	if(NULL==newseg){
		printf("Failed to allocate space for the pointer of segment_t!\n");
		end_snake(snk);
		return -1;
		//exit(EXIT_FAILURE);
	}
	newseg->y=snk->tail->y;
	newseg->x=snk->tail->x;
	
	newseg->prev=snk->tail;
	newseg->next=NULL;
	newseg->prev->next=newseg;
	snk->tail=newseg;
	snk->length++;
	
	return 0;
}

int add_object(int y, int x,obj_type_t type, screen_t *screen)
{
	screen_t *scr=screen;
	assert(NULL!=scr);
	object_t *cur=NULL;
	object_t *prev=NULL;
	object_t *tmp=(object_t *)malloc(sizeof(object_t));
	tmp->type=type;
	tmp->y = y;
	tmp->x = x;
	tmp->next=NULL;

	for(prev=scr->object,cur=scr->object;cur!=NULL;cur=cur->next){
		prev=cur;
	}
	
	if(NULL==prev){
		scr->object=tmp;
	}else{
		prev->next=tmp;
	}
	
	return 0;
}

BOOL del_object(object_t *object, screen_t *screen)
{
	object_t *obj=object;
	screen_t *scr=screen;
	assert(NULL!=scr);
	if(NULL==obj){
		return FALSE;
	}

	object_t *cur=NULL;
	object_t *prev=NULL;
	
	for(cur=scr->object;cur!=NULL;prev=cur, cur=cur->next){
		if(obj->y==cur->y&&obj->x==cur->x){
			if(NULL==prev){
				scr->object=cur->next;
			}else{
				prev->next=cur->next;
			}
			free(cur);
			cur=NULL;
			break;
		}
	}
	
	return TRUE;
}

void end_snake(snake_t *snake)
{
	snake_t *snk=snake;
	assert(NULL!=snk);

	snk->status=DEAD;
	
	return ;
}

void pause_snake(snake_t *snake)
{
	snake_t *snk=snake;
	assert(NULL!=snk);

	if(PAUSE != snk->status){
		snk->status=PAUSE;
	}else{
		snk->status=ALIVE;
	}
	
	return ;
}

int within_screen(int y, int x, screen_t *screen)
{
	screen_t *scr=screen;
	assert(NULL!=scr);

	if(y>scr->begin_y&&y<scr->nlines-1&&x>scr->begin_x&&x<scr->nclos-1){
		return TRUE;		
	}else{
		return FALSE;
	}
}

object_t *on_screen_object(int y, int x,screen_t *screen)
{
	screen_t *scr=screen;
	if(NULL==scr){
		return NULL;
	}
	
	object_t *cur;
	for(cur=scr->object; cur!=NULL; cur=cur->next){
		if(y==cur->y&&x==cur->x){
			return cur;
		}
	}
	
	return NULL;
}

int on_snake_body(int y, int x,snake_t *snake)
{
	snake_t *snk=snake;
//	assert(NULL!=snk);
	if(NULL==snk){
		return FALSE;
	}

	if(NULL==snk->head){
		return FALSE;
	}
	
	segment_t *cur=NULL;
	for(cur=snk->head->next; cur!=NULL; cur=cur->next){//from the next node of head node
		if(y==cur->y&&x==cur->x){
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL move_snake(screen_t *screen, snake_t *snake)
{
	screen_t *scr=screen;
	snake_t *snk=snake;
	if(NULL==scr||NULL==snk){
		return FALSE;
	}

	if(snk->status==PAUSE||snk->status==DEAD){
		return FALSE;
	}
	
	
	segment_t *tmp=snk->tail;
	//Move the tail to the head
	if(snk->head!=snk->tail){
		//Tail pointer moves one step forward
		snk->tail=tmp->prev;
		snk->tail->next=NULL;

		//move the end node of snake to the head
		tmp->y=snk->head->y;
		tmp->x=snk->head->x;
		tmp->prev=NULL;
		tmp->next=snk->head;
		snk->head->prev=tmp;

		//Head pointer move to point to the new head node
		snk->head=tmp;
		
	}

	//Move the head
	switch(snk->dir){
		case UP:
			snk->head->y--;
			break;
			
		case DOWN:
			snk->head->y++;
			break;
			
		case LEFT:
			snk->head->x--;
			break;
			
		case RIGHT:
			snk->head->x++;
			break;
			
		default:
			break;
	}

	if(FALSE==can_move(scr,snk)){
		end_snake(snk);
	}
	
	return TRUE;
}

BOOL can_move(screen_t *screen, snake_t *snake)
{
	screen_t *scr=screen;
	snake_t *snk=snake;
	if(NULL==scr||NULL==snk){
		return FALSE;
	}

	if((TRUE==within_screen(snk->head->y,snk->head->x,scr))&&\
		FALSE==on_snake_body(snk->head->y,snk->head->x,snk)){
		return TRUE;
	}else{
		return FALSE;
	}
}

void new_pos(int *y, int *x,screen_t *screen, snake_t *snake)
{
	screen_t *scr=screen;
	snake_t *snk=snake;
	assert(NULL!=scr);
	
	srand((unsigned int)(time(NULL)));
	do{
		//Boundary value:scr->begin_x,scr->nclos-1,scr->begin_y,scr->nlines-1
		//so we need the rand number of the x between scr->begin_x+1 to scr->nclos-2;
		*y=rand()%(scr->nlines-2 - scr->begin_y)+scr->begin_y+1;
		*x=rand()%(scr->nclos-2 - scr->begin_x)+scr->begin_x+1;
	}while(NULL!=on_screen_object(*y,*x,scr) || FALSE!=on_snake_body(*y,*x,snk));
	
	return ;
}
void clear_screen(screen_t *screen)
{
	screen_t *scr=screen;
	if(NULL==scr){
		return ;
	}
#if 0
	wclear(scr->win);
	wrefresh(scr->win);
#endif
	if(has_colors()){
		attron(COLOR_PAIR(COLOR_SCREE));
	}
	int y,x;
	for(x=scr->begin_x; x<scr->nclos; x++){
		for(y=scr->begin_y; y<scr->nlines; y++){
			mvwaddch(scr->win,y,x,PAINT_SCREEN);
		}
	}
	if(has_colors()){
		attroff(COLOR_PAIR(COLOR_SCREE));
	}
	wrefresh(scr->win);
}

void draw_screen(screen_t *screen)
{
	screen_t *scr=screen;
	if(NULL==scr){
		return ;
	}
	object_t *cur=NULL;
	for(cur=scr->object;cur!=NULL; cur=cur->next){
		switch(cur->type){
			case OBJ_FOOD:
				if(has_colors())attron(COLOR_PAIR(COLOR_FOOD));
				mvwaddch(scr->win,cur->y,cur->x,PAINT_FOOD);
				if(has_colors())attroff(COLOR_PAIR(COLOR_FOOD));
				break;
			case OBJ_WALL:
				if(has_colors())attron(COLOR_PAIR(COLOR_WALL));
				mvwaddch(scr->win,cur->y,cur->x,PAINT_WALL);
				if(has_colors())attroff(COLOR_PAIR(COLOR_WALL));
				break;
		}
	}

	//wrefresh(scr->win);
	
}
void draw_snake(screen_t *screen, snake_t *snake)
{
	screen_t *scr=screen;
	snake_t *snk=snake;
	if(NULL==scr||NULL==snk){
		return ;
	}
	segment_t *cur=NULL;
	if(has_colors())attron(COLOR_PAIR(COLOR_SNAKE));
	for(cur=snk->head;cur!=NULL;cur=cur->next){
		mvwaddch(scr->win,cur->y,cur->x,PAINT_SNAKE);
	}
	if(has_colors())attroff(COLOR_PAIR(COLOR_SNAKE));
	//mvwprintw(scr->win,scr->nlines/2,scr->nclos/2,"(%2d,%2d)",cur->y,cur->x);
	//	mvwprintw(scr->win,scr->nlines/2,scr->nclos/2,"(%2d,%2d)",snk->head->y,snk->head->x);
	//wrefresh(scr->win);
	
	return ;
}

void start_menu(screen_t *screen,snake_t *snake)
{
	screen_t *scr=screen;
	snake_t *snk=snake;
	assert(NULL!=scr);
	assert(NULL!=snk);
	
	clear_screen(scr);
	int center_x=scr->nclos/2;
	//int center_y=scr->nlines/2;
	char help[]="Snake game help";
	char pause[]="PAUSE:[P] [p] [Space]";
	char dire[]="DIRECTION:";
	char exit[]="EXIT:[ESC]";

	if(has_colors())attron(COLOR_PAIR(COLOR_MSG));
	mvwprintw(scr->win,scr->begin_y+1,center_x-strlen(help)/2,"%s",help);
	mvwprintw(scr->win,scr->begin_y+3,scr->begin_x+1,"%s",pause);
	mvwprintw(scr->win,scr->begin_y+5,scr->begin_x+1,"%s",dire);

	mvwprintw(scr->win,scr->begin_y+6,scr->begin_x+1+strlen(dire)+1,"%c",'W');
	mvwprintw(scr->win,scr->begin_y+7,scr->begin_x+1+strlen(dire)-1,"%c",'A');
	mvwprintw(scr->win,scr->begin_y+7,scr->begin_x+1+strlen(dire)+3,"%c",'D');
	mvwprintw(scr->win,scr->begin_y+8,scr->begin_x+1+strlen(dire)+1,"%c",'S');

	mvwprintw(scr->win,scr->begin_y+6,scr->begin_x+1+strlen(dire)+1+8,"%c",'I');
	mvwprintw(scr->win,scr->begin_y+7,scr->begin_x+1+strlen(dire)-1+8,"%c",'H');
	mvwprintw(scr->win,scr->begin_y+7,scr->begin_x+1+strlen(dire)+3+8,"%c",'L');
	mvwprintw(scr->win,scr->begin_y+8,scr->begin_x+1+strlen(dire)+1+8,"%c",'K');

	mvwprintw(scr->win,scr->begin_y+6,scr->begin_x+1+strlen(dire)+1+24,"%s"," UP");
	mvwprintw(scr->win,scr->begin_y+7,scr->begin_x+1+strlen(dire)-1+24-4,"%s","LEFT");
	mvwprintw(scr->win,scr->begin_y+7,scr->begin_x+1+strlen(dire)+3+24+4,"%s","RIGHT");
	mvwprintw(scr->win,scr->begin_y+8,scr->begin_x+1+strlen(dire)+1+24,"%s","DOWN");
	mvwprintw(scr->win,scr->begin_y+10,scr->begin_x+1,"%s",exit);
	if(has_colors())attroff(COLOR_PAIR(COLOR_MSG));
	
	box(scr->win,0,0);
	wrefresh(scr->win);
	int ch=wgetch(scr->win);
	if(ch==KEY_ESC){
		snk->status=DEAD;
	}

	
}

#if 0
void end_menu(screen_t *screen, snake_t *snake)
{
	screen_t *scr=screen;
	snake_t *snk=snake;
	assert(NULL!=scr);

	char over[]="GAME OVER!";
	clear_screen(scr);
	int center_x=scr->nclos/2;
	int center_y=scr->nlines/2;
	nodelay(scr->win,FALSE);

	
	nodelay(scr->win,TRUE);
	
}
#endif
void free_snake(snake_t *snake)
{
	snake_t *snk=snake;
	if(NULL==snk){
		return ;
	}

	segment_t *cur=NULL;
	for(cur=snk->head; cur!=NULL; cur=cur->next){
		free(cur);
	}

	free(snk);
	snk=NULL; //only limit to use it in this function

	return ;
}

void free_screen(screen_t *screen)
{
	screen_t *scr=screen;
	if(NULL==scr){
		return ;
	}
	object_t *cur=NULL;
	for(cur=scr->object; cur!=NULL; cur=cur->next){
		free(cur);
	}

	free(scr);
	scr=NULL;

	return ;
}

