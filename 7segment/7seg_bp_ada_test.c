#include <stdio.h>
#include <string.h>
#include "7seg_bp_ada.h"

int main() {
	int rc;
	// prepare the backpack driver
        // (the first parameter is the raspberry pi i2c master controller attached to the HT16K33, the second is the i2c selection jumper)
        // The i2c selection address can be one of HT16K33_ADDR_01 to HT16K33_ADDR_08
	HT16K33 led_backpack1 = HT16K33_INIT(1, HT16K33_ADDR_01);
	
	// initialize the backpack
	rc = HT16K33_OPEN(&led_backpack1);
	if(rc != 0) {
		fprintf(stderr, "Error initializing HT16K33 led backpack (%s). Check your i2c bus (es. i2cdetect)\n", strerror(led_backpack1.lasterr));
		// you don't need to HT16K33_CLOSE() if HT16K33_OPEN() failed, but it's safe doing it.
		HT16K33_CLOSE(&led_backpack1);
		return 1;
	}
	
	// power on the ht16k33
	rc = HT16K33_ON(&led_backpack1);
	if(rc != 0) {
		fprintf(stderr, "Error putting the HT16K33 led backpack ON (%s). Check your i2c bus (es. i2cdetect)\n", strerror(led_backpack1.lasterr));
		// you don't need to HT16K33_OFF() if HT16K33_ON() failed, but it's safe doing it.
		HT16K33_OFF(&led_backpack1);
		HT16K33_CLOSE(&led_backpack1);
		return 1;
	}
	
	// make it shining bright
	HT16K33_BRIGHTNESS(&led_backpack1, 0x0F);
	
	// make it not blinking
	HT16K33_BLINK(&led_backpack1, HT16K33_BLINK_OFF);
	
	// power on the display
	HT16K33_DISPLAY(&led_backpack1, HT16K33_DISPLAY_ON);
	
	// use a commodity macro defined in 7seg_bp_ada.h to say "HELLO" on the display
	HT16K33_SAY_HELLO(&led_backpack1);
	
	/*
	HT16K33_UPDATE_DIGIT(&led_backpack1, 0, 'H', 0);
	HT16K33_UPDATE_DIGIT(&led_backpack1, 1, 'E', 0);
	HT16K33_UPDATE_DIGIT(&led_backpack1, 2, HT16K33_COLON_OFF, 0);
	HT16K33_UPDATE_DIGIT(&led_backpack1, 3, '#', 0);
	HT16K33_UPDATE_DIGIT(&led_backpack1, 4, 'o', 0);
	HT16K33_COMMIT(&led_backpack1);
	*/
	
	/*
	HT16K33_SAY_NO(&led_backpack1);
	HT16K33_SAY_YES(&led_backpack1);
	HT16K33_SAY_PLAY(&led_backpack1);
	HT16K33_SAY_HELP(&led_backpack1);
	HT16K33_SAY_YOU(&led_backpack1);
	HT16K33_SAY_COOL(&led_backpack1);
	*/
	
	// if you want to power off the display before exiting uncomment this one
	//rc = HT16K33_OFF(&led_backpack1);
	
	// close things (the display remains in the conditions left)
	HT16K33_CLOSE(&led_backpack1);
	
	return 0;
}
