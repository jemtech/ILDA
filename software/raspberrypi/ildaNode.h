#ifndef ILDANODE_H
#define ILDANODE_H

//chanal number for x movement
static const unsigned char CH_X  = 0x03;
//chanal number for y movement
static const unsigned char CH_Y  = 0x02;
//chanal number for color red
static const unsigned char CH_R  = 0x01;
//chanal number for color green
static const unsigned char CH_G  = 0x00;
//chanal number for color blue
static const unsigned char CH_B  = 0x04;
//chanal number for custom color 1
static const unsigned char CH_C1 = 0x05;
//time for one move in µs
static const int MOVE_TIME_MICROS = 150;

/*
run this before any opration
*/
void initILDA();
/*
Set the positon in float values.
The value 0 is the center.
1 the max to the right on x and the max to top for y.
-1 max left on x and max down on y.
executing takes MOVE_TIME_MICROS µs
*/
void moveTo(float x, float y);
/*
micros this moveTo should take
seperates the singel movement in multiple moveTo which take MOVE_TIME_MICROS µs
*/
void moveToTimed(float x, float y, int micros);
/*
calculates the move-time based on the distance and distPerS
distPerS is the distence per second (pps * 4)
*/
void moveToSpeedLimit(float x, float y, int distPerS);
/*
writes the values but not executes them.
0 is off and 1 full on.
with the "moveTo" functions the colour is also executed. 
*/
void setColour(float red, float green, float blue);

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif /*M_PI*/

#endif /*ILDANODE_H*/
