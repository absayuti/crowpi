/*****************************************************************************
	Program:	Example 7seg
				Example program using 7seg_bp_ada library on the 7segment 
				diplay on CrowPi. It showis how to display words, numbers and
				as a basic counter.
	Hardware:	CrowPi (with Raspberry Pi 3B+, including 7-segment display 
				via Adafruit Backpack module) 
	Software:	Raspbian system with WiringPi library
				7seg_bp_ada library 
					https://dino.ciuffetti.info/2014/04/adafruit-4-digit-7-
					segment-backpack-raspberry-pi-in-c/
				libi2c-dev
					$ sudo apt-get install libi2c-dev
	Compile:	$ gcc -o 7seg example_7seg.c -7seg_bp_ada.c -lwiringPi -Wall
	Run with:	$ ./7seg

	Jan 2020
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include "7seg_bp_ada.h"

// Function prototypes
HT16K33 init_7seg();		// Call this function at beginning of program
void display_words(HT16K33 device);
void display_numbers(HT16K33 device);
void counter(HT16K33 device, int start);


//-----------------------------------------------------------------------------
// Main function

int main() 
{
	HT16K33 sevenseg;		// Declare special struct to point to 7-seg display

	printf("Initializing 7-segment display...\n");
	sevenseg = init_7seg();					// Initialize the 7 segment display
	printf("Displaying a few basic words...\n");
	display_words(sevenseg);
	printf("Displaying the number 12:34 ...\n");
	display_numbers(sevenseg);
	printf("Counting from 1000 to 9999...\n");
	counter(sevenseg, 1000);
	printf("Switching OFF 7-segment display ...\n");
      
    HT16K33_OFF(&sevenseg);  				// Power off display before END
	return 0;
}

//-----------------------------------------------------------------------------
// Function: Initialize 7 segment display

HT16K33 init_7seg()
{
	int rc;
	HT16K33 led_backpack1 = HT16K33_INIT(1, HT16K33_ADDR_01);

	// initialize the backpack
	rc = HT16K33_OPEN(&led_backpack1);
	if(rc != 0) {
		printf("Error initializing HT16K33 led backpack (%s).\n", \
				strerror(led_backpack1.lasterr));
		printf("Check your i2c bus (es. i2cdetect)\n");
		HT16K33_CLOSE(&led_backpack1);
		exit(0); 
	}
	
	// power on the ht16k33
	rc = HT16K33_ON(&led_backpack1);
	if(rc != 0) {
		printf("Error putting the HT16K33 led backpack ON (%s).\n", \
				strerror(led_backpack1.lasterr));
		printf("Check your i2c bus (es. i2cdetect)\n");
		HT16K33_OFF(&led_backpack1);
		HT16K33_CLOSE(&led_backpack1);
		exit(0);
	}

	// make it shining bright
	HT16K33_BRIGHTNESS(&led_backpack1, 0x0F);
	// make it not blinking
	HT16K33_BLINK(&led_backpack1, HT16K33_BLINK_OFF);
	// power on the display
	HT16K33_DISPLAY(&led_backpack1, HT16K33_DISPLAY_ON);
	
	return led_backpack1;
}

//-----------------------------------------------------------------------------
// Function: Display words -- displays a few pre-defined words

void display_words( HT16K33 led_backpack1 )
{
	// Display the basic words defined in the library
	HT16K33_SAY_HELLO(&led_backpack1);
	delay(1000);
	HT16K33_SAY_NO(&led_backpack1);
	delay(1000);
        HT16K33_SAY_YES(&led_backpack1);
	delay(1000);
        HT16K33_SAY_PLAY(&led_backpack1);
	delay(1000);
        HT16K33_SAY_HELP(&led_backpack1);
	delay(1000);
        HT16K33_SAY_YOU(&led_backpack1);
	delay(1000);
        HT16K33_SAY_COOL(&led_backpack1);
	delay(2000);
}

//-----------------------------------------------------------------------------
// Function: Display numbers -- Displays 12:34 on the 7-segment display 

void display_numbers( HT16K33 led_backpack1 )
{
	HT16K33_UPDATE_DIGIT(&led_backpack1, 0, '1', 0);
	HT16K33_UPDATE_DIGIT(&led_backpack1, 1, '2', 0);
	HT16K33_UPDATE_DIGIT(&led_backpack1, 2, HT16K33_COLON_ON, 0);
	HT16K33_UPDATE_DIGIT(&led_backpack1, 3, '3', 0);
	HT16K33_UPDATE_DIGIT(&led_backpack1, 4, '4', 0);
	HT16K33_COMMIT(&led_backpack1);
	delay(2000);
}

//-----------------------------------------------------------------------------
// Function: Counter -- basic counting from start to 9999

void counter(HT16K33 sevenseg, int start)
{
	int count = start;
	char digit[5];

	while(1) {
		sprintf(digit, "%4d", count);
		printf("%c %c %c %c\n", digit[0], digit[1], digit[2], digit[3]);
		HT16K33_UPDATE_DIGIT(&sevenseg, 0, digit[0], 0);
		HT16K33_UPDATE_DIGIT(&sevenseg, 1, digit[1], 0);
		HT16K33_UPDATE_DIGIT(&sevenseg, 2, HT16K33_COLON_OFF, 0);
		HT16K33_UPDATE_DIGIT(&sevenseg, 3, digit[2], 0);
		HT16K33_UPDATE_DIGIT(&sevenseg, 4, digit[3], 0);
		HT16K33_COMMIT(&sevenseg);
		count++;
		if( count > 9999 ) 
			return;
		delay(100);
	}
}
