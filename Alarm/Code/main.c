#include "StdTypes.h"
#include "MemMap.h"
#include "Utils.h"

#include "DIO.h"

#include "Stop_Watch.h"

#define  F_CPU	8000000
#include <util/delay.h>

int main(void)
{
	DIO_Init();
	LCD_Init();
	segmentDisplay_Init();
	

    while(1) 
    {
		segmentDisplay_Running();
	}
}
