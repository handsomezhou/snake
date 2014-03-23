/*************************************************************************
    Function:实现贪吃蛇游戏
    Purpose: 熟悉游戏开发，了解应用程序中其逻辑和UI 处理过程.
    Author:  Others&ZJQ     
    Date:	 2013-06-26 ~ 2013-07-01
    Remark: https://github.com/justinmeza/snakeses 
			贪吃蛇分析
			本贪吃蛇游戏具有三种状态：活动、结束和暂停；
			贪吃蛇用双向链表来保存,链表结点中保存了保存贪吃蛇每个组成部分的坐标值(y,x);
			贪吃蛇中的障碍物和食物用一个单链表来保存，每个单链表结点保存了其坐标值以及类型(障碍物or食物)；
			初始状态为暂停,获取键盘消息，得到贪吃蛇前进方向,本贪吃蛇采取先前进后判断的方法,将贪吃蛇的尾部移到头部使其成为新的头部，
			更新其头部坐标,然后再判断此头部坐标是否合法(有没有超出屏幕或碰到障碍物,也就是说有没有其头部坐标在单链表借点中),
			若碰到障碍物则结束游戏,释放资源;若碰到食物则将蛇身+1 个结点,将尾部节点拷贝一份直接再添加到尾部(此次显示尾部节点和倒数第二个节点一样,
			显示的贪吃蛇长度没变,下次再移动的时候,显示的长度就增加了一个结点),将此食物节点从单链表中删除,随机再选择一个合法的位置
			(在屏幕范围内,但不在蛇身或已有障碍物或食物上)生成新食物,并将新食物添加到单链表中.然后再继续下一轮获取键盘消息->......
    Version: 0.1
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "snake.h"

int main(void)
{
	screen_t *scr=NULL;
	snake_t *snk=NULL;
	scr=(screen_t *)malloc(sizeof(screen_t));
	if(NULL==scr){
		printf("Failed to allocate space for the pointer of screen_t!\n");
		exit_snake(scr,snk);
		exit(EXIT_FAILURE);
	}

	snk=(snake_t *)malloc(sizeof(snake_t));
	if(NULL==snk){
		printf("Failed to allocate space for the pointer of snake_t!\n");
		exit_snake(scr,snk);
		exit(EXIT_FAILURE);
	}
	
	init_snake(scr,snk);
	//wgetch(scr->win);
	while(DEAD != snk->status){
		handle_snake(scr,snk);
		paint_snake(scr,snk);
		usleep(snk->speed/5);
	}

	if(scr!=NULL||snk!=NULL){
		exit_snake(scr,snk);
		scr=NULL;
		snk=NULL;
	}
	
	
	return 0;
}
