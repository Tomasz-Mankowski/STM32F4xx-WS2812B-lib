/*
 * ws2812b.h
 *
 *  Created on: 10 lut 2016
 *      Author: Tomasz
 */

#ifndef WS2812B_H_
#define WS2812B_H_

#include "stm32f4xx.h"

#include "ws2812b_conf.h"

#define LED_BUFFER_SIZE 		(LED_COUNT_IN_SEGMENT*24*3)

volatile uint8_t LED_OutputBuffer[LED_BUFFER_SIZE];

void WS2812B_Init(void);
void WS2812B_SetRGB(uint8_t ChannelNumber, uint16_t LEDNumber, uint8_t R, uint8_t G, uint8_t B);

void DMA_Transmit_Burst_IRQHandler(void);
void TIM_LED_Refresh_IRQHandler(void);


#endif /* WS2812B_H_ */
