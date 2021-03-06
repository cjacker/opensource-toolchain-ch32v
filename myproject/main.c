/* USART demo and blink a led */
/* Wire: A8->R->LED->GND */

#include "debug.h"
/* Global typedef */

/* Global define */

/* Global Variable */

void InitGPIO()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	{
		GPIO_InitTypeDef  GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_Init();
	USART_Printf_Init(115200);
	InitGPIO();
	printf("SystemClk:%d\r\n", SystemCoreClock);
	printf("This is printf example\r\n");

	while (1)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
		Delay_Us(500*1000);
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);
		Delay_Us(500*1000);
	}
}

