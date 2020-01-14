/******************************************************************************
	Program: 	Temp & Humidity
				This example program shows the use of DHT11 to measure 
				temperature and humidity.
				Output on character LCD display
				
	Hardware:	CrowPi (with Raspberry Pi 3B+)
	Software:	Raspbian system with WiringPi library
	Compile:	$ gcc -o temp -temphumidity.c -lwiringPi -lwiringPiDev -Wall
	Run with:	$ ./temp
	
	Jan 2020	
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <mcp23008.h>
#include <lcd.h>

// DHT11 parameters
#define MAXTIMINGS	85
#define DHTPIN		7

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
int read_dht11( int data[] );


//-----------------------------------------------------------------------------
// Main function

int main( void )
{
	int   dht11_dat[5], lcd;
	char  text[20];
	
	printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
	printf( "Output on LCD\n" );
	lcd = setup();
	lcdClear( lcd );
	lcdPuts( lcd, "Humidity  Temp.");

	while ( 1 )
	{
		if( read_dht11( dht11_dat ) ) {
			printf( "Humidity = %d.%d %% Temperature = %d.%d °C\n", \
					dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3]);
			sprintf(text, "%d.%d %%   %d.%d %cC", \
					dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], 0xDF);
			lcdPosition( lcd, 0,1 );
			lcdPuts( lcd, text );
		}			
		delay( 2000 ); // wait 1sec...
	}

	return(0);
}

//-----------------------------------------------------------------------------
// Function: Setup / initialization

int setup()
{
	int  lcd;

	wiringPiSetupPhys();
	mcp23008Setup( AF_BASE, CHARLCD );

	// Init LCD module
	lcd = lcdInit( 2,16,4, LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7, 0,0,0,0);
	pinMode( LCD_LITE, OUTPUT );
	digitalWrite( LCD_LITE, HIGH ); 			// Switch on LCD back light
	return lcd;
}


//-----------------------------------------------------------------------------
// Function: read DHT11
// 		Ref: https://github.com/nkundu/wiringpi-examples/blob/master/dht11.c					

int read_dht11( int data[] )
{
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j		= 0, i;

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	// pull pin down for 18 milliseconds
	pinMode( DHTPIN, OUTPUT );
	digitalWrite( DHTPIN, LOW );
	delay( 18 );
	// then pull it up for 40 microseconds
	digitalWrite( DHTPIN, HIGH );
	delayMicroseconds( 40 );
	// prepare to read the pin
	pinMode( DHTPIN, INPUT );

	// detect change and read data
	for ( i = 0; i < MAXTIMINGS; i++ )  {
		counter = 0;
		while ( digitalRead( DHTPIN ) == laststate ) {
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 )  break;
		}
		laststate = digitalRead( DHTPIN );

		if ( counter == 255 )  break;

		// ignore first 3 transitions
		if ( (i >= 4) && (i % 2 == 0) )  {
			/* shove each bit into the storage bytes */
			data[j / 8] <<= 1;
			if ( counter > 50 )
				data[j / 8] |= 1;
			j++;
		}
	}

	/*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */
	if ( (j >= 40) &&
	     (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) ) )
	{
		return 1;
	}else  {
		return 0;
		//printf( "Data not good, skip\n" );
	}
}
