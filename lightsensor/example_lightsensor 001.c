/**********************************************************************
 Detecting light using light sensor via I2C bus 
 
 Hardware	:	CrowPi, Raspberry Pi 3B
				Input switches: ???
				Output switches: ???
 Software	:	Raspbian, WiringPi

 To compile	:	gcc -WaLL thisfile.c -lwiringPi	
 					      ^^^^^^^^^^

 References	: 	Various sources, including:
				http://raspberrypi.link-tech.de/doku.php?id=bh1750
				https://components101.com/sites/default/files/component_datasheet/BH1750.pdf

***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>


// ******************* BH1750 parameters start here ***************

#define BH1750_DEVICE 				0x5C 	// Light sensor devID on I2C bus
											// Use $ gpio i2detect to get devID
#define BH1750_POWER_DOWN           0x00	// No active state
#define BH1750_POWER_ON             0x01	// Waiting for measurement command
#define BH1750_RESET                0x07	// Reset data register value - not accepted in POWER_DOWN mode
#define BH1750_DEFAULT_MTREG          69	// Default MTreg value
#define UNCONFIGURED                   0
#define CONTINUOUS_HIGH_RES_MODE    0x10	// Measurement at 1 lux resolution. Measurement time is approx 120ms.
#define CONTINUOUS_HIGH_RES_MODE_2  0x11	// Measurement at 0.5 lux resolution. Measurement time is approx 120ms.
#define CONTINUOUS_LOW_RES_MODE     0x13	// Measurement at 4 lux resolution. Measurement time is approx 16ms.
#define ONE_TIME_HIGH_RES_MODE	    0x20	// Measurement at 1 lux resolution. Measurement time is approx 120ms.
#define ONE_TIME_HIGH_RES_MODE_2    0x21	// Measurement at 0.5 lux resolution. Measurement time is approx 120ms.
#define ONE_TIME_LOW_RES_MODE       0x23	// Measurement at 4 lux resolution. Measurement time is approx 16ms.


int main()
{
	int    fd, data, lux;
	double val;
	//int  lightsensor = BH1750_DEVICE;

	// Setup pin modes and initialize I2C/light sensor
	wiringPiSetupPhys();
	fd = wiringPiI2CSetup( BH1750_DEVICE );
	if( fd < 0 ) {
		printf("Unable to initialize I2C / Light sensor.\n");
		printf("Return code: %d\n", errno);
		exit(1);
	}
	delay(100);
	wiringPiI2CWrite( fd, CONTINUOUS_HIGH_RES_MODE );

	// Instruction
	printf("Cover or shine light on the motion sensor\n");
	printf("Press CTRL-C to end\n");
	delay(1000);

	while(1) {
		data = wiringPiI2CReadReg16( fd, 'L' );				// ADDR = 'L' according to BH1750 datasheet
		val = ((data & 0xFF00)>>8) | ((data & 0x00FF)<<8);	// Data received in Little-Endian format
		lux = (int)(val/3);									// This seems to give more accurate lux value
															// BH1750 datasheet uses: val/1.2
		printf("Light = %d lux\n", lux);
		delay(1000);
	}

	return 0;
}
