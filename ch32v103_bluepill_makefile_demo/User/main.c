/* USART demo and blink a led */
/* Wire: A8->R->LED->GND */

#include "debug.h"
/* Global typedef */

/* Global define */

/* Global Variable */

void InitGPIO()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	{
		GPIO_InitTypeDef  GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	}
}

int main(void)
{
    u8 i = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_Init();
	USART_Printf_Init(115200);
	InitGPIO();
	printf("SystemClk:%lu\r\n", SystemCoreClock);
	printf("This is printf example\r\n");

    while(1)
    {
        Delay_Ms(500);
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
    }
}

