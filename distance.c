/******************************************************************************
	Program: 	Distance
				This example program shows the use of ultrasonic distance
				sensor to measure distance in centimetres and inches.
				
	Hardware:	CrowPi (with Raspberry Pi 3B+)
	Software:	Raspbian system with WiringPi library
	Compile:	$ gcc -o distance -distance.c -lwiringPi -Wall
	Run with:	$ ./distance
						
	Jan 2020	
******************************************************************************/

#include <stdio.h>
#include <wiringPi.h>

// Pins for the DHT11 distance sensor
#define  TRIG 	36
#define  ECHO  	32
#define  CM     0
#define  INCH   1

// Function prototypes
int get_distance( int unit );


//-----------------------------------------------------------------------------
// Main function

int main()
{
	int distanceCM, distanceINCH;
	
	// Setup
	wiringPiSetupPhys();	
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);
	digitalWrite(TRIG, LOW);
	delay(1000);						// Wait for sensor to settle down
	
	while(1) {
		distanceCM = get_distance(CM);
		distanceINCH = get_distance(INCH);
		printf("Distance = %d cm / %d inch\n", distanceCM, distanceINCH );		
		delay(1000);
	}
	
	return 0;
}


//-----------------------------------------------------------------------------
// Function: Get distance

int get_distance( int unit )
{
	int  pulse_start, pulse_end;

	digitalWrite(TRIG, LOW);					// Reset TRIG pulse
	delay(1);
	digitalWrite(TRIG, HIGH);					// Start TRIG pulse
	delayMicroseconds(10);						// .. for 10 us
	digitalWrite(TRIG, LOW);
	
	while( !digitalRead(ECHO) )					// Wait for start of ECHO pulse
		pulse_start = micros();					// Record start time
	
	while( digitalRead(ECHO) )					// Wait until ECHO pulse ends
		pulse_end = micros();					// Record end time
	
	if( unit == CM )							// Convert to distance CM/INCH
		return 0.5 * (pulse_end - pulse_start) / 29.1;
	else
		return 0.5 * (pulse_end - pulse_start) / 74;
}