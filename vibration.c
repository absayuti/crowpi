/**********************************************************************
 Vibration controlled by buttons
 
 Hardware	:	CrowPi, Raspberry Pi 3B
				Input switches: #7 ON
				Output switches: #1 ON
 Software	:	Raspbian, WiringPi
 	
***********************************************************************/

#include <stdio.h>
#include <wiringPi.h>

const int button1_pin = 37;
const int button2_pin = 33;
const int buzzer_pin = 12;
const int vibrat_pin = 13;


int main()
{
	// Setup pin modes
	wiringPiSetupPhys();	
	pinMode(button1_pin, INPUT);
	pinMode(button2_pin, INPUT);
	pinMode(buzzer_pin, OUTPUT);
	pinMode(vibrat_pin, OUTPUT);

	// Instruction
	printf("Press BUTTON UP to beep, BUTTON DOWN to vibrate\n");
	printf("Press CTRL-C to end\n");
	
	while(1) {
		if( !digitalRead(button1_pin) ) {
			digitalWrite(buzzer_pin, HIGH);
		}
		else if( !digitalRead(button2_pin) ) {
			digitalWrite(vibrat_pin, HIGH);
		}
		else {
			digitalWrite(buzzer_pin, LOW);
			digitalWrite(vibrat_pin, LOW);
		}
	}
	
	return 0;
}
