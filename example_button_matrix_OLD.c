#include <stdio.h>
#include <wiringPi.h>


int setup_button_matrix();
int get_button_matrix();


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


int setup_button_matrix()
{
	int row[] = { 13, 15, 29, 31 };		// Pin #s for the rows
	int col[] = { 22, 37, 35, 33 };		// Pin #s for the columns
	int r, c;

	// Setup GPIO
	wiringPiSetupPhys();
	for( c=0; c < 4; c++ ) {
		pinMode( col[c], OUTPUT );
		digitalWrite( col[c], HIGH );
	}
	for( r=0; r < 4; r++ ) {
		pinMode( row[r], PUD_UP );
		pullUpDnControl( row[r], PUD_UP );
	}
	return 0;
}


int get_button_matrix()
{
	int row[] = { 13, 15, 29, 31 };		// Pin #s for the rows
	int col[] = { 22, 37, 35, 33 };		// Pin #s for the columns
	int r, c, key;

	for( c=0; c < 4; c++ ) {
		digitalWrite( col[c], LOW );
		for( r=0; r < 4; r++ ) {
			if( !digitalRead(row[r]) ) {
				key = r*4 + c + 1;
				delay(50);
				return key;
			}
		}
		digitalWrite( col[c], HIGH );
	}
	return 0;
}
