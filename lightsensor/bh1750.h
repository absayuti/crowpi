/**********************************************************************
    bh1750.h
	Include file for BH1750 light sensor module connected via I2C

***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

// BH1750 parameters

#define BH1750_DEVICE 		    	0x5C 	// Light sensor devID on I2C bus
											// Use $ gpio i2detect to get devID
#define BH1750_POWER_DOWN           0x00	// No active state
#define BH1750_POWER_ON             0x01	// Waiting for measurement command
#define BH1750_RESET                0x07	// Reset data register value
											// - not accepted in POWER_DOWN mode
#define BH1750_DEFAULT_MTREG          69	// Default MTreg value
#define UNCONFIGURED                   0
#define CONTINUOUS_HIGH_RES_MODE    0x10	// Measurement at 1 lux resolution. 
											// Measurement time is approx 120ms.
#define CONTINUOUS_HIGH_RES_MODE_2  0x11	// Measurement at 0.5 lux resolution. 
											// Measurement time is approx 120ms.
#define CONTINUOUS_LOW_RES_MODE     0x13	// Measurement at 4 lux resolution. 
											// Measurement time is approx 16ms.
#define ONE_TIME_HIGH_RES_MODE	    0x20	// Measurement at 1 lux resolution. 
											// Measurement time is approx 120ms.
#define ONE_TIME_HIGH_RES_MODE_2    0x21	// Measurement at 0.5 lux resolution. 
											// Measurement time is approx 120ms.
#define ONE_TIME_LOW_RES_MODE       0x23	// Measurement at 4 lux resolution. 
											// Measurement time is approx 16ms.
#define LUX_CALIBRATION             0.3		// Lux value is affected by this constant
											// BH1750 datasheet uses 1/1.2 = 0.83333333
											// 0.3 seems to give a better lux value

// Function: Initialize BH1750 and return file number fd

int init_bh1750()
{
	int    fd;

	// Setup pin modes and initialize I2C/light sensor
	fd = wiringPiI2CSetup( BH1750_DEVICE );
	if( fd < 0 ) {
		printf("Unable to initialize I2C / Light sensor.\n");
		printf("Return code: %d\n", errno);
		exit(1);
	}
	delay(100);
	wiringPiI2CWrite( fd, CONTINUOUS_HIGH_RES_MODE );

	return fd;
}

// Function: Read data from light sensor, compute and return lux value

int read_bh1750( int fd )
{
	int data, val, lux;

	data = wiringPiI2CReadReg16( fd, 'L' );		// ADDR = 'L' according to BH1750 datasheet
												// Data received in Little-Endian format
	val = ((data & 0xFF00)>>8) | ((data & 0x00FF)<<8);	
	lux = (int)(val*LUX_CALIBRATION);
	
	return lux;
}
