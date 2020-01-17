/******************************************************************************
	Program: 	Paddle and ball

	Input:		Independent buttons
	Output:		LED matrix, terminal screen
	Hardware:	CrowPi (with Raspberry Pi 3B+)
	Software:	Raspbian system with WiringPi library
				max7219matrix.h
	Compile:	$ gcc -o snake snake.c -lwiringPi -Wall	
	Run with:	$ ./direction
						
	Jan 2020
******************************************************************************/
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "max7219matrix.h"

// Define button pins 
#define  BUTTON_UP  37
#define  BUTTON_DN  33
#define  BUTTON_RT  35
#define  BUTTON_LF  22
#define  MAXROW     8
#define  MAXCOL     8

// Declare Enum type for directions so that code is easier to read
enum Directions { None, Up, Down, Right, Left };

// Function prototypes
void setup();
void print_instruction();
int  read_buttons();
void move_paddle( int dir, int *paddle, unsigned char buffer[]  );
int  move_ball( int *xball, int *yball, int *dx, int *dy, unsigned char buffer[]  );
int check_collision( int ballX, int ballY, int paddle );
void show_missed( int ballX, int paddle, unsigned char buffer[] );
void update_display( unsigned char buffer[]  );


//-----------------------------------------------------------------------------
// Main function

int main()
{
	unsigned char buffer[8] = {0};  	// Display buffer
	int  paddle, ballX, ballY, dx = 1, dy = 1;
	int  i, missed, hit, gameover = 0;
	enum Directions dir;
	
	setup();
	print_instruction();
	
	// Main loop
	while(1) {
		ballX = 5;
		ballY = 0;
		paddle = 3;
		
		while(1) {
			dir = read_buttons();
			move_paddle( dir, &paddle, buffer );
			missed = move_ball( &ballX, &ballY, &dx, &dy, buffer );
			update_display( buffer );
			if( missed ) {
				gameover = 1;
				break;
			}
			hit = check_collision( ballX, ballY, paddle );
			if( hit ) dy = -1;				// Bounce ball back up
			delay(200);
		}
		if( gameover ) {
			show_missed( ballX, paddle, buffer );
			gameover = 0;
		}
	}
	
	return 0;
}


//-----------------------------------------------------------------------------
// Function: Move paddle

void move_paddle( int dir, int *paddle, unsigned char buffer[]  )
{
	//unsigned char erase = ~buffer[7];	
	//buffer[7] = buffer[7] | erase;
	
	switch( dir ) {
		case Left:
					if( *paddle > 0 ) (*paddle)--;
					break;
		case Right:
					if( *paddle < 5 ) (*paddle)++;
					break;
	}
	buffer[7] = (0b11100000 >> *paddle);
	//buffer[7] = buffer[7] | (0b11100000 >> *paddle);
}


//-----------------------------------------------------------------------------
// Function: Move ball

int move_ball( int *xball, int *yball, int *dx, int *dy, unsigned char buffer[]  )
{
	int x = *xball, y = *yball, ex = *dx, ey = *dy;

	// Erase old ball
	buffer[y] = 0;
	
	// Compute new  ball position
	x = x + ex;
	if( x == 0  || x == 7 ) ex = -ex;
	y = y + ey;
	if( y == 0 ) ey = -ey;
	//if( y == 0  || y == 7 ) ey = -ey;
	
	// Place new ball
	buffer[y] = buffer[y] | (128 >> x);
	
	*xball = x; *yball = y;
	*dx = ex; *dy = ey;
	if( y == 7 ) return 1;           // Paddle missed ball
	else return 0;
}	


//-----------------------------------------------------------------------------
// Function: Check for collison of paddle vs ball

int check_collision( int ballX, int ballY, int paddle )
{
	if( ballY == 6 ) {			// Ball at almost bottom row
		if( ballX==paddle || ballX==paddle+1 || ballX==paddle+2 )
			return 1;
		//else if( ballX==paddle-1 ) {
		//	ballX = paddle-1;
		//	return 0;
		//}
		//else if( ballX==paddle+1 ) {
		//	ballX = paddle+1;
		//	return 0;
		//}
		else return 0;
	}
	else
		return 0;
}


//-----------------------------------------------------------------------------
// Function: Show that paddle missed ball

void show_missed( int ballX, int paddle, unsigned char buffer[] )
{
	int i;
		
	for(i = 0; i<10; i++ ) { 
		buffer[7] = 0b11100000 >> paddle;
		update_display( buffer );
		delay(100);
		if( ballX==paddle || ballX==paddle+2 ) {
			buffer[7] = 0b11100000 >> paddle;
			buffer[7] = buffer[7] & ~(128 >> ballX);
		}	
		else {
			buffer[7] = 0b11100000 >> paddle;
			buffer[7] = buffer[7] | (128 >> ballX);
		}
		update_display( buffer );
		delay(100);
	}
	delay(2000);
}


//-----------------------------------------------------------------------------
// Function: Update display

void update_display( unsigned char buffer[]  )
{
	int  r;

	for( r = 0; r < MAXROW; r++ ) {
		MAX7219write( r+1, buffer[r] );
	}
}	


//-----------------------------------------------------------------------------
// Function: Setup WiringPi, pin modes and LED matrix display

void setup()
{
	int  row;

	wiringPiSetupPhys();	
	pinMode(BUTTON_UP, INPUT);
	pinMode(BUTTON_DN, INPUT);
	pinMode(BUTTON_RT, INPUT);
	pinMode(BUTTON_LF, INPUT);
		
	init_ledmatrix();						// Initialize LED matrix
	for( row = 1; row < 9; row++ ) {   		// OFF all LEDs
		MAX7219write( row, 0);
	}
	initBuffer();							// Clear LED matrix buffer
}


//-----------------------------------------------------------------------------
// Function: Display instruction

void print_instruction()
{
	printf("\nSwitch ON #5,#6,#7 ,#8 @ LEFT selection switches.\n");
	printf("Press the UP / DOWN / LEFT / RIGHT button.\n");
	printf("Press Ctrl-C to EXIT.\n");
}


//-----------------------------------------------------------------------------
// Function: Read directional buttons, return direction value

int read_buttons()
{
	enum Directions dir;
	
	if( !digitalRead(BUTTON_UP) ) dir = Up;
	else if( !digitalRead(BUTTON_DN) ) dir = Down;
	else if( !digitalRead(BUTTON_RT) ) dir = Right;
	else if( !digitalRead(BUTTON_LF) ) dir = Left;
	else dir = None;
	
	return dir;
}
	
