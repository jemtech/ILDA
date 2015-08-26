#if !defined(_ILDANODE_H)
#define _ILDANODE_H 1

#if !defined(M_PI)
#define M_PI 3.14159265358979323846
#endif

unsigned char CH_X  = 0x03;
unsigned char CH_Y  = 0x02;
unsigned char CH_R  = 0x01;
unsigned char CH_G  = 0x00;
unsigned char CH_B  = 0x04;
unsigned char CH_C1 = 0x05;

void initILDA();
/*
Set the positon in float values.
The value 0 is the center.
1 the max to the right on x and the max to top for y.
-1 max left on x and max down on y.
*/
void moveTo(float x, float y);

#endif
