#include "lcd.h"

/*
 * lcd.c
 *
 *  Created on: 26 apr. 2018
 *      Author: linus
 */

void TextLCD_Strobe(TextLCDType *lcd);
void TextLCD_Cmd(TextLCDType *lcd, uint8_t cmd);
void TextLCD_Data(TextLCDType *lcd, uint8_t data);

void TextLCD_Printf (TextLCDType *lcd, char *message, ...){}

void TextLCD_Strobe(TextLCDType *lcd){

	HAL_GPIO_WritePin(lcd->controlPort, lcd->strbPin, GPIO_PIN_SET);
	delay(500);
	HAL_GPIO_WritePin(lcd->controlPort, lcd->strbPin, GPIO_PIN_RESET);
	delay(500);
}

void TextLCD_Data(TextLCDType *lcd, uint8_t data){

	HAL_GPIO_WritePin(lcd->controlPort, lcd->rsPin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->controlPort, lcd->rwPin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(lcd->dataPort, lcd->dataPins, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->dataPort, data, GPIO_PIN_SET);

	TextLCD_Strobe(lcd);
	delay(40);

}

void TextLCD_Cmd(TextLCDType *lcd, uint8_t cmd){

	HAL_GPIO_WritePin(lcd->controlPort, lcd->rsPin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->controlPort, lcd->rwPin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(lcd->dataPort, lcd->dataPins, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->dataPort, cmd , GPIO_PIN_SET);

	TextLCD_Strobe(lcd);
	delay(40);
}

void TextLCD_Init(TextLCDType *lcd, GPIO_TypeDef *controlPort, uint16_t rsPin, uint16_t rwPin,uint16_t enPin, GPIO_TypeDef *dataPort, uint16_t dataPins){

	lcd->controlPort = controlPort;
	lcd->dataPort = dataPort;
	lcd->rsPin = rsPin;
	lcd->rwPin = rwPin;
	lcd->dataPins = dataPins;
	lcd->strbPin = enPin;

	HAL_Delay(30);
	TextLCD_Cmd(lcd, 0x38);
	HAL_Delay(8);
	TextLCD_Cmd(lcd, 0x38);
	delay(200);
	TextLCD_Cmd(lcd, 0x38);
	delay(80);
	TextLCD_Cmd(lcd, 0x38);
	delay(80);
	TextLCD_Cmd(lcd, 0x06);
	delay(80);
	TextLCD_Cmd(lcd, 0x0E);
	delay(80);
	TextLCD_Cmd(lcd, 0x01);
	HAL_Delay(3);
	TextLCD_Cmd(lcd, 0x80);
	delay(80);

}

void TextLCD_Home (TextLCDType *lcd){

	HAL_GPIO_WritePin(lcd->controlPort, lcd->rsPin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->controlPort, lcd->rwPin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(lcd->dataPort, lcd->dataPins, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->dataPort, LCD_D1_Pin , GPIO_PIN_SET);

	TextLCD_Strobe(lcd);
	delay(40);

}

void TextLCD_Clear (TextLCDType *lcd){

	HAL_GPIO_WritePin(lcd->controlPort, lcd->rsPin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->controlPort, lcd->rwPin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(lcd->dataPort, lcd->dataPins, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->dataPort, LCD_D0_Pin , GPIO_PIN_SET);

	TextLCD_Strobe(lcd);
	delay(40);
}

void TextLCD_Position (TextLCDType *lcd, int x, int y){

	uint8_t data;
	data = (x-1)&0x0F;
	if(y==1)
	{
		data |= (0x80);
		TextLCD_Cmd(lcd, data);
	}
	else
	{
		data |= (0xC0);
		TextLCD_Cmd(lcd, data);
	}

}

void TextLCD_Putchar (TextLCDType *lcd, uint8_t data){

	TextLCD_Data(lcd,data);
}

void TextLCD_Puts (TextLCDType *lcd, char *string){

	int i = 0;
	while (string[i] != '\0'){
		TextLCD_Putchar(lcd, string[i]);
		i++;
	}

}

void delay(uint32_t delay_us){

	HAL_TIM_Base_Start_IT(&htim11);
	uint32_t current = usekTick;
		while((usekTick-current)<(delay_us-2));

	HAL_TIM_Base_Stop_IT(&htim11);
}

void TextLCD_PrintI(TextLCDType *lcd, uint32_t number){

	if(number < 0){
		TextLCD_Putchar(lcd, '-');
		number = number *-1;
	}
	if(number > 9){
		TextLCD_PrintI(lcd, number / 10);
	}
	TextLCD_Putchar(lcd, number % 10 + 0x30);
}

