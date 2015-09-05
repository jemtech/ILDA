#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "ildaNode.h"
#include "ildaFile.h"

struct colour {
	char red;
	char green;
	char blue;
};

struct statusCode {
	char blanking;
	char lastEntry;
};

struct coordinateData {
	int x;
	int y;
	int z;
	struct statusCode status;
	//color from color table
	char r;
	char g;
	char b;
};

// contains the actual colour table
struct colour *colourTable;

int readTwoByteInt(FILE *fp){
	char ch = fgetc(fp);
	int i = *(signed char *)(&ch);
	i *= 1 << CHAR_BIT;
	ch = fgetc(fp);
	i |= ch;
	return i;
}

/*
MSB 0
	Bit 0 is the "last point" bit. This bit is set to 0 for all points except the last point. A 1 indicates end of image data. This was done for compatibility with certain existing systems; note that a zero in bytes 25-26 (Total Points) is the official end-of-file indication.
	Bit 1 is the blanking bit. If this is a 0, then the laser is on (draw). If this is a 1, then the laser is off (blank). Note that all systems must write this bit, even if a particular system uses only bits 0-7 for blanking/colour information.
	Bits 2-7 are unassigned and should be set to 0 (reserved).
*/
struct statusCode readStatusCode(FILE *fp){
	char ch = fgetc(fp);
	struct statusCode status;
	status.lastEntry = (((ch & 0x80) >> 7) == 1);
	status.blanking = (((ch & 0x40) >> 6) == 1);
}

int distPerS = 100000;
float ILDA_AxisMax = 32768.0;
float ILDA_Colour_Max = 255.0;
void executeCoordCommand(struct coordinateData *data){
	float x = (*data).x / ILDA_AxisMax;
	float y = (*data).y / ILDA_AxisMax;
	if((*data).status.blanking = 0){
			float red = (*data).r / ILDA_Colour_Max;
			float green = (*data).g / ILDA_Colour_Max;
			float blue = (*data).b / ILDA_Colour_Max;
			setColour(red, green, blue);
	}else{
		setColour(0.0, 0.0, 0.0);
	}
	moveTo(x, y);
	//moveToSpeedLimit(x, y, distPerS);
}

void readRGBByColourIndex(FILE *fp, struct coordinateData *data){
		char colour = fgetc(fp);
		if(colourTable == NULL ){
			// no colour Tabel use default
			(*data).r = ILDA_DEFAULT_COLOUR_PALETTE[colour][0];
			(*data).g = ILDA_DEFAULT_COLOUR_PALETTE[colour][1];
			(*data).b = ILDA_DEFAULT_COLOUR_PALETTE[colour][2];
		}else{
			(*data).r = colourTable[colour].red;
			(*data).g = colourTable[colour].green;
			(*data).b = colourTable[colour].blue;
		}
}

/*
	0-1 	X coordinate 	A 16-bit binary twos complement (signed) number. Extreme left is -32768; extreme right is +32767. (All directions stated using front projection.)
	
	2-3 	Y coordinate 	A 16-bit binary twos complement (signed) number. Extreme bottom is -32768; extreme top is +32767.

	4-5 	Z coordinate 	A 16-bit binary twos complement (signed) number. Extreme rear (away from viewer; behind screen) is -32768; extreme front (towards viewer; in front of screen) is +32767.

	6 	Status code (MSB 0)
			Bit 0 is the "last point" bit. This bit is set to 0 for all points except the last point. A 1 indicates end of image data. This was done for compatibility with certain existing systems; note that a zero in bytes 25-26 (Total Points) is the official end-of-file indication.
			Bit 1 is the blanking bit. If this is a 0, then the laser is on (draw). If this is a 1, then the laser is off (blank). Note that all systems must write this bit, even if a particular system uses only bits 0-7 for blanking/colour information.
			Bits 2-7 are unassigned and should be set to 0 (reserved).

	7	ColourIndex
			0-255 indicate the point's colour number. This value is used as an index into a colour lookup table containing red, green and blue values. See ILDA Colour Lookup Table Header section for more information.
*/

