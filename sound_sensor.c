/******************************************************************************
	Program: 	Sound sensor
				This example program shows the use of sound sensor to detect
				hand clap or finger snap.
				
	Hardware:	CrowPi (with Raspberry Pi 3B+)
	Software:	Raspbian system with WiringPi library
	Compile:	$ gcc -o sound sound_sensor.c -lwiringPi -Wall
	Run with:	$ ./sound
						
	Jan 2020	
******************************************************************************/

#include <stdio.h>
#include <wiringPi.h>

#define  SOUNDSENSOR    18
#define  LED22          22


int main()
{
	// Setup
	wiringPiSetupPhys();
	pinMode(SOUNDSENSOR, INPUT);
	pullUpDnControl(SOUNDSENSOR, PUD_UP);
	pinMode(LED22, OUTPUT);

	// Instruction
	printf("Clap your hands near the sound sensor\n");
	printf("Press CTRL-C to end\n");
	
	// Main loop
	while(1) {
		if( digitalRead(SOUNDSENSOR) ) {
			printf("Sound detected\n");
			digitalWrite(LED22, LOW);
			delay(500);	
		}
		else {
			digitalWrite(LED22, HIGH);
		}
	}
	
	return 0;
}
