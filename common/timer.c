#include "timer.h"
#include "mcu.h"

#define overflow 60000
#define overhead 144
#define overhead2 25
#define unstablezone 2

static volatile int64_t overflow_count = 0;
static volatile int us_cycle_count;
static volatile int overflow_time_us;

void TIM2_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
	overflow_count += overflow;	
}

int init_timer(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	#ifdef STM32F1
	us_cycle_count = SystemCoreClock/1000000;
	#endif
	#ifdef STM32F4
	us_cycle_count = 84;
	#endif
	overflow_time_us = overflow / us_cycle_count / 2;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;      
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_DeInit(TIM2);
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseStructure.TIM_Prescaler=0;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=overflow-1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM2,DISABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

	TIM_Cmd(TIM2,ENABLE);
	
	return 0;
}
int64_t gettick(void)
{
	volatile int i = TIM2->CNT;
	if(i <us_cycle_count*unstablezone || i > overflow-us_cycle_count*unstablezone)
		delayus(unstablezone);
	
	return overflow_count + TIM2->CNT;
}
int64_t getus(void)
{
	return gettick() / us_cycle_count;
}
int64_t gettick_nodelay(void)
{
	volatile int i = TIM2->CNT;

	return overflow_count + TIM2->CNT;
}
int64_t getus_nodelay(void)
{
	return gettick_nodelay() / us_cycle_count;
}

int delayms(int count)
{
	return delayus(count*1000);
}

void delaytick(int count)
{
	volatile uint16_t start; 
	volatile uint16_t target;
	
	if (count < overhead)
		return;
	
	start = TIM2->CNT;
	target = (start + count - overhead) % overflow;
	
	if (start <= target)
	{
		while(TIM2->CNT < target && TIM2->CNT >= start)
			;
	}
	else
	{
		while (TIM2->CNT > start || TIM2->CNT < target)
			;
	}
}
int delayus(int count)
{
	if (count < overflow_time_us)
	{
		delaytick(count*us_cycle_count - overhead2);
	}
	else
	{
		volatile int64_t target = gettick() + count*us_cycle_count;
		while(gettick()<target)	
			;
	}
	
	return 0;
}
