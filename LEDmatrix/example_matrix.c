#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPiSPI.h>
#include "max7219matrix.h"


void init_ledmatrix();


int main()
{
	int  row, data;

	init_ledmatrix();

	// OFF all LEDs
	for( row = 1; row < 9; row++ ) {
		MAX7219write( row, 0);
	}

	initBuffer();
	displayMessage("Hello World!!!! ", 150);
	displayMessage("Super scrolling message board I wrote myself. ", 100);
	displayMessage("Well, almost.", 150);

	sleep(2);
	
	for( row = 1; row < 9; row++ ) {
		for( data = 1; data <= 0x80; data <<= 1 ) {
			MAX7219write( row, data);
			usleep(100000);
		}
	}
	
	sleep(2);
	
	// Switch OFF LED matrix
	MAX7219write( SHUTDOWN, 0);
	
	return 0;
}

/*
void init_ledmatrix()
{
	int  rc;

	rc = wiringPiSPISetup( CHANNEL, 500000 );
	if( rc<0 ) {
		printf("Error initializing SPI, rc = %d\n", rc);
		exit(rc);
	}

	MAX7219write( SCAN_LIMIT, 7);
	MAX7219write( DECODE_MODE, 0);
	MAX7219write( DISPLAY_TEST, 0);
	MAX7219write( INTENSITY, 1);
	MAX7219write( SHUTDOWN, 1);
}
*/