void parse3DCoordData(FILE *fp, int numberEntries){

	for(int i = 0; i<numberEntries; i++){
		struct coordinateData data;
		data.x = readTwoByteInt(fp);
		data.y = readTwoByteInt(fp);
		data.z = readTwoByteInt(fp);
		data.status = readStatusCode(fp);
		readRGBByColourIndex(fp, &data);
		//execute
		executeCoordCommand(&data);
	}
}

/*
	0-1 	X coordinate
	2-3 	Y coordinate
	4 	Status code
	5	ColourIndex
*/
void parse2DCoordData(FILE *fp, int numberEntries){
	for(int i = 0; i<numberEntries; i++){
		struct coordinateData data;
		data.x = readTwoByteInt(fp);
		data.y = readTwoByteInt(fp);
		data.z = 0;
		data.status = readStatusCode(fp);
		readRGBByColourIndex(fp, &data);
		//execute
		executeCoordCommand(&data);
	}
}


/*	
	0 	Red value 	Intensity value of red. Value ranges from 0 (off) to 255 (full on).
	1 	Green value 	Intensity value of green.
	2 	Blue value 	Intensity value of blue.
*/
void parseColourPaletteData(FILE *fp, int numberEntries){
	struct colour newColourTable[numberEntries];
	for(int i = 0; i<numberEntries; i++){
		newColourTable[i].red = fgetc(fp);
		newColourTable[i].green = fgetc(fp);
		newColourTable[i].blue = fgetc(fp);
	}
	free(colourTable);
	colourTable = newColourTable;
}

/*
	0-1 	X coordinate
	2-3 	Y coordinate
	4-5 	z coordinate
	6 	Status code
	7	blue
			This value is the point’s blue color component.  A value of 0 indicates “zero brightness”
and a value of 255 indicates “maximum brightness”.
	8	green
			This value is the point’s green color component. A value of 0 indicates “zero brightness”
and a value of 255 indicates “maximum brightness”.
	9	red
			This value is the point’s red color component.  A value of 0 indicates “zero brightness”
and a value of 255 indicates “maximum brightness”.
*/
void parse3DCoordTrueColData(FILE *fp, int numberEntries){

	for(int i = 0; i<numberEntries; i++){
		struct coordinateData data;
		data.x = readTwoByteInt(fp);
		data.y = readTwoByteInt(fp);
		data.z = readTwoByteInt(fp);
		data.status = readStatusCode(fp);
		data.b = fgetc(fp);
		data.g = fgetc(fp);
		data.r = fgetc(fp);
		//execute
		executeCoordCommand(&data);
	}
}

/*
	0-1 	X coordinate
	2-3 	Y coordinate
	4 	Status code
	5	blue
	6	green
	7	red
*/
void parse2DCoordTrueColData(FILE *fp, int numberEntries){
	for(int i = 0; i<numberEntries; i++){
		struct coordinateData data;
		data.x = readTwoByteInt(fp);
		data.y = readTwoByteInt(fp);
		data.z = 0;
		data.status = readStatusCode(fp);
		data.b = fgetc(fp);
		data.g = fgetc(fp);
		data.r = fgetc(fp);
		//execute
		executeCoordCommand(&data);
	}
}

