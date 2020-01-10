/**********************************************************************
 Using 2x16 LCD display to print 'Hello'
 
 Hardware	:	CrowPi, Raspberry Pi 3B
 Software	:	Raspbian, WiringPi

 Compilation	:	gcc -Wall [this_file.c] -l wiringPi
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <mcp23008.h>


// Char LCD Commands
#define LCD_CLEARDISPLAY	0x01
#define LCD_RETURNHOME      0x02
#define LCD_ENTRYMODESET    0x04
#define LCD_DISPLAYCONTROL  0x08
#define LCD_CURSORSHIFT     0x10
#define LCD_FUNCTIONSET     0x20
#define LCD_SETCGRAMADDR    0x40
#define LCD_SETDDRAMADDR    0x80

// Entry flags
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Control flags
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00

// Move flags
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

// Function set flags
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

// Offset for up to 4 rows.
//#define LCD_ROW_OFFSETS         (0x00, 0x40, 0x14, 0x54)

// Char LCD backpack GPIO numbers.
#define LCD_BACKPACK_RS		1
#define LCD_BACKPACK_EN     2
#define LCD_BACKPACK_D4     3
#define LCD_BACKPACK_D5     4
#define LCD_BACKPACK_D6     5
#define LCD_BACKPACK_D7     6
#define LCD_BACKPACK_LITE   7
#define CHARLCD   			0x21


// Function prototypes
int init_charlcd();



//----- MAIN FUNCTION ----------------------------------------------------

int main( int argc, char **argv )
{
	int  fd;
	
	wiringPiSetup();
	fd = init_charlcd();

	return 0;
}


//------------------------------------------------------------------------

int init_charlcd()
{
	int  fd, rc;

	fd = wiringPiI2CSetup( CHARLCD );
	if( fd < 0 ) {
		printf("Error initialising I2C\n");
		exit(1);
	}

	// Setup pin modes via MCP23008
	rc = mcp23008Setup(0x64, CHARLCD);
	if( rc < 0 ) {
		printf("Error initialising MCP23008\n");
		exit(1);
	}
	pinMode( 0x64, OUTPUT );
	pinMode( 0x64+LCD_BACKPACK_RS, OUTPUT);
	pinMode( 0x64+LCD_BACKPACK_EN, OUTPUT);
	pinMode( 0x64+LCD_BACKPACK_D4, OUTPUT);
	pinMode( 0x64+LCD_BACKPACK_D5, OUTPUT);
	pinMode( 0x64+LCD_BACKPACK_D6, OUTPUT);
	pinMode( 0x64+LCD_BACKPACK_D7, OUTPUT);

	// Init the display
	wiringPiI2CWrite(fd, 0x33);
	wiringPiI2CWrite(fd, 0x32);

	// Init display control, function and mode registers
	wiringPiI2CWrite(fd, LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF );
	wiringPiI2CWrite(fd, LCD_FUNCTIONSET | LCD_4BITMODE | LCD_1LINE | LCD_2LINE | LCD_5x8DOTS );
	wiringPiI2CWrite(fd, LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT );
	wiringPiI2CWrite(fd, LCD_CLEARDISPLAY );
	delay(3);

	return fd;
}	
