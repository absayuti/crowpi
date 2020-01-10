/**********************************************************************
 	Program:	Light sensor
				This example program shows the use of the light sensor
				on CrowPi. The light sensor is BH1750, connected the 
				I2C bus of the Raspberry Pi.

	Hardware:	CrowPi (with Raspberry Pi 3B+)
				CrowPi's built-in light sensor
				Monitor
				Source of light e.g. a torch light

	Software:	Raspbian system with WiringPi library
				
	Others:		Compile with:
				$ gcc -o example example_lightsensor.c -lwiringPi -Wall
		
				Run with:
				$ ./example
						
	References: Various sources, including:
				http://raspberrypi.link-tech.de/doku.php?id=bh1750
				https://components101.com/sites/default/files/component_datasheet/BH1750.pdf

	Jan 2020

***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "bh1750.h"


int main()
{
	int    lightsensor, lux;

	wiringPiSetupPhys();			// Setup WiringPi & pin numbering scheme 
	lightsensor = init_bh1750();	//initialize I2C/light sensor

	// Instruction
	printf("Cover or shine light on the motion sensor\n");
	printf("Press CTRL-C to end\n");
	delay(1000);

	while(1) {
		lux = read_bh1750( lightsensor );
		printf("Light = %d lux\n", lux);
		delay(1000);
	}

	return 0;
}
