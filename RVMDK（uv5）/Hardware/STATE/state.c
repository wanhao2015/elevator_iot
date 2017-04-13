#include "state.h"
#include "key.h"


//电梯故障状态变量(在main.c里定义）
extern u8 state_breakdown;   

//上升下降标志(在main.c里定义）
extern u8 Rise_Fall_FLAG;

//电梯门关闭次数（在main.c里定义）
extern u8 Door_closes;

//电梯速度(在main.c中定义)
extern float speed;

void Running_door_open()
{
	if(Rise_Fall_FLAG==0 || Rise_Fall_FLAG==1)  //判断电梯是否为运行状态
	{
		if(Read_Door == 1)                      //电梯门打开    
		{	
			state_breakdown |=1<<1;
			/* if(Read_People==0)                  //有人状态
				printf("运行中开门（有人）故障\r\n");
			else printf("运行中开门（无人）故障\r\n"); */
		}
	}
}

void Out_Speed()
{
	if(speed>MAX_SPEED)
		state_breakdown |=1<<3;
		/* printf("电梯超速\r\n") */
}

void Open_Close_Door()
{
	if(Door_closes>MAX_DOOR_CLOSE)
		state_breakdown |=1<<6;
}

void Warning()
{
	if(Read_warning == 1)
			state_breakdown |=1<<7;
}

void State_Breakdown()
{
	 if((state_breakdown & 1) != 0)
	 {
		 if(Read_People==1)
			 printf("区域外停梯(有人)故障\r\n");
		 else printf("区域外停梯(无人)故障\r\n");
	 }
	 if((state_breakdown & 1<<1) != 0)
	 {
		 if(Read_People==1)
			 printf("运行中开门(有人)故障\r\n");
		 else printf("运行中开门(无人)故障\r\n");
	 }
	 if((state_breakdown & 1<<2) != 0)
		 printf("开门状态下开始运行故障\r\n");
	 if((state_breakdown & 1<<3) != 0)
		 printf("电梯超速\r\n");
	 if((state_breakdown & 1<<4) != 0)
		 printf("平层困人\r\n");
	 if((state_breakdown & 1<<5) != 0)
		 printf("长时间开门故障\r\n");
	 if((state_breakdown & 1<<6) != 0)
		 printf("反复开关门故障\r\n");
	 if((state_breakdown & 1<<7) != 0)
		 printf("手动报警\r\n");
}


