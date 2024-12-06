/*
 * neopixel.h
 *
 *  Created on: Sep 18, 2024
 *      Author: wataoxp
 */

#ifndef INC_NEOPIXEL_H_
#define INC_NEOPIXEL_H_
//LED数
#define NUM_LED 60
//SPI送信バッファ用
#define RESET_PULSE 30	//1.25us*30 = 30us
#define NEOPIXEL_BUFFER_SIZE (NUM_LED * 24 + RESET_PULSE)

//NeoPixInit用
#define HUE_SHIFT 4

//SPI HIビット数
#define NeoPixel_HIGH 0xF0
#define NeoPixel_LOW 0x80

//HSV変換用固定小数
/*
 * Hueが1変化するごとにRGBは255/60ずつ変化する。
 * (255/60)*2^4=68
 * 2^4は(255/60)との乗算の結果が「整数」になる最小の数値。
 * 
 * 上記の式に演算したい乗数を掛けた後に、16で割る(右4シフト)
 * 今回の場合は(68*Hue)>>4によって小数と整数の疑似的な乗算ができる。
 */
#define HUE_FIXEDPOINT 68
#define POINT_SHIFT 4

//STMはリトルエンディアン。なのでメモリ上では以下の順番でgrbの順となる
typedef struct{
	uint8_t b;
	uint8_t r;
	uint8_t g;
}color;
typedef union{
	uint32_t fullcolor;
	color bit24;
}NeoPixelTypedef;

typedef struct{
	int8_t Lux;
	uint8_t Type;
	uint8_t Shift;
	uint8_t Push;
}PatternTypedef;


void InitNeoPixel(NeoPixelTypedef *pixel,PatternTypedef *param,uint8_t *NeoPixelBuffer,uint16_t *HueArray);
void BuildHueData(uint16_t *HueArray,PatternTypedef *param);
void HUEtoRGB(NeoPixelTypedef *pixel,uint32_t Hue,uint8_t num);
void SetLux(NeoPixelTypedef *pixel,uint8_t num,uint8_t Lux);
void MakeDMABuffer(NeoPixelTypedef *pixel,uint8_t *SrcBuf,uint8_t size);
void ShiftPixel(uint8_t *srcBuf);



#endif /* INC_NEOPIXEL_H_ */
