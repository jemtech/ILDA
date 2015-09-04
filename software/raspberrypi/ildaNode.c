#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPi.h>
#include <string.h>
#include "ltc2656.h"
#include "ildaFile.h"
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
	moveToTimed(x, y, micros);
}

/*
writes the values but not execute them
*/
void setColour(float red, float green, float blue){
	setChVal_float(CH_R, red);
	setChVal_float(CH_G, green);
	setChVal_float(CH_B, blue);
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

int selectWhatToDo(){
	int  number;
	printf("Type in the number to select:\n");
	printf("0: quit\n");
	printf("1: paint a cicle\n");
	printf("2: paint a ciceling cicle\n");
	printf("3: paint a house\n");
	printf("4: execute ILDA-file\n");
	scanf("%d", &number);
	return number;
}

struct termios stdin_orig;  // Structure to save parameters

void term_reset() {
        tcsetattr(STDIN_FILENO,TCSANOW,&stdin_orig);
        tcsetattr(STDIN_FILENO,TCSAFLUSH,&stdin_orig);
	int oldfl = fcntl(STDIN_FILENO, F_GETFL);
	if (oldfl == -1) {
	    /* handle error */
	}
	fcntl(STDIN_FILENO, F_SETFL, oldfl & ~O_NONBLOCK);
}

void term_nonblocking() {
        struct termios newt;
        if(tcgetattr(STDIN_FILENO, &stdin_orig) == -1){
		perror("could not back up terminal settings");
		return;
	}
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // non-blocking
        newt = stdin_orig;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        atexit(term_reset);
}

void cleanStdin() {
    int ch;
    while ((ch = fgetc(stdin)) != EOF && ch != '\n') {
        /* null body */;
    }
}

int main(){
	initILDA();
	int runMainLoop = 1;
	while(runMainLoop){
		int  selction = selectWhatToDo();
		if(selction == 0){
			runMainLoop = 0; //quiting
		}else if(selction < 1 || selction > 4){
			printf("%i is not a option.\n", selction);
			//runMainLoop = 0; //quiting
		}else if(selction == 4){
			char fileName[256];
			cleanStdin();
			printf("Please enter filename:\n");
			fgets(fileName, sizeof(fileName), stdin);
			char* fileNameTmp = strtok(fileName, "\n");
			printf("open file: \"%s\"\n", fileNameTmp);
			executeIldaFileByName(fileNameTmp);
		}else{
			term_nonblocking();
			printf("Type 's' to stop painting.");
			int runPaintLoop = 1;
			while(runPaintLoop){
				if(selction == 1){
					cicle(0.5, 0, 0);
				}else if(selction == 2){
					rotataitingCicle();
				}else if(selction == 3){
					hoseOfNicolaus();
				}
				int ch = getchar();
				if(ch == 's'){
					runPaintLoop = 0;
				}
			}
			term_reset();
		}
	}
	endILDA();
	return 0;
}
