/**********************************************************************
 Detecting motion 
 
 Hardware	:	CrowPi, Raspberry Pi 3B
				Input switches: ???
				Output switches: ???
 Software	:	Raspbian, WiringPi
 	
***********************************************************************/

#include <stdio.h>
#include <wiringPi.h>

const int motion_pin = 16;


int main()
{
	// Setup pin modes
	wiringPiSetupPhys();	
	pinMode(motion_pin, INPUT);

	// Instruction
	printf("Wave your hand above the motion sensor\n");
	printf("Press CTRL-C to end\n");
	
	while(1) {
		if( digitalRead(motion_pin) ) {
			printf("Motion detected!\r");
		}
		else {
			printf("                \r");
		}
		delay(5);		
	}
	
	return 0;
}
