/******************************************************************************
	Program: 	Temp & Humidity
				This example program shows the use of DHT11 to measure 
				temperature and humidity. 
				
	Hardware:	CrowPi (with Raspberry Pi 3B+)
	Software:	Raspbian system with WiringPi library
	Compile:	$ gcc -o temp -temphumidity.c -lwiringPi -Wall
	Run with:	$ ./temp
	
	Jan 2020	
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>

#define MAXTIMINGS	85
#define DHTPIN		7

// Function prototypes
int read_dht11( int data[] );


//-----------------------------------------------------------------------------
// Main function

int main( void )
{
	int dht11_dat[5];
	
	printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );

	if ( wiringPiSetupPhys() == -1 )  exit( 1 );

	while ( 1 )
	{
		if( read_dht11( dht11_dat ) )
			printf( "Humidity = %d.%d %% Temperature = %d.%d *C\n", \
					dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3]);
		delay( 2000 ); // wait 1sec...
	}

	return(0);
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
