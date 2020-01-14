/******************************************************************************
	Program: 	Vibration
				This example program shows the use of a button to control
				vibration module and another button to control buzzer.
				
	Hardware:	CrowPi (with Raspberry Pi 3B+)
				Selection switches LEFT: 5, 7; RIGHT: 1 
	Software:	Raspbian system with WiringPi library
	Compile:	$ gcc -o vibration vibration.c -lwiringPi -Wall
	Run with:	$ ./vibration
						
	Jan 2020	
******************************************************************************/

#include <stdio.h>
#include <wiringPi.h>

#define BUTTON_UP   37
#define BUTTON_DN   33
#define BUZZER      12
#define VIBRATION   13


int main()
{
	// Setup
	wiringPiSetupPhys();	
	pinMode(BUTTON_UP, INPUT);
	pinMode(BUTTON_DN, INPUT);
	pinMode(BUZZER, OUTPUT);
	pinMode(VIBRATION, OUTPUT);

	// Instruction
	printf("\nSwitch ON selection switches: LEFT: #5 & #7,  RIGHT: #1\n"); 
	printf("Press BUTTON UP to beep, BUTTON DOWN to vibrate\n");
	printf("Press CTRL-C to END program.\n");
	
	// Main loop
	while(1) {
		if( !digitalRead(BUTTON_UP) ) {
			digitalWrite(BUZZER, HIGH);
		}
		else if( !digitalRead(BUTTON_DN) ) {
			digitalWrite(VIBRATION, HIGH);
		}
		else {
			digitalWrite(BUZZER, LOW);
			digitalWrite(VIBRATION, LOW);
		}
	}
	
	return 0;
}
