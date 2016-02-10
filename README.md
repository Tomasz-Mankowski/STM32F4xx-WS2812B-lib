# STM32F4xx WS2812B library by Manik

## Description 

STM32F4xx-WS2812B-lib is a very simple to use library which handles WS2812B programmable LEDs using STM32F4xx.
Whole library is intend to a as simple as possible to use. It requires only one command for initialization
and includes only one command for particular LED settings. 

## How it works?

The library is **based on DMA** which transfers LED colour data **directly into ODR** register of the GPIO.
Half of the GPIO peripherals is used. Data is bursted via DMA on TIM1 Update trigger.
It is important for bit-rate period to be about 400 ms, as the WS2812B interface is time dependent. Data transfer is triggered by
TIM2 with frequency of 100Hz. This provides possibility to alter LED settings any time, even in the main program loop.
You can change refresh rate frequency, but bare in mind that it should depend on how many LEDs are there in the longest strip.
Minimum refresh period is 404ms\*72\*LED_COUNT_IN_SEGMENT+50us. Timers period and prescalers are calculated for 168MHz SystemCoreClock,
you should alter this values in an appropriate way.

## Usage

Provide longest strip length in *ws2812b_conf.h*
```
#define LED_COUNT_IN_SEGMENT 					200
```
here you cal also change an output port for you WS2812B strip. Initialize and use:
```
#include "ws2812b.h"

int main(void)
{
	WS2812B_Init();

	WS2812B_SetRGB(0, 0, 255, 0, 0);
	WS2812B_SetRGB(0, 1, 0, 255, 0);
	WS2812B_SetRGB(0, 2, 0, 0, 255);

	while(1)
	{
		
	}
}
```
*WS2812B_SetRGB* is defined as:
```
void WS2812B_SetRGB(uint8_t ChannelNumber, uint16_t LEDNumber, uint8_t R, uint8_t G, uint8_t B);
```
which is pretty much self-explanatory.

## Dependencies

- CMSIS DSP Library v1.4.4
- STM32F4xx_DSP_StdPeriph_Lib_V1.6.0

## License

MIT


