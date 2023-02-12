/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2022/01/25
 * Description        : Ä£Äâ¼æÈÝHIDÉè±¸
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "CH58x_common.h"

void board_led_init(void)
{
    GPIOA_SetBits(GPIO_Pin_8);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeOut_PP_5mA);
}

void board_led_toggle(void)
{
    GPIOA_InverseBits(GPIO_Pin_8);
}

void board_led_set(uint8_t set)
{
    if (set)
        GPIOA_ResetBits(GPIO_Pin_8);
    else
        GPIOA_SetBits(GPIO_Pin_8);
}

int main()
{
    uint8_t s;
    SetSysClock(CLK_SOURCE_PLL_60MHz);

    board_led_init();

    while(1)
    {
      mDelaymS(100);
      board_led_toggle(); 
      mDelaymS(100);
    }
}

