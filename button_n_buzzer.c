/**********************************************************************
	Program: 	Button and buzzer
				This example program shows the use of a button to
				control the buzzer
				
	Hardware:	CrowPi (with Raspberry Pi 3B+)

	Software:	Raspbian system with WiringPi library
				
	Compile:	$ gcc -o example button_n_buzzer.c -lwiringPi -Wall
		
	Run with:	$ ./example
						
	Jan 2020
***********************************************************************/

#include <stdio.h>
#include <wiringPi.h>

#define  BUTTON  37
#define  BUZZER  12

int main()
{
	// Setup WiringPi and pin modes
	wiringPiSetupPhys();	
	pinMode(BUTTON, INPUT);
	pinMode(BUZZER, OUTPUT);
	
	// Print instruction
	printf("\nSwitch ON #7 @ LEFT selection switches.\n");
	printf("Press the UP button to switch on buzzer.\n");
	printf("Press Ctrl-C to EXIT.\n");
	
	// Main loop
	while(1) {
		if( !digitalRead(BUTTON) ) {
			digitalWrite(BUZZER, HIGH);
		}
		else {
			digitalWrite(BUZZER, LOW);
		}
	}
	
	return 0;
}
