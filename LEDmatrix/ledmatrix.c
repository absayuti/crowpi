/*****************************************************************************
	Program:	LED matrix
				Example program using MAX7219 LED matrix display on CrowPi.
				It displays several scrolling messages.

	Hardware:	CrowPi (with Raspberry Pi 3B+) 
				Including LED matrix display (MAX7219 via SPI bus) 
	Software:	Raspbian system with WiringPi library
				max7219matrix library
					https://github.com/absayuti/crowpi/max7219matrix.h
	Compile:	$ gcc -o ledmatrix ledmatrix.c -lwiringPi -Wall
	Run with:	$ ./ledmatrix

	Jan 2020
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "max7219matrix.h"

// Function prototypes
void setup();
void display_messages();
void display_patterns();


//-----------------------------------------------------------------------------
// Main function

int main()
{
	setup();

	display_messages();
	display_patterns();
	delay(2000);
	MAX7219write( SHUTDOWN, 0);				// Switch OFF LED matrix

	return 0;
}


//-----------------------------------------------------------------------------
// Function: Setup

void setup()
{
	int  row;
	
	init_ledmatrix();
	for( row = 1; row < 9; row++ ) {   		// OFF all LEDs
		MAX7219write( row, 0);
	}
	initBuffer();							// Clear LED matrix buffer
}


//-----------------------------------------------------------------------------
// Function: Display messages

void display_messages()
{
	displayMessage("Hello World! ", 150);
	displayMessage("This is an example of using LED matrix.", 100);
	delay(1000);
}

//-----------------------------------------------------------------------------
// Function: LEDmatrix pattern

void display_patterns()
{
	int  c, r;
	
	// Row top down
	c = 255;
	for( r = 1; r < 9; r++ ) {
		MAX7219write( r, c );
		delay(100);
		MAX7219write( r, 0 );
	}
	delay(1000);
	
	// Row bottom up
	for( r = 8; r > 0; r-- ) {
		MAX7219write( r, c );
		delay(100);
		MAX7219write( r, 0 );
	}
	delay(1000);

	// Column right to left
	for( c = 1; c < 9; c++ ) {
		for( r = 1; r < 9; r++ ) {
			MAX7219write( r, 1<<(c-1) );
		}
		delay(100);
	}
	delay(1000);

	// Column left to right
	for( c = 8; c > 0; c-- ) {
		for( r = 1; r < 9; r++ ) {
			MAX7219write( r, 1<<(c-1) );
		}
		delay(100);
	}
	delay(1000);
}
