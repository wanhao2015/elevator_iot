#include "stdio.h"
#include "state.h"
#include "key.h"

//电梯运行状态变量(全局变量)
STATE_Typedef state = Level_Static;  //当前运行状态

//电梯故障状态变量
u8 state_breakdown=0;               //初始化无故障

//当前层数(初始值)
u8 Floors = 3;

//门关闭次数
u8 Door_closes=0;

//电梯速度
float speed;

//上升下降标志
//0为上升
//1为下降
//2为正常停下(平层静止)    3为非正常停下
u8 Rise_Fall_FLAG = 2; //默认静止
    
//定时器2计数器的保存值，用来计算电梯运行速度
u32 TIME = 0;

void Normal_state()
{
	if(Read_DownLimitSwitch == 0)
			printf("蹲底状态\r\n");
		if(Read_UpLimitSwitch == 0)
			printf("冲顶状态\r\n");

		if(TIME == 0)
			speed = 0;
		else
			speed = (float)10000*HEIGHT/TIME;
		
		printf("当前层数:%d\r\n",Floors);
		
		if(Read_Door == 1)
				printf("电梯门打开\r\n");
		else
				printf("电梯门关闭\r\n");
			
		if(Rise_Fall_FLAG == 0)
			printf("正在上升\r\n");
		else if(Rise_Fall_FLAG == 1)
			printf("正在下降\r\n");
		else if(Rise_Fall_FLAG == 2)
		{
			printf("已经正常停下\r\n");
			speed = 0;
		}
		else
		{
			printf("非正常停下\r\n");
			speed = 0;
		}
		
		printf("运行速度为：%f\r\n",speed);
		printf("\r\n");
}

void Out_Speed()                         //判断电梯是否超速
{
	if(speed>MAX_SPEED)
		state_breakdown |=1<<3;
}
   
void Open_Close_Door()                  //判断电梯门开关次数是否超过设定的阈值
{
	if(Door_closes>MAX_DOOR_CLOSE)
		state_breakdown |=1<<6;
}

void Warning()                         //判断电梯是否触发手动呼救信号
{
	if(Read_warning == 1)
			state_breakdown |=1<<7;
}

void State_Breakdown()                 //各种故障判断
{
// printf("%d\r\n",state_breakdown);
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


