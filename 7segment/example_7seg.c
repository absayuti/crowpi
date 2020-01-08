/*****************************************************************************
	Basic test program for 7segment diplay on CrowPi,
	showing basic functions of the library.
	Also how to do make a counter. 

	The initialization routine has be place in a function:
	init_7seg() so that the code can be more modularly written.

	Hardware:
		CrowPi + 7-segment via Adafruit Backpack module 

	Requires:
		libi2c-dev : $ sudo apt-get install libi2c-dev
		7seg_bp_ada.c library
		( https://dino.ciuffetti.info/2014/04/adafruit-4-digit-7-segment-backpack-raspberry-pi-in-c/ )

	Compile with:
		gcc 7seg_bp_ada_test.c 7seg_bp_ada.c
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "7seg_bp_ada.h"

// Function prototype --------------------------------------------------------

HT16K33 init_7seg();		// Call this function at beginning of program
void display_words(HT16K33 device);
void display_numbers(HT16K33 device);
void counter(HT16K33 device, int start);


// Main ----------------------------------------------------------------------

int main() 
{
	HT16K33 sevenseg;	// Declare a special struct to point to the 7 segment display
	int  rc;

	printf("Initializing 7-segment display...\n");
	sevenseg = init_7seg();	// Initialize the 7 segment display
	printf("Displaying a few basic words...\n");
	display_words(sevenseg);
	printf("Displaying the number 12:34 ...\n");
	display_numbers(sevenseg);
	printf("Counting from 1000 to 9999...\n");
	counter(sevenseg, 1000);
	printf("Switching OFF 7-segment display ...\n");
        // if you want to power off the display before exiting uncomment this o$
        rc = HT16K33_OFF(&sevenseg);
	
	return 0;
}


// Counter function: from start to 9999 ---------------------------------------

void counter(HT16K33 sevenseg, int start)
{
	// Counter starts here.
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
		usleep(100000);
	}
}


// Function to initialize 7 segment display ----------------------------------

HT16K33 init_7seg()
{
        int rc;
        HT16K33 led_backpack1 = HT16K33_INIT(1, HT16K33_ADDR_01);

        // initialize the backpack
        rc = HT16K33_OPEN(&led_backpack1);
        if(rc != 0) {
                fprintf(stderr, "Error initializing HT16K33 led backpack (%s). Check your i2c bus (es. i2cdetect)\n", strerror(led_backpack1.lasterr));
                // you don't need to HT16K33_CLOSE() if HT16K33_OPEN() failed, but it's safe doing it.
                HT16K33_CLOSE(&led_backpack1);
                exit(0); 
        }

	// power on the ht16k33
        rc = HT16K33_ON(&led_backpack1);
        if(rc != 0) {
                fprintf(stderr, "Error putting the HT16K33 led backpack ON (%s). Check your i2c bus (es. i2cdetect)\n", strerror(led_backpack1.lasterr));
                // you don't need to HT16K33_OFF() if HT16K33_ON() failed, but it's safe doing it.
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


// Function that displays a few pre-defined words ------------------------------

void display_words( HT16K33 led_backpack1 )
{
	// Display the basic words defined in the library
	HT16K33_SAY_HELLO(&led_backpack1);
	sleep(2);
	HT16K33_SAY_NO(&led_backpack1);
	sleep(2);
        HT16K33_SAY_YES(&led_backpack1);
	sleep(2);
        HT16K33_SAY_PLAY(&led_backpack1);
	sleep(2);
        HT16K33_SAY_HELP(&led_backpack1);
	sleep(2);
        HT16K33_SAY_YOU(&led_backpack1);
 	sleep(2);
        HT16K33_SAY_COOL(&led_backpack1);
	sleep(2);
}

// Function that displays 12:34 on the 7-segment display ---------------------

void display_numbers( HT16K33 led_backpack1 )
{
        HT16K33_UPDATE_DIGIT(&led_backpack1, 0, '1', 0);
        HT16K33_UPDATE_DIGIT(&led_backpack1, 1, '2', 0);
        HT16K33_UPDATE_DIGIT(&led_backpack1, 2, HT16K33_COLON_ON, 0);
        HT16K33_UPDATE_DIGIT(&led_backpack1, 3, '3', 0);
        HT16K33_UPDATE_DIGIT(&led_backpack1, 4, '4', 0);
        HT16K33_COMMIT(&led_backpack1);
	sleep(2);
}


// --------------------------------- END -------------------------------------
