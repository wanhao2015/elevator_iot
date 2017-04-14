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

//总层数
#define MAX_Floor 3
//每层楼的高度(米)
#define HEIGHT 3

//电梯运行最高速度 (m/s)
#define MAX_SPEED 2

//门开关次数阈值
#define MAX_DOOR_CLOSE 10

void Normal_state(void);
//void Running_door_open(void);
void Out_Speed(void);
void Warning(void);
void Open_Close_Door(void);
void State_Breakdown(void);

#endif
