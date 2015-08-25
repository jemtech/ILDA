#include <stdio.h>
#include <math.h>
#include <wiringPi.h>
#include "ltc2656.h"
#include "ildaNode.h"

void cicle(){
	float gerad = M_PI / 180.0;
	for(float i =0.0; i < 360.0; i++){
		unsigned char x = sin(gerad*i)*127.0 + 128;
                unsigned char y = cos(gerad*i)*127.0 + 128;

                setChVal(CH_X, x, 0);
                setChVal(CH_Y, y, 0);

                executeValues(); 
        }
}

int HOUSE_WAIT_MICOS = 1000;
int HOUSE_SIZE = 20;
int HOUSE_OFSET = 118;//0.5 * (255-HOUSE_SIZE)
void hoseOfNicolaus(){
	setChVal(CH_X, HOUSE_OFSET, 0);
        setChVal(CH_Y, HOUSE_OFSET, 0);
	setChVal(CH_C1, 0, 0);
        executeValues();
	delayMicroseconds(HOUSE_WAIT_MICOS);

	setChVal(CH_X, HOUSE_OFSET + HOUSE_SIZE, 0);
	setChVal(CH_C1, 150, 0);
	executeValues();
        delayMicroseconds(HOUSE_WAIT_MICOS);

	setChVal(CH_X, HOUSE_OFSET + 0, 0);
	setChVal(CH_Y, HOUSE_OFSET + 0.7*HOUSE_SIZE, 0);
	executeValues();
        delayMicroseconds(HOUSE_WAIT_MICOS);

	setChVal(CH_X, HOUSE_OFSET + HOUSE_SIZE, 0);
	setChVal(CH_C1, 0, 0);
	executeValues();
        delayMicroseconds(HOUSE_WAIT_MICOS);

	setChVal(CH_X, HOUSE_OFSET + 0.5*HOUSE_SIZE, 0);
	setChVal(CH_Y, HOUSE_OFSET + HOUSE_SIZE, 0);
	executeValues();
        delayMicroseconds(HOUSE_WAIT_MICOS);

	setChVal(CH_X, HOUSE_OFSET + 0, 0);
        setChVal(CH_Y, HOUSE_OFSET + 0.7*HOUSE_SIZE, 0);
        setChVal(CH_C1, 150, 0);
	executeValues();
        delayMicroseconds(HOUSE_WAIT_MICOS);

        setChVal(CH_Y, HOUSE_OFSET + 0, 0);
        executeValues();
        delayMicroseconds(1000);

	setChVal(CH_X, HOUSE_OFSET + HOUSE_SIZE, 0);
        setChVal(CH_Y, HOUSE_OFSET + 0.7 * HOUSE_SIZE, 0);
        executeValues();
        delayMicroseconds(HOUSE_WAIT_MICOS);

	setChVal(CH_Y, HOUSE_OFSET + 0, 0);
	executeValues();
        delayMicroseconds(HOUSE_WAIT_MICOS);
}

int main(){
	initLtc2656();
	while(1){
		//cicle();
		hoseOfNicolaus();
	}
	closeLtc2656();
	return 0;
}
