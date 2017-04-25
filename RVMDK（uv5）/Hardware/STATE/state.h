#ifndef __STATE_H
#define __STATE_H	 

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

//运行状态变量（在state.c里定义）
extern STATE_Typedef state;  //当前运行状态

//电梯故障状态变量(在state.c里定义）
extern u8 state_breakdown;

//当前层数(在state.c里定义）	
extern u8 Floors;

//电梯门关闭次数（在state.c里定义）
extern u8 Door_closes;

//上升下降标志(在state.c里定义）
extern u8 Rise_Fall_FLAG;

//定时器2计数器的保存值，用来计算电梯运行速度(在state.c里定义）
extern u32 TIME;

#endif
