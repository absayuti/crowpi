#include <stdio.h>
#include <wiringPi.h>

const int pwm_pin = 18;
const int led_pin = 23;
const int button  = 17;
const int pwm_val = 75;


int main()
{
	wiringPiSetupPhys();
	
	pinMode(pwm_pin, PWM_OUTPUT);
	pinMode(led_pin, OUTPUT);
	pinMode(button, INPUT);
	pullUpDnControl(button, PUD_UP);

	printf("I am blinky. Press CTRL-C to quit");
	
	while(1) {
		if( digitalRead(button)) {
			pwmWrite(pwm_pin, pwm_val);
			digitalWrite(led_pin, LOW);
		}
		else {
			pwmWrite(pwm_pin, 1024 - pwm_val);
			digitalWrite(led_pin, HIGH);
			delay(75);
			digitalWrite(led_pin, LOW);
			delay(75);
		}
	}
	
	return 0;
}
