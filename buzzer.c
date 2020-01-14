// Program: Buzzer -- Switch on buzzer for 0.5 second
#include <stdio.h>
#include <wiringPi.h>

#define BUZZER  12

int main()
{
	// Setup
	wiringPiSetupPhys();
	pinMode(BUZZER, OUTPUT);

	printf("BUZZ!!!\n");
	digitalWrite(BUZZER, HIGH);
	delay(500);
	digitalWrite(BUZZER, LOW);
	
	return 0;
}
