/*
 * ws2812b.c
 *
 *  Created on: 10 lut 2016
 *      Author: Tomasz
 */

#include "ws2812b.h"

static void WS2812B_GPIO_Led_Seg(void);
static void WS2812B_DMA_Transmit_Burst(void);
static void WS2812B_TIM_Transmit_Burst(void);
static void WS2812B_TIM_LED_Refresh(void);

static GPIO_InitTypeDef GPIO_InitStructure;
static DMA_InitTypeDef DMA_InitStructure;
static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
static NVIC_InitTypeDef NVIC_InitStructure;

void WS2812B_Init(void)
{
	uint16_t i;
	for(i=0; i < LED_COUNT_IN_SEGMENT; i++)
	{
		uint8_t j;
		for(j=0; j < 24; j++)
		{
			LED_OutputBuffer[i*24*3+j*3] = 0xFF;
			LED_OutputBuffer[i*24*3+j*3+1] = 0x00;
			LED_OutputBuffer[i*24*3+j*3+2] = 0x00;
		}
	}

	WS2812B_GPIO_Led_Seg();
	WS2812B_DMA_Transmit_Burst();
	WS2812B_TIM_Transmit_Burst();
	WS2812B_TIM_LED_Refresh();
}

void WS2812B_SetRGB(uint8_t ChannelNumber, uint16_t LEDNumber, uint8_t R, uint8_t G, uint8_t B)
{
	uint8_t* LED_OutputBuffer_Pointer = &LED_OutputBuffer[LEDNumber * 24 *3];

	uint8_t i;
	for(i=0; i<8; i++)
	{
		*(LED_OutputBuffer_Pointer +          i * 3 + 1) ^= ((*(LED_OutputBuffer_Pointer +          i * 3 + 1) >> ChannelNumber) & (1 << 0)) != ((G >> i) & (1 << 0))?(1 << ChannelNumber):0x00;
		*(LED_OutputBuffer_Pointer + 8  * 3 + i * 3 + 1) ^= ((*(LED_OutputBuffer_Pointer + 8  * 3 + i * 3 + 1) >> ChannelNumber) & (1 << 0)) != ((R >> i) & (1 << 0))?(1 << ChannelNumber):0x00;
		*(LED_OutputBuffer_Pointer + 16 * 3 + i * 3 + 1) ^= ((*(LED_OutputBuffer_Pointer + 16 * 3 + i * 3 + 1) >> ChannelNumber) & (1 << 0)) != ((B >> i) & (1 << 0))?(1 << ChannelNumber):0x00;
	}
}

static void WS2812B_GPIO_Led_Seg(void)
{
	LED_Strip_AHB_Cmd(LED_Strip_AHB, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Pin =  LED_Seg_0_Pin | LED_Seg_1_Pin | LED_Seg_2_Pin | LED_Seg_3_Pin | LED_Seg_4_Pin | LED_Seg_5_Pin | LED_Seg_6_Pin | LED_Seg_7_Pin;
	GPIO_Init(LED_Strip_Port, &GPIO_InitStructure);

	LED_Strip_Port->BSRRH =  LED_Seg_0_Pin | LED_Seg_1_Pin | LED_Seg_2_Pin | LED_Seg_3_Pin | LED_Seg_4_Pin | LED_Seg_5_Pin | LED_Seg_6_Pin | LED_Seg_7_Pin;
}

static void WS2812B_DMA_Transmit_Burst(void)
{
	DMA_Cmd(DMA_Transmit_Burst_Str, DISABLE);
	DMA_DeInit(DMA_Transmit_Burst_Str);

	DMA_Transmit_Burst_AHB_Cmd(DMA_Transmit_Burst_AHB, ENABLE);

	DMA_InitStructure.DMA_Channel = DMA_Transmit_Burst_Chn;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = LED_BUFFER_SIZE;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(LED_Strip_Port->ODR);
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&LED_OutputBuffer;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;

	DMA_Init(DMA_Transmit_Burst_Str, &DMA_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA_Transmit_Burst_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA_Transmit_Burst_Str, DMA_IT_TC, ENABLE);

	DMA_Cmd(DMA_Transmit_Burst_Str, ENABLE);
}

static void WS2812B_TIM_Transmit_Burst(void)
{
	TIM_Transmit_Burst_APB_Cmd(TIM_Transmit_Burst_APB, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 67; // (67+1) / (168 MHz) = 0.4048 µs  (microseconds)
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM_Transmit_Burst_TIM, &TIM_TimeBaseStructure);

	TIM_ARRPreloadConfig(TIM_Transmit_Burst_TIM, ENABLE);

	TIM_DMACmd(TIM_Transmit_Burst_TIM, TIM_DMA_Update, ENABLE);

	TIM_Cmd(TIM_Transmit_Burst_TIM, ENABLE);
}

static void WS2812B_TIM_LED_Refresh(void)
{
	TIM_LED_Refresh_APB_Cmd(TIM_LED_Refresh_APB, ENABLE);

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_Prescaler = 3359; // (84MHz) / 25kHz - 1 = 3359
	TIM_TimeBaseStructure.TIM_Period = 249; // (249+1) / 25 kHz = 10 ms  (refresh 100Hz)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM_LED_Refresh_TIM, &TIM_TimeBaseStructure);

	TIM_ARRPreloadConfig(TIM_LED_Refresh_TIM, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM_LED_Refresh_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	TIM_LED_Refresh_TIM->SR = (uint16_t)~TIM_IT_Update;//reset pending bit

	TIM_ITConfig(TIM_LED_Refresh_TIM, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM_LED_Refresh_TIM, ENABLE);
}

void DMA_Transmit_Burst_IRQHandler(void)
{
	if((((DMA_Transmit_Burst_DMA->DMA_Transmit_Burst_IRQ_Reg & (uint32_t)0x0F7D0F7D) & DMA_Transmit_Burst_TC_Flag) != (uint32_t)RESET) && ((DMA_Transmit_Burst_Str->CR & DMA_IT_TC) != (uint32_t)RESET)) //is interrupt && is enabled interrupt
	{
		DMA_Transmit_Burst_Str->CR &= (uint32_t)~DMA_SxCR_EN;//disable DMA
		TIM_Transmit_Burst_TIM->CR1 &= (uint16_t)~TIM_CR1_CEN;//disable TIM

		DMA_Transmit_Burst_Str->M0AR = (uint32_t)&LED_OutputBuffer;
		DMA_Transmit_Burst_Str->NDTR = (uint16_t)LED_BUFFER_SIZE;

		DMA_Transmit_Burst_DMA->DMA_Transmit_Burst_Clear_Flag_Reg = (uint32_t)(DMA_Transmit_Burst_TC_Flag & (uint32_t)0x0F7D0F7D);//reset pending bit
	}
}

void TIM_LED_Refresh_IRQHandler(void)
{
	if(((TIM_LED_Refresh_TIM->SR & TIM_IT_Update) != (uint16_t)RESET) && ((TIM_LED_Refresh_TIM->DIER & TIM_IT_Update) != (uint16_t)RESET))//is interrupt flag && is interrupt enabled
	{
		TIM_LED_Refresh_TIM->SR = (uint16_t)~TIM_IT_Update;//reset pending bit

		DMA_Transmit_Burst_Str->CR |= (uint32_t)DMA_SxCR_EN;//enable DMA
		TIM_Transmit_Burst_TIM->CR1 |= TIM_CR1_CEN;//enable TIM

	}
}