/*
	0 to 3 	Signature ("ILDA") 	This makes a positive identification that this is the start of a header section, it should be the ASCII characters "ILDA". It can also be used to quickly identify a file as a valid ILDA file since the first section of such a file must be a header section.

	4 to 6 	Not used 	Supposed to be set to 0

	7 	Format type 	This indicates the type of data section that will follow the header. This must be either 0 (3D coordinate section), 1 (2D coordinate section), or 2 (colour palette section). The structure of these is described in more detail later.

	8 to 15 	Name 	This 8 byte string indicates the name (if there is one) of the current frame for format type 0 and 1, and the palette name for format type 2. If no name is appropriate then all the bytes should be set to 0.

	16 to 23 	Company name 	The name of the company who created the frame or palette. If no name is appropriate then all the bytes should be set to 0.

	24 and 25 	Total number of entries in data section 	For coordinates this is the number of points in the following data section, for colour palettes it is the number of entries in the palette. Using this along with the known size for the data section entries allows a parsing program to skip over sections they weren't interested.

	26 and 27 	Current frame number 	For files that contain a number of frames, eg: library of graphical shapes, collection of colour palettes, or an animation sequence, this is the current number. It ranges from 0 up to one the total number of frames minus 1.

	28 and 29 	Total number of frames 	The total number of frames and is not used for colour palette format types. This is set to 0 in a "null header" to indicate the end of the ILDA file.

	30 	Scanner head 	Used for systems with multiple scanners or heads, otherwise set to 0 for the default device.

	31 	Not used 	Supposed to be set to
*/
void executeIldaFile(FILE *fp, char loop){
	if( fp == NULL ){
		perror("Error no file.\n");
		return;
	}
	term_nonblocking();
	printf("Type 's' to stop painting.");
	do{
		rewind(fp);
		int state = PARSE_STATE_SEARCHING_HEADER;
		unsigned int byteNr = 0;
		char dataType;
		char name[8];
		char companyName[8];
		int numberOfDataEntries;
		int number;
		int totalNumber;
		char scannerHead;
		int chTmp;
		while( ( chTmp = fgetc(fp) ) != EOF){
			char ch = chTmp;//actual byte
			if(state == PARSE_STATE_SEARCHING_HEADER){
				if(ch == HEADER_START[byteNr]){
					//match go next
					byteNr++;
					if(byteNr == 7){
						//header identified start parsing heder info
						state = PARSE_STATE_PARSING_HEADER;
					}
				}else{
					//no match
					byteNr = 0;
				}	
			} else if(state == PARSE_STATE_PARSING_HEADER){
				if(byteNr == 7){
					dataType = ch;
				}else if(byteNr < 16){
					name[byteNr-8] = ch;
				}else if(byteNr < 24){
					companyName[byteNr-16] = ch;
				}else if(byteNr == 24){
					numberOfDataEntries = 256*ch;
				}else if(byteNr == 25){
					numberOfDataEntries += ch;
				}else if(byteNr == 26){
					number = 256*ch;
				}else if(byteNr == 27){
					number += ch;
				}else if(byteNr == 28){
					totalNumber = 256*ch;
				}else if(byteNr == 29){
					totalNumber += ch;
				}else if(byteNr == 30){
					scannerHead = ch;
				}else if(byteNr == 31){
					//last (not used) header byte: Data beginns
					if(dataType == ILDA_3D_COORD_HEADER_TYPE){
						parse3DCoordData(fp, numberOfDataEntries);
					}else if(dataType == ILDA_2D_COORD_HEADER_TYPE){
						parse2DCoordData(fp, numberOfDataEntries);
					}else if(dataType == ILDA_COLOUR_PALETTE_HEADER_TYPE){
						parseColourPaletteData(fp, numberOfDataEntries);
					}else if(dataType == ILDA_3D_COORD_TRUE_COL_HEADER_TYPE){
						parse3DCoordTrueColData(fp, numberOfDataEntries);
					}else if(dataType == ILDA_2D_COORD_TRUE_COL_HEADER_TYPE){
						parse2DCoordTrueColData(fp, numberOfDataEntries);
					}else{
						printf("Ignorring unknowm Data Type: %i\n", (int) dataType);
					}
					//data is parsed start again
					state = PARSE_STATE_SEARCHING_HEADER;
					byteNr = -1;
				}
				byteNr++;
			}
		
		}
	int userIn = getchar();
	if(userIn == 's'){
		loop = 0;
	}
	}while(loop);
	term_reset();
}

void executeIldaFileByName(char fileName[], char loop){
	FILE *fp;
	fp = fopen(fileName,"r"); // read mode
	if( fp != NULL ){
		executeIldaFile(fp, loop);
		fclose(fp);
	}else{
		printf("---ERROR---\ncan't open file: \"%s\"\n\n", fileName);
	}
}
