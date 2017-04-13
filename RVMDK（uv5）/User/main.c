#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "state.h"
#include "exti.h"
#include "math.h"

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
		Normal_state();
		Running_door_open();
		Out_Speed();
		Open_Close_Door();
		Warning();
		State_Breakdown();
		
		LED0=1;
		delay_ms(100);
		LED0=0;
		delay_ms(100);
	}
}
