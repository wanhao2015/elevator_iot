#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 

/*下面的方式是通过直接操作库函数方式读取IO*/

#define Read_Door  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)//读取门开关
#define Read_UpLevelingSwitch  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)//读取上平层开关
#define Read_DownLevelingSwitch  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取下平层开关
#define Read_DownLimitSwitch  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取上限位开关
#define Read_UpLimitSwitch  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取下限位开关
#define Read_People  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)        //读取电梯里是否有人     
#define Read_Warning  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)       //读取手动报警呼救按钮触点信号
#define KEY7  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//读取按键1  L
#define KEY8  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15)//读取按键1  L
#define KEY9  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)//读取按键1  L
#define KEY10  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)//读取按键1  L
#define KEY11  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)//读取按键1  L
#define KEY12  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)//读取按键1  L
#define KEY13  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8)//读取按键1  L	

void KEY_Init(void);	//IO初始化

#endif
