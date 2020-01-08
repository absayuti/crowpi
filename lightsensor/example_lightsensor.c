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
#include "bh1750.h"


int main()
{
	int    lux;
	double val;

	// Setup pin modes and initialize I2C/light sensor
	wiringPiSetupPhys();
	fd = init_bh1750();

	// Instruction
	printf("Cover or shine light on the motion sensor\n");
	printf("Press CTRL-C to end\n");
	delay(1000);

	while(1) {
		lux = read_bh1750();
		printf("Light = %d lux\n", lux);
		delay(1000);
	}

	return 0;
}
