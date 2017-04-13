#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

void TIM2_Int_Init(u32 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
void TIM5_Int_Init(u32 arr,u16 psc);
void TIM7_Int_Init(u16 arr,u16 psc);

#endif
