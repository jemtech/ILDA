#include <stdio.h>
#include <math.h>
#include <wiringPi.h>
#include "ltc2656.h"
#include "ildaNode.h"

void initILDA(){
	initLtc2656();
}

void endILDA(){
	closeLtc2656();
}

float lastX = 0.0;
float lastY = 0.0;

/*
mesured speed 0.15ms (~6.6kpps)
*/
void moveTo(float x, float y){
	lastX = x;
	lastY = y;
	setChVal_float(CH_X, x);
	setChVal_float(CH_Y, y);
	
	executeValues();
}

/*
min 150 micros (used to set the values)
*/
void moveToTimed(float x, float y, int micros){
	if(micros > MOVE_TIME_MICROS){
		int steps = micros / MOVE_TIME_MICROS;
		int remain = micros % MOVE_TIME_MICROS;
		float dx = (x - lastX) / steps;
		float dy = (y - lastY) / steps;
		//doing one step less
		for(int i = 1; i < steps; i++){
			moveTo(lastX + dx, lastY + dy);
		}
		//do last step
		delayMicroseconds(remain);
		moveTo(x, y);
	}else{
		moveTo(x, y);
	}
}

/*
distPerS is the distence per second (pps * 4)
*/
void moveToSpeedLimit(float x, float y, int distPerS){
	float dx = (x - lastX);
	float dy = (y - lastY);
	int micros = 0;
	if(dx > dy){
		micros = (dx / distPerS) * 1000000.0;
	}else{
		micros = (dy / distPerS) * 1000000.0;
	}
	moveToTimed(float x, float y, micros);
}

/*
r radius of the cicle
posX and posY center of the cicle
*/
void cicle(float r, float posX, float posY){
	for(float i =0.0; i < 360.0; i+=4){
		moveTo(sin(M_PI*i/180.0)*r+posX, cos(M_PI*i/180.0)*r+posY);
        }
}

void rotataitingCicle(){
	for(float i =0.0; i < 360.0; i+=20){
		cicle(0.2, sin(M_PI*i/180.0)*0.5, cos(M_PI*i/180.0)*0.5);
        }
} 

int HOUSE_WAIT_MICROS = 1000;
float HOUSE_SIZE = 0.2;
float ROOF_SIZE = 0.2; //invertete  (-1 is max 1 is min part of the house)
/*
	 ^
	/ \
	|X|
*/
void hoseOfNicolaus(){
	moveToTimed(-1.0 * HOUSE_SIZE, -1.0 * HOUSE_SIZE, HOUSE_WAIT_MICROS);

	moveToTimed(HOUSE_SIZE, -1.0 * HOUSE_SIZE, HOUSE_WAIT_MICROS);

	moveToTimed(-1.0 * HOUSE_SIZE, ROOF_SIZE * HOUSE_SIZE, HOUSE_WAIT_MICROS);

	moveToTimed(HOUSE_SIZE, ROOF_SIZE * HOUSE_SIZE, HOUSE_WAIT_MICROS);

	moveToTimed(0.0, HOUSE_SIZE, HOUSE_WAIT_MICROS);

	moveToTimed(-1.0 * HOUSE_SIZE, ROOF_SIZE * HOUSE_SIZE, HOUSE_WAIT_MICROS);

        moveToTimed(-1.0 * HOUSE_SIZE, -1.0 * HOUSE_SIZE, HOUSE_WAIT_MICROS);

	moveToTimed(HOUSE_SIZE, ROOF_SIZE * HOUSE_SIZE, HOUSE_WAIT_MICROS);

	moveToTimed(HOUSE_SIZE, -1.0 * HOUSE_SIZE, HOUSE_WAIT_MICROS);
}

int main(){
	initILDA();
	while(1){
		cicle(0.5, 0, 0);
		//hoseOfNicolaus();
		//rotataitingCicle();
	}
	endILDA();
	return 0;
}
