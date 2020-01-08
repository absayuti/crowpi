/**
 * Copyright 2014 Dino Ciuffetti <dino@tuxweb.it>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* apt-get install libi2c-dev */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdint.h>

#include "7seg_bp_ada.h"

/**
 * write one byte to i2c bus
 */
static inline int ht16k33_write8(HT16K33 *backpack, uint8_t val) {
	if(backpack->adapter_fd == -1) { // not opened
		// TODO: choose a valid errno error
		backpack->lasterr = -1;
		return -1;
	}
	
	//backpack->buf[0] = val;
	if(write(backpack->adapter_fd, &val, sizeof(uint8_t)) != 1) {
		backpack->lasterr = errno;
		return -1;
	}
	return 0;
}
/**
 * Convert a ascii char to it's 7 segment display binary representation.
 * 
 * WARNING: Due to the hardware code, this one is valid only for the adafruit 7 segment display!!!
 * It won't work with different electronic schematics.
 * 
 * The parameter "x" is the value to display (NOTE: not all characters are valid on a 7 seg display!!)
 * The parameter decimal point can be 0 (decimal point led OFF) or 1 (decimal point led ON)
 */
static inline uint8_t ascii_to_7seg(unsigned int x, unsigned short decimal_point) {
	uint8_t seg7;
	
	if(x > 128) return 0x89; // char not available
	
	seg7 = ht16k55_digits[x];
	//if (decimal_point == 1) seg7 |= 0x80;
	if (decimal_point == 1) seg7 |= ht16k55_digits[',']; // add the comma
	
	//printf("seg7: 0x%02x\n", seg7);
	return seg7;
}

// HT16K33 Functions (inspired on Adafruit_LEDBackpack.cpp at https://github.com/adafruit/Adafruit-LED-Backpack-Library)
int HT16K33_OPEN(HT16K33 *backpack) {
	char filename[20];
	unsigned short i;
	
	if(backpack->adapter_fd != -1) { // already opened
		// TODO: choose a valid errno error
		backpack->lasterr = -1;
		return -1;
	}
	snprintf(filename, 20, "/dev/i2c-%d", backpack->adapter_nr);
	if ((backpack->adapter_fd = open(filename, O_RDWR)) < 0) { // open the device file (requests i2c-dev kernel module loaded)
		backpack->lasterr = errno;
		return -1;
	}
	
	if (ioctl(backpack->adapter_fd, I2C_SLAVE, backpack->driver_addr) < 0) { // talk to the requested device
		backpack->lasterr = errno;
		close(backpack->adapter_fd);
		backpack->adapter_fd = -1;
		return -1;
	}
	
	/*
	HT16K33_ON(backpack);
	HT16K33_BLINK(backpack, HT16K33_BLINK_OFF);
	HT16K33_DISPLAY(backpack, HT16K33_DISPLAY_OFF);
	HT16K33_OFF(backpack);
	*/
	
	for (i=0; i<5; i++) {
		HT16K33_CLEAN_DIGIT(backpack, i);
	}
	
	return 0;
}
void HT16K33_CLOSE(HT16K33 *backpack) {
	if(backpack->adapter_fd != -1) {
		close(backpack->adapter_fd);
		backpack->adapter_fd = -1;
	}
	HT16K33_OFF(backpack);
}
// wake up HT16K33 by setting the S option bit of the "system setup register" (that starts at 0x20h. ON: 0x21 (00100001), OFF: 0x20 (00100000))
int HT16K33_ON(HT16K33 *backpack) {
	int rc;
	
	if(backpack->adapter_fd == -1) { // not opened
		// TODO: choose a valid errno error
		backpack->lasterr = -1;
		return -1;
	}
	
	// check if the display is ON (in this case the display is not in power saving mode, and we can send the command)
	if((HT16K33_DISPLAY_ON & backpack->display_state) == HT16K33_DISPLAY_ON) { // already on
		rc = 0;
	} else { // it's off. Wake up!
		rc = ht16k33_write8(backpack, HT16K33_WAKEUP_OP);
		if(rc == 0) backpack->display_state = HT16K33_DISPLAY_ON;
	}
	return rc;
}
// turn HT16K33 into power saving mode
int HT16K33_OFF(HT16K33 *backpack) {
	int rc;
	
	if(backpack->adapter_fd == -1) { // not opened
		// TODO: choose a valid errno error
		backpack->lasterr = -1;
		return -1;
	}
	
	// check if the display is ON (in this case the display is not in power saving mode, and we can send the command)
	if((HT16K33_DISPLAY_ON & backpack->display_state) == HT16K33_DISPLAY_ON) { // it's on
		// go to sleep
		rc = ht16k33_write8(backpack, HT16K33_SLEEP_OP);
		if(rc == 0) backpack->display_state = HT16K33_DISPLAY_OFF;
	} else { // already off
		rc = 0;
	}
	return rc;
}
/**
 * blink_cmd can be one of HT16K33_BLINK_OFF, HT16K33_BLINK_SLOW (0.5 HZ), HT16K33_BLINK_NORMAL (1 HZ), HT16K33_BLINK_FAST (2 HZ)
 */
