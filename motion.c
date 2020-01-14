/******************************************************************************
	Program: 	Motion detector
				This example program shows the use of motion sensor to detect
				movement/motion
				
	Hardware:	CrowPi (with Raspberry Pi 3B+)
	Software:	Raspbian system with WiringPi library
	Compile:	$ gcc -o motion motion.c -lwiringPi -Wall
	Run with:	$ ./motion
						
	Jan 2020	
******************************************************************************/

#include <stdio.h>
#include <wiringPi.h>

#define  MOTIONSENSOR   16


int main()
{
	// Setup
	wiringPiSetupPhys();	
	pinMode(MOTIONSENSOR, INPUT);

	// Instruction
	printf("Wave your hand above the motion sensor.\n");
	printf("Press Ctrl-C to END.\n");
	
	// Main loop
	while(1) {
		if( digitalRead(MOTIONSENSOR) ) {
			printf("Motion detected!\r");
		}
		else {
			printf("                \r");
		}
		delay(10);
	}
	
	return 0;
}
