/**********************************************************************
 Buzzer controlled by button
 
 Hardware	:	CrowPi, Raspberry Pi 3B
				Input switches: #7 ON
 Software	:	Raspbian, WiringPi
 	
***********************************************************************/

#include <stdio.h>
#include <wiringPi.h>

const int button_pin = 37;
const int buzzer_pin = 12;


int main()
{
	// Setup pin modes
	wiringPiSetupPhys();	
	pinMode(button_pin, INPUT);
	pinMode(buzzer_pin, OUTPUT);
	
	while(1) {
		if( !digitalRead(button_pin) ) {
			printf("BUZZ!!!");
			digitalWrite(buzzer_pin, HIGH);
		}
		else {
			digitalWrite(buzzer_pin, LOW);
		}
	}
	
	return 0;
}
