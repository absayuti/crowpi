/**********************************************************************
 Using 2x16 LCD display to print 'Hello'
 
 Hardware	:	CrowPi, Raspberry Pi 3B
 Software	:	Raspbian, WiringPi

 Compilation	:	gcc example.c -Wall -lwiringPi lwiringPiDev 
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <mcp23008.h>
#include <lcd.h>

// I2C address for character LCD on CrowPi
#define CHARLCD   	0x21

// Defines for the Adafruit Pi LCD interface board
#define AF_BASE     100				// MCP23008 GPIO base number
#define LCD_RS		(AF_BASE + 1)	// Register select
#define LCD_EN     	(AF_BASE + 2)	// Enable
#define LCD_D4     	(AF_BASE + 3)	// DB4
#define LCD_D5     	(AF_BASE + 4)	// DB5
#define LCD_D6     	(AF_BASE + 5)	// DB6
#define LCD_D7     	(AF_BASE + 6)	// DB7
#define LCD_LITE   	(AF_BASE + 7)	// Back light


//----- MAIN FUNCTION ----------------------------------------------------

int main( int argc, char **argv )
{
	int  lcd, count;
	char text[20];
	
	wiringPiSetup();
	mcp23008Setup( AF_BASE, CHARLCD );

  	printf ("Raspberry Pi - MCP23008 with Character LCD Test\n") ;
	lcd = lcdInit( 2,16,4, LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7, 0,0,0,0);
	// Switch on LCD back light
	pinMode( LCD_LITE, OUTPUT );
	digitalWrite( LCD_LITE, HIGH );

	lcdHome( lcd );
	lcdPuts( lcd, "Hello World!");
	delay(1000);
	lcdPosition( lcd, 0,1 );
	lcdPuts( lcd, "LCD is working");
	delay(2000);

	lcdClear( lcd );
	lcdPuts( lcd, "A counter 0-999");

	for( count = 0; count<1000; count++ ) {
		sprintf( text,"%d    ",count);
		lcdPosition( lcd, 7,1 );
		lcdPuts( lcd, text );
		delay(500);
	}

	return 0;
}


//------------------------------------------------------------------------

