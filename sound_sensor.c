/**********************************************************************
 Using sound sensor to detect hand clap
 
 Hardware	:	CrowPi, Raspberry Pi 3B
 Software	:	Raspbian, WiringPi

 Compilation	:	gcc sound_sensor.c -l wiringPi 	
***********************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

const int sound_pin   = 18;
const int led_pin     = 22;	
//const int button1_pin = 37;
//const int button2_pin = 33;
//const int buzzer_pin  = 12;
//const int vibrat_pin  = 13;


int main()
{
	// Setup pin modes
	wiringPiSetupPhys();
	pinMode(sound_pin, INPUT);
	pullUpDnControl(sound_pin, PUD_UP);
	pinMode(led_pin, OUTPUT);

	// Instruction
	printf("Clap your hands near the sound sensor\n");
	printf("Press CTRL-C to end\n");
	
	while(1) {
		if( digitalRead(sound_pin) ) {
			printf("Sound detected\n");
			digitalWrite(led_pin, HIGH);
			sleep(1);
		}
		else {
			digitalWrite(led_pin, LOW);
		}
	}
	
	return 0;
}