int HT16K33_BLINK(HT16K33 *backpack, ht16k33blink_t blink_cmd) {
	int rc=-1;
	uint8_t cmd;
	ht16k33blink_t new_blink_state;
	
	if(backpack->adapter_fd == -1) {
		backpack->lasterr = -1;
		return -1;
	}
	
	/* "display setup register" command is composed as:
	 * MSB             LSB
	 * 1 0 0 0 0 B1 B0 D
	 * 
	 * B1-B0 defines the blinking frequency
	 * D defined display on/off status
	 */
	
	// no blink, display off
	cmd = HT16K33_DISPLAY_SETUP_BASE;
	
	switch(blink_cmd) {
	case HT16K33_BLINK_OFF:
		cmd |= HT16K33_BLINK_OFF | backpack->display_state;
		new_blink_state = HT16K33_BLINK_OFF;
		break;
	case HT16K33_BLINK_SLOW:
		cmd |= HT16K33_BLINK_SLOW | backpack->display_state;
		new_blink_state = HT16K33_BLINK_SLOW;
		break;
	case HT16K33_BLINK_NORMAL:
		cmd |= HT16K33_BLINK_NORMAL | backpack->display_state;
		new_blink_state = HT16K33_BLINK_NORMAL;
		break;
	case HT16K33_BLINK_FAST:
		cmd |= HT16K33_BLINK_FAST | backpack->display_state;
		new_blink_state = HT16K33_BLINK_FAST;
		break;
	default:
		cmd |= HT16K33_BLINK_NORMAL | backpack->display_state;
		new_blink_state = HT16K33_BLINK_NORMAL;
		break;
	}
	
	// check if the display is ON (in this case the display is NOT in power saving mode, and we can send the command)
	if((HT16K33_DISPLAY_ON & backpack->display_state) == HT16K33_DISPLAY_ON) {
		rc = ht16k33_write8(backpack, cmd);
	} else {
		rc = 0;
	}
	
	if (rc == 0) {
		backpack->blink_state = new_blink_state;
	}
	
	return rc;
}
/**
 * display_cmd can be one of HT16K33_DISPLAY_OFF or HT16K33_DISPLAY_ON
 */
