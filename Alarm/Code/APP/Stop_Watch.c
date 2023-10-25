#include "StdTypes.h"
#include "MemMap.h"
#include "Utils.h"
#include "DIO.h"
#include "Stop_Watch_CFG.h"
#include "Stop_Watch.h"

#define  F_CPU	8000000
#include <util/delay.h>
u8 digit_0, digit_1, data , copyData, MODE_tick;
u8 const segmentArray[10]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

void segmentDisplay_Init(void)
{
	DIO_WritePin(ENABLE_DIGIT0,HIGH);
	DIO_WritePin(ENABLE_DIGIT1,HIGH);

	data = START_COUNTDOWN;
	copyData = data;
	MODE_tick =0;
}
void segmentDisplay(u8 number)
{
	
	digit_0 = number%10 , digit_1 = number/10;
	
	DIO_WritePin(ENABLE_DIGIT0,HIGH);
	DIO_WritePort(PA,(segmentArray[digit_0]<<1));
	DIO_WritePin(ENABLE_DIGIT1,LOW);
	_delay_ms(1);
	
	
	DIO_WritePin(ENABLE_DIGIT1,HIGH);
	DIO_WritePort(PA,(segmentArray[digit_1]<<1));

	DIO_WritePin(ENABLE_DIGIT0,LOW);
	_delay_ms(1);
}

void segmentDisplay_Running(void)
{
	while(DIO_ReadPin(MODE)== LOW)
	{
		if(MODE_tick==0)
		{
			if(DIO_ReadPin(RESET)==LOW)
			{
				data++;
				_delay_ms(500);
			}
			if(DIO_ReadPin(PAUSE_RESUME)==LOW)
			{
				data--;
				_delay_ms(500);
			}
			copyData = data;
			MODE_tick++;
		}
		else if(MODE_tick == 1)
		{
			data = copyData;
			segmentDisplay(copyData);
			_delay_ms(10);
			MODE_tick=0;
		}
	}
	data--;
	for(u8 index=0; index<200; index++)
	{
		segmentDisplay(data);
	}
	if(DIO_ReadPin(RESET)==LOW)
	{
		data = copyData;
		_delay_ms(100);
	}
	if(DIO_ReadPin(PAUSE_RESUME)==LOW)
	{
		while(DIO_ReadPin(PAUSE_RESUME)==LOW);
		while(DIO_ReadPin(PAUSE_RESUME)==HIGH);
	}
	if(data==0)
	{
		DIO_WritePin(BUZZER,HIGH);
		while(DIO_ReadPin(STOP_BUZZER)==HIGH)
		{
			segmentDisplay(data);
		}
		data = copyData;
	}
	DIO_WritePin(BUZZER,LOW);
}