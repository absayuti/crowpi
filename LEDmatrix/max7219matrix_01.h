// Header file to be used for driving the LED matrix via MAX7219 on CrowPi
// Dec 2019

// LED matrix command / register numbers
#define DECODE_MODE   0x09
#define INTENSITY     0x0a
#define SCAN_LIMIT    0x0b
#define SHUTDOWN      0x0c
#define DISPLAY_TEST  0x0f

static void MAX7219write( unsigned char channel, unsigned char reg_number, unsigned char data )
{
	unsigned char buffer[100];

	buffer[0] = reg_number;
	buffer[1] = data;
	wiringPiSPIDataRW( channel, buffer, 2 );
}
