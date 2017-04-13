#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "state.h"
#include "exti.h"
#include "math.h"

//电梯运行状态变量(全局变量)
STATE_Typedef state = Level_Static;  //当前运行状态

////电梯故障状态变量
//STATE_BREAK state_breakdown=No_breakdown;     //初始化无故障

u8 state_breakdown=0;

//当前层数(初始值)
u8 Floors = 3;

//门关闭次数
u8 Door_closes=0;

//速度
float speed;

//上升下降标志
//0为上升
//1为下降
//其他为静止
u8 Rise_Fall_FLAG = 2; //默认静止


////故障状态标志
//u8 BREAKDWON_FLAG;     

//定时器2计数器的保存值，用来计算电梯运行速度
u32 TIME = 0;

int main(void)
{ 
	
	delay_init(168);  //初始化延时函数
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	LED_Init();				//初始化LED端口 
	EXTIX_Init();     //初始化外部中断
	
	TIM2_Int_Init(pow(2,32)-1,8400-1);      //定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率 	
	TIM3_Int_Init(10000-1,8400-1);      //定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数10000次为1s 
	TIM4_Int_Init(20000-1,8400-1);      //区域外停梯超出设定时间
	TIM5_Int_Init(40000-1,8400-1);      //平层困人时间阈值
	TIM7_Int_Init(40000-1,8400-1);      //长时间开门时间阈值
	
	uart_init(9600);
	
	printf("\r\n Welcome to use STM32F407 \r\n");
 
	while(1)
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
		else
		{
			printf("已经停下\r\n");
			speed = 0;
		}
		
		printf("运行速度为：%f\r\n",speed);
		printf("\r\n");
		
		LED0=1;
		delay_ms(100);
		LED0=0;
		delay_ms(100);
	}
}
