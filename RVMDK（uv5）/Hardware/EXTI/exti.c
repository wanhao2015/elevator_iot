#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
#include "state.h"
#include "timer.h"
#include "stdio.h"
	
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

//外部中断初始化程序
//初始化PE1~2,PE1~2为中断输入.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	KEY_Init(); //按键对应的IO口初始化
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);//PE1 连接到中断线1
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);//PE0 连接到中断线0
	
	/* 配置EXTI_Line1,2,3*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line2 | EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //边沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//外部中断1
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置	  

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置	
}

//外部中断1服务程序(A)
void EXTI1_IRQHandler(void)
{
	if(Read_UpLevelingSwitch == 1)
	{
		//上升沿触发
		if(Read_DownLevelingSwitch == 0 && state == Rise_Two) //判断当前运行状态是不是上升2
			state = Rise_Three;                    //此时运行状态转换为上升3
		if(Read_DownLevelingSwitch == 1 && state == Fall_Three) //判断当前运行状态是不是下降3
		{
			state = Level_Dynamic;                 //此时运行状态转换为平层运动
			TIM_Cmd(TIM3,ENABLE);                  //使能定时器3
//			TIM4->CNT=0;                           //清零定时器4的计数器
//			TIM_Cmd(TIM4,DISABLE);                 //关闭定时器4
			
			if(Rise_Fall_FLAG == 0)
				Floors++;                            //层数加1
			else if(Rise_Fall_FLAG == 1)
				Floors--;                            //层数减1
		}
	}
	else
//判断当前运行状态是不是平层静止或平层运动或下降1
//下降1指的是干扰引起的误判断
	{
		//下降沿触发 
		if(Read_DownLevelingSwitch == 1 && (state == Level_Static || state == Level_Dynamic || state == Fall_One)) 
		{
			if(state == Level_Static && Read_Door == 1)                     //判断是否是开门状态下开始运行故障
					state_breakdown |=1<<2;
				
			state = Rise_One;                      //当前运行状态转换为上升1
			Rise_Fall_FLAG = 0;                    //正在上升
//			TIM_Cmd(TIM4,ENABLE);                  //使能定时器4
			
			TIM5->CNT=0;                             //清零定时器5的计数器
			TIM_Cmd(TIM5,DISABLE);                   //关闭定时器5	
			
			Door_closes=0;                            //门关闭次数清0
		}
		if(Read_DownLevelingSwitch == 0 && state == Fall_One) //判断当前运行状态是不是下降1
		{
			TIM3->CNT=0;                           //清零定时器3的计数器
		  TIM_Cmd(TIM3,DISABLE);                 //去使能定时器3
			state = Fall_Two;                      //此时运行状态转换为下降2
			 
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line1);        //清除LINE1上的中断标志位 
}

//外部中断2服务程序(B)
void EXTI2_IRQHandler(void)
{
	if(Read_DownLevelingSwitch == 1)
	{
		//上升沿触发
		if(Read_UpLevelingSwitch == 1 && state == Rise_Three) //判断当前运行状态是不是上升3           
		{
			state = Level_Dynamic;                //此时运行状态转换为平层运动
//			TIM4->CNT=0;                          //清零定时器4的计数器
//			TIM_Cmd(TIM4,DISABLE);                //关闭定时器4
			TIM_Cmd(TIM3,ENABLE);                 //使能定时器3
			
			TIM_Cmd(TIM2,DISABLE);                //关闭定时器2
			TIME = TIM2->CNT;                     //读取定时器2的计数器
			
			if(Rise_Fall_FLAG == 0)
				Floors++;                           //层数加1
			else if(Rise_Fall_FLAG == 1)
				Floors--;                            //层数减1
		}
		if(Read_UpLevelingSwitch == 0 && state == Fall_Two) //判断当前运行状态是不是下降2
		{
			TIM_Cmd(TIM2,DISABLE);                //关闭定时器2
			TIME = TIM2->CNT;                     //读取定时器2的计数器

			state = Fall_Three;                   //此时运行状态转换为下降3
		}
	}
	else
	{
		//下降沿触发 
		if(Read_UpLevelingSwitch==0 && state == Rise_One) //判断当前运行状态是不是上升1
		{
			TIM3->CNT=0;                          //清零定时器3的计数器
			TIM_Cmd(TIM3,DISABLE);                //关闭定时器3
			TIM2->CNT=0;                          //清零定时器2的计数器
			TIM_Cmd(TIM2,ENABLE);                 //打开定时器2，开始计时

			state = Rise_Two;                    	//此时运行状态转换为上升2
		}
//判断当前运行状态是不是平层静止或平层运动或上升1
//上升1指的是干扰引起的误判断
		if(Read_UpLevelingSwitch==1 && (state == Level_Static || state == Level_Dynamic || state == Rise_One)) 
		{
			TIM2->CNT=0;                          //清零定时器2的计数器
			TIM_Cmd(TIM2,ENABLE);                 //打开定时器2，开始计时
		
			if(state == Level_Static && Read_Door == 1)   //判断是否是开门状态下开始运行故障
				state_breakdown |=1<<2;
			
			state = Fall_One;                     //此时运行状态转换为下降1
			Rise_Fall_FLAG = 1;                    //正在下降
			
//		TIM_Cmd(TIM4,ENABLE);                  //使能定时器4
			
			TIM5->CNT=0;                             //清零定时器5的计数器
			TIM_Cmd(TIM5,DISABLE);                   //关闭定时器5	
			
			Door_closes=0;                            //门关闭次数清0
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line2);       //清除LINE2上的中断标志位 
}

void EXTI0_IRQHandler(void)                 //门开关状态触发   平层困人故障
{
	if(Read_Door == 0)                        //门关闭
	{
		printf("门关\r\n");
		state_breakdown &=~(1<<5);
		TIM7->CNT=0;                             //清零定时器7的计数器
		TIM_Cmd(TIM7,DISABLE);                   //关闭定时器7		
		
		if(Rise_Fall_FLAG == 2)                  //平层静止
		{
			Door_closes++;                            //门开关次数加一
			if(Read_People==1)                       //有人状态
				TIM_Cmd(TIM5,ENABLE);                  //使能定时器5
		}
	}
	else                                        //门打开
		{
			printf("门开\r\n");
			TIM_Cmd(TIM7,ENABLE);                   //使能定时器7
			if(Rise_Fall_FLAG == 0 || Rise_Fall_FLAG == 1)
				state_breakdown |=1<<1;
			
	//		Door_close++;                            //门开关次数加一			            
	
			TIM5->CNT=0;                             //清零定时器5的计数器
			TIM_Cmd(TIM5,DISABLE);                   //关闭定时器5	
		}
		EXTI_ClearITPendingBit(EXTI_Line0);       //清除LINE0上的中断标志位 		
}
