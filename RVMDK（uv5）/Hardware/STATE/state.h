#ifndef __MAIN_H
#define __MAIN_H	 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

//定义一个枚举变量表示电梯运行状态
typedef enum
{
	Level_Dynamic = 0x00,
	Level_Static,
	Rise_One,
	Rise_Two,
	Rise_Three,
	Fall_One,
	Fall_Two,
	Fall_Three
}STATE_Typedef;

//typedef enum
//{
//	No_breakdown,              //无故障
//	Elevator_stopping,         //区域外停梯故障        
//	Door_opening_run,          //开门状态下开始运行故障
//	Level_Static_trapping,     //平层困人故障
////	Door_opened_continued,     //长时间开门
//	Open_close_door            //反复开关门故障
//}STATE_BREAK;

//总层数
#define MAX_Floor 3
//每层楼的高度(米)
#define HEIGHT 3

//电梯运行最高速度 (m/s)
#define MAX_SPEED 2

//门开关次数阈值
#define MAX_DOOR_CLOSE 10

void Running_door_open(void);
void Out_Speed(void);
void Warning(void);

#endif
