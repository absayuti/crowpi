/*****************************************************************************
	Program: 	Button matrix
				This example program shows the use of the button matrix on 
				CrowPi.
				
	Hardware:	CrowPi (with Raspberry Pi 3B+)
				CrowPi's built-in button matrix
				Monitor

	Software:	Raspbian system with WiringPi library
				
	Others:		Compile with:
				$ gcc -o example example_button_matrix.c -lwiringPi -Wall
		
				Run with:
				$ ./example
						
				Jan 2020
				
******************************************************************************/

#include <stdio.h>
#include <wiringPi.h>

// Function prototypes
int setup_button_matrix();
int get_button_matrix();

// Button matrix configuration for key numbers
int BMrow[] = { 13, 15, 29, 31 };		// Pin #s for the rows
int BMcol[] = { 22, 37, 35, 33 };		// Pin #s for the columns


// ---------------------------------------------------------------------------
// Main function
//
int main( int argc, char **argv )
{
	int k;
	
	setup_button_matrix();

	printf("Press button matrix...\n");

	while(1) {
		do {
			k = get_button_matrix();
			if( k ) printf("Key = %d\n", k);
		} while( k==0 );
		delay(200);
	}
	return 0;
}


// ---------------------------------------------------------------------------
// Function: to setup button matrix
//
int setup_button_matrix()
{
	int r, c;

	// Setup GPIO
	wiringPiSetupPhys();
	for( c=0; c < 4; c++ ) {
		pinMode( BMcol[c], OUTPUT );
		digitalWrite( BMcol[c], HIGH );
	}
	for( r=0; r < 4; r++ ) {
		pinMode( BMrow[r], PUD_UP );
		pullUpDnControl( BMrow[r], PUD_UP );
	}
	return 0;
}


// ---------------------------------------------------------------------------
// Function: Read button matrix and return corresponding key number
//
int get_button_matrix()
{
	int r, c, key;

	for( c=0; c < 4; c++ ) {
		digitalWrite( BMcol[c], LOW );
		for( r=0; r < 4; r++ ) {
			if( !digitalRead(BMrow[r]) ) {
				key = r*4 + c + 1;
				delay(50);
				return key;
			}
		}
		digitalWrite( BMcol[c], HIGH );
	}
	return 0;
}
