/*
 * neopixel.c
 *
 *  Created on: Sep 18, 2024
 *      Author: wataoxp
 */

#include "main.h"
#include "neopixel.h"
#include <string.h>

void InitNeoPixel(NeoPixelTypedef *pixel,PatternTypedef *param,uint8_t *NeoPixelBuffer,uint16_t *HueArray)
{
	uint8_t pix;

	BuildHueData(HueArray, param);
	for(pix = 0; pix < NUM_LED; pix++)
	{
		HUEtoRGB(pixel, HueArray[pix], pix);
		SetLux(pixel, pix, param->Lux);
	}
	MakeDMABuffer(pixel, NeoPixelBuffer, NUM_LED);
}
void BuildHueData(uint16_t *HueArray,PatternTypedef *param)
{
	uint8_t i;
	uint16_t angle = (360 / NUM_LED) * param->Shift;

	for(i = 0; i < NUM_LED; i++)
	{
		HueArray[i] = i * angle;
		switch (param->Type)
		{
			case 0:
				HueArray[i] %= 360;
				break;
			case 1:
				HueArray[i] %= 90;
				break;
			case 2:
				HueArray[i] = HueArray[i] % 90 + 90;
				break;
			case 3:
				HueArray[i] = HueArray[i] % 90 + 180;
				break;
			default:
				break;
		}
	}
}
void HUEtoRGB(NeoPixelTypedef *pixel,uint32_t Hue,uint8_t num)
{
	if(Hue < 60)
	{
		pixel[num].bit24.r = 255;
		pixel[num].bit24.g = ((Hue * HUE_FIXEDPOINT) >> POINT_SHIFT);
		pixel[num].bit24.b = 0;
	}
	else if(Hue < 120)
	{
		Hue = 120 - Hue;
		pixel[num].bit24.r = ((Hue * HUE_FIXEDPOINT) >> POINT_SHIFT);
		pixel[num].bit24.g = 255;
		pixel[num].bit24.b = 0;
	}
	else if(Hue < 180)
	{
		Hue -= 120;
		pixel[num].bit24.r = 0;
		pixel[num].bit24.g = 255;
		pixel[num].bit24.b = ((Hue * HUE_FIXEDPOINT) >> POINT_SHIFT);
	}
	else if(Hue < 240)
	{
		Hue = 240 - Hue;
		pixel[num].bit24.r = 0;
		pixel[num].bit24.g = ((Hue * HUE_FIXEDPOINT) >> POINT_SHIFT);
		pixel[num].bit24.b = 255;
	}
	else if(Hue < 300)
	{
		Hue -= 240;
		pixel[num].bit24.r = ((Hue * HUE_FIXEDPOINT) >> POINT_SHIFT);
		pixel[num].bit24.g = 0;
		pixel[num].bit24.b = 255;
	}
	else if(Hue < 360)
	{
		Hue = 360 - Hue;
		pixel[num].bit24.r = 255;
		pixel[num].bit24.g = 0;
		pixel[num].bit24.b = ((Hue * HUE_FIXEDPOINT) >> POINT_SHIFT);
	}
}
void SetLux(NeoPixelTypedef *pixel,uint8_t num,uint8_t Lux)
{
	pixel[num].bit24.r >>= Lux;
	pixel[num].bit24.g >>= Lux;
	pixel[num].bit24.b >>= Lux;
}
void MakeDMABuffer(NeoPixelTypedef *pixel,uint8_t *SrcBuf,uint8_t size)
{
	uint8_t *ptr = SrcBuf;
	uint8_t i;
	uint32_t mask;

    for(i = 0;i < size; i++)
    {
    	for(mask = 1 << 23; mask; mask >>= 1)
    	{
    		if(pixel[i].fullcolor & mask)
			{
    			*ptr++ = NeoPixel_HIGH;
			}
			else
			{
				*ptr++ = NeoPixel_LOW;
			}
    	}
    }
}
//ShiftPixel->24
void ShiftPixel(uint8_t *srcBuf)
{
	uint8_t tmpBuf[24];

	memcpy(tmpBuf,srcBuf,24);
	memmove(srcBuf,srcBuf+24,(NUM_LED - 1)*24);
	memcpy(srcBuf+(NUM_LED - 1)*24,tmpBuf,24);

}
