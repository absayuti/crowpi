#include <stdio.h>
#include <wiringPi.h>

const int led_pin = 3;


int main()
{
	wiringPiSetupPhys();	
	pinMode(led_pin, OUTPUT);

	printf("I am blinky. Press CTRL-C to quit");
	
	while(1) {
		digitalWrite(led_pin, HIGH);
		delay(300);
		digitalWrite(led_pin, LOW);
		delay(300);
	}
	
	return 0;
}
