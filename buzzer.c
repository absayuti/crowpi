#include <stdio.h>
#include <wiringPi.h>

const int buzzer_pin = 12;


int main()
{
	wiringPiSetupPhys();
	
	pinMode(buzzer_pin, OUTPUT);

	printf("BUZZ!!!");
	
	digitalWrite(0, HIGH);
	digitalWrite(buzzer_pin, HIGH);
	delay(500);
	digitalWrite(buzzer_pin, LOW);
	digitalWrite(0, LOW);
	
	return 0;
}