int HT16K33_DISPLAY(HT16K33 *backpack, ht16k33display_t display_cmd) {
	int rc=-1;
	uint8_t cmd;
	ht16k33display_t new_display_state;
	
	if(backpack->adapter_fd == -1) {
		backpack->lasterr = -1;
		return -1;
	}
	
	/* "display setup register" command is composed as:
	 * MSB             LSB
	 * 1 0 0 0 0 B1 B0 D
	 * 
	 * B1-B0 defines the blinking frequency
	 * D defined display on/off status
	 */
	
	// no blink, display off
	cmd = HT16K33_DISPLAY_SETUP_BASE;
	
	switch(display_cmd) {
	case HT16K33_DISPLAY_OFF:
		cmd |= backpack->blink_state | HT16K33_DISPLAY_OFF;
		new_display_state = HT16K33_DISPLAY_OFF;
		break;
	case HT16K33_DISPLAY_ON:
		cmd |= backpack->blink_state | HT16K33_DISPLAY_ON;
		new_display_state = HT16K33_DISPLAY_ON;
		break;
	default:
		cmd |= backpack->blink_state | HT16K33_DISPLAY_OFF;
		new_display_state = HT16K33_DISPLAY_OFF;
		break;
	}
	
	// check if the display is ON (in this case the display is NOT in power saving mode, and we can send the command)
	if((HT16K33_DISPLAY_ON & backpack->display_state) == HT16K33_DISPLAY_ON) {
		rc = ht16k33_write8(backpack, cmd);
	} else {
		rc = 0;
	}
	
	if(rc == 0) {
		backpack->display_state = new_display_state;
	}
	
	return rc;
}
int HT16K33_BRIGHTNESS(HT16K33 *backpack, uint8_t brightness) {
	int rc=-1;
	uint8_t cmd;
	//unsigned char buf[16];
	//unsigned char x;
	
	if(backpack->adapter_fd == -1) {
		backpack->lasterr = -1;
		return -1;
	}
	
	if(brightness > 0x0F) brightness = 0x0F; // maximum brightness cannot exceed low nibble
	
	cmd = HT16K33_DIMMING_BASE | brightness;
	// check if the display is ON (in this case the display is NOT in power saving mode, and we can send the command)
	if((HT16K33_DISPLAY_ON & backpack->display_state) == HT16K33_DISPLAY_ON) {
		rc = ht16k33_write8(backpack, cmd);
	} else {
		rc = 0;
	}
	
	if(rc == 0) {
		backpack->brightness = brightness;
	}
	
	return rc;
}
/**
 * Update a single display digit, identified by the digit parameter, starting from 0.
 * The digit 2 is the colon sign in the middle of the four digits.
 * To make it reflected on the 7 segment display you have to call HT16K33_COMMIT()
 */
int HT16K33_UPDATE_DIGIT(HT16K33 *backpack, unsigned short digit, const unsigned char value, unsigned short decimal_point) {
	// digit 0 is the first digit on the left
	// digit 1 is the second digit on the left
	// digit 2 is the colon sign in the middle
	// digit 3 is the third digit, on the right
	// digit 4 is the last digit, on the right
	
	if (digit > 4) { // we are working with a 7 segment, 4 digits display. Only the first 5 columns are used to drive the display
		return -1;
	}
	if (value > 128) { // cannot represent the given character on the display
		return -1;
	}
	
	backpack->display_buffer.com[digit] = ascii_to_7seg(value, decimal_point);
	//printf("digit %i: 0x%02X\n", digit, backpack->display_buffer.com[digit]);
	return 0;
}
/**
 * Clean a single display digit, identified by the digit parameter, starting from 0.
 * The digit 2 is the colon sign in the middle of the four digits.
 * To make it reflected on the 7 segment display you have to call HT16K33_COMMIT()
 */
int HT16K33_CLEAN_DIGIT(HT16K33 *backpack, unsigned short digit) {
	// digit 0 is the first digit on the left
	// digit 1 is the second digit on the left
	// digit 2 is the colon sign in the middle
	// digit 3 is the third digit, on the right
	// digit 4 is the last digit, on the right
	
	if (digit > 4) { // we are working with a 7 segment, 4 digits display. Only the first 5 columns are used to drive the display
		return -1;
	}
	
	backpack->display_buffer.com[digit] = 0x00;
	return 0;
}
/**
 * Commit the display buffer data to the 7 segment display, showing the saved data.
 * Data must be saved to the buffer calling HT16K33_UPDATE_DIGIT() before calling this one.
 */
int HT16K33_COMMIT(HT16K33 *backpack) {
	unsigned char buf[16];
	int i;
	
	if(backpack->adapter_fd == -1) {
		backpack->lasterr = -1;
		return -1;
	}
	
	for (i=0; i<10; i+=2) { // only the first 5 x 16 bits are used in a 7 segment display...
		buf[i]   = backpack->display_buffer.com[i/2];
		buf[i+1] = 0; // ... mhhh... better: only the first 8 bits of each 16 bits word is use really!!
	}
	for (i=10; i<16; i++) { // data not used in a adafruit 7 segment display, but HT16K33 will need it anyway
		buf[i]   = 0;
	}
	
	// commit data to the i2c bus
	i = i2c_smbus_write_i2c_block_data(backpack->adapter_fd, 0x00, 16, buf);
	if (i == 0) return 0;
	
	return -1;
}
