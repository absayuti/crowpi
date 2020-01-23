// Program: 	Buzzer -- Switch on buzzer for 0.5 second
// To compile:	$ gcc -o buzzer buzzer.c -lwiringPi -Wall
// To run:		$ ./buzzer 

#include <stdio.h>
#include <wiringPi.h>

#define BUZZER  12

int main()
{
	// Setup
	wiringPiSetupPhys();
	pinMode(BUZZER, OUTPUT);

	printf("BUZZ!!!\n");
	digitalWrite(BUZZER, HIGH);		// Switch ON buzzer
	delay(500);						// Wait for 500 ms
	digitalWrite(BUZZER, LOW);		// Switch OFF buzzer
	
	return 0;
}
