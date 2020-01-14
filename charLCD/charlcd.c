/******************************************************************************
	Program: 	Character LCD
				This example program shows the use of 16x2 character LCD to
				display data.
				
	Hardware:	CrowPi (with Raspberry Pi 3B+)
	Software:	Raspbian system with WiringPi library
	Compile:	$ gcc -o lcd charlcd.c -lwiringPi lwiringPiDev -Wall
	Run with:	$ ./lcd
						
	Jan 2020	
******************************************************************************/

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


// Function prototypes
int  setup();
void hello_world( int lcd );
void counter( int lcd );


//-----------------------------------------------------------------------------
// Main function

int main( int argc, char **argv )
{	
	int  lcd;

 	printf ("Raspberry Pi - MCP23008 with Character LCD Test\n") ;
	lcd = setup();
	
	while(1) {
		hello_world( lcd );
		counter( lcd );
	}
	
	return 0;
}


//-----------------------------------------------------------------------------
// Function: Setup / initialization

int setup()
{
	int  lcd;

	wiringPiSetup();
	mcp23008Setup( AF_BASE, CHARLCD );

	// Init LCD module
	lcd = lcdInit( 2,16,4, LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7, 0,0,0,0);
	pinMode( LCD_LITE, OUTPUT );
	digitalWrite( LCD_LITE, HIGH ); 			// Switch on LCD back light
	return lcd;
}


//-----------------------------------------------------------------------------
// Function: Hello world -- a simple LCD test

void hello_world( int lcd )
{
	lcdClear( lcd );
	lcdPuts( lcd, "Hello World!");
	delay(1000);
	lcdPosition( lcd, 0,1 );
	lcdPuts( lcd, "LCD is working");
	delay(2000);
}


//-----------------------------------------------------------------------------
// Function: Counter -- counting & displaying numbers from 0 to 99 

void counter( int lcd )
{
	int  count;
	char text[20];

	lcdClear( lcd );
	lcdPuts( lcd, "A counter 0-99");

	for( count = 0; count<100; count++ ) {
		sprintf( text,"%d    ",count);
		lcdPosition( lcd, 7,1 );
		lcdPuts( lcd, text );
		delay(500);
	}
}
