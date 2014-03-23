/*************************************************************************
    Function:ʵ��̰������Ϸ
    Purpose: ��Ϥ��Ϸ�������˽�Ӧ�ó��������߼���UI �������.
    Author:  Others&ZJQ     
    Date:	 2013-06-26 ~ 2013-07-01
    Remark: https://github.com/justinmeza/snakeses 
			̰���߷���
			��̰������Ϸ��������״̬�������������ͣ��
			̰������˫������������,�������б����˱���̰����ÿ����ɲ��ֵ�����ֵ(y,x);
			̰�����е��ϰ����ʳ����һ�������������棬ÿ���������㱣����������ֵ�Լ�����(�ϰ���orʳ��)��
			��ʼ״̬Ϊ��ͣ,��ȡ������Ϣ���õ�̰����ǰ������,��̰���߲�ȡ��ǰ�����жϵķ���,��̰���ߵ�β���Ƶ�ͷ��ʹ���Ϊ�µ�ͷ����
			������ͷ������,Ȼ�����жϴ�ͷ�������Ƿ�Ϸ�(��û�г�����Ļ�������ϰ���,Ҳ����˵��û����ͷ�������ڵ���������),
			�������ϰ����������Ϸ,�ͷ���Դ;������ʳ��������+1 �����,��β���ڵ㿽��һ��ֱ������ӵ�β��(�˴���ʾβ���ڵ�͵����ڶ����ڵ�һ��,
			��ʾ��̰���߳���û��,�´����ƶ���ʱ��,��ʾ�ĳ��Ⱦ�������һ�����),����ʳ��ڵ�ӵ�������ɾ��,�����ѡ��һ���Ϸ���λ��
			(����Ļ��Χ��,����������������ϰ����ʳ����)������ʳ��,������ʳ����ӵ���������.Ȼ���ټ�����һ�ֻ�ȡ������Ϣ->......
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
