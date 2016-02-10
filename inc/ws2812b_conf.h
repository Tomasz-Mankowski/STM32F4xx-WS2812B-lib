/*
 * ws2812b_conf.h
 *
 *  Created on: 10 lut 2016
 *      Author: Tomasz
 */

#ifndef WS2812B_CONF_H_
#define WS2812B_CONF_H_

#define LED_COUNT_IN_SEGMENT 					200

#define LED_Strip_Port							GPIOA
#define LED_Strip_AHB							RCC_AHB1Periph_GPIOA
#define LED_Strip_AHB_Cmd						RCC_AHB1PeriphClockCmd
#define LED_Seg_0_Pin							GPIO_Pin_0
#define LED_Seg_1_Pin							GPIO_Pin_1
#define LED_Seg_2_Pin							GPIO_Pin_2
#define LED_Seg_3_Pin							GPIO_Pin_3
#define LED_Seg_4_Pin							GPIO_Pin_4
#define LED_Seg_5_Pin							GPIO_Pin_5
#define LED_Seg_6_Pin							GPIO_Pin_6
#define LED_Seg_7_Pin							GPIO_Pin_7

#define TIM_Transmit_Burst_TIM					TIM1
#define TIM_Transmit_Burst_APB					RCC_APB2Periph_TIM1
#define TIM_Transmit_Burst_APB_Cmd				RCC_APB2PeriphClockCmd

#define DMA_Transmit_Burst_DMA					DMA2
#define DMA_Transmit_Burst_AHB					RCC_AHB1Periph_DMA2
#define DMA_Transmit_Burst_AHB_Cmd				RCC_AHB1PeriphClockCmd
#define DMA_Transmit_Burst_Str					DMA2_Stream5
#define DMA_Transmit_Burst_Chn					DMA_Channel_6
#define DMA_Transmit_Burst_IRQn					DMA2_Stream5_IRQn
#define DMA_Transmit_Burst_IRQHandler			DMA2_Stream5_IRQHandler
#define DMA_Transmit_Burst_TC_Flag				DMA_IT_TCIF5
#define DMA_Transmit_Burst_IRQ_Reg				HISR
#define DMA_Transmit_Burst_Clear_Flag_Reg		HIFCR

#define TIM_LED_Refresh_TIM						TIM2
#define TIM_LED_Refresh_APB						RCC_APB1Periph_TIM2
#define TIM_LED_Refresh_APB_Cmd					RCC_APB1PeriphClockCmd
#define TIM_LED_Refresh_IRQn					TIM2_IRQn
#define TIM_LED_Refresh_IRQHandler				TIM2_IRQHandler

#endif /* WS2812B_CONF_H_ */
