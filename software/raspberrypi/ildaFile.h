#if !defined(_ILDAFILE_H)
#define _ILDAFILE_H 1

static const int PARSE_STATE_SEARCHING_HEADER = -1; //Starts with (7 Bytes): I,L,D,A,0x00,0x00,0x00
static const int PARSE_STATE_PARSING_HEADER = 0;

static const char ILDA_3D_COORD_HEADER_TYPE = 0x00;
static const char ILDA_2D_COORD_HEADER_TYPE = 0x01;
static const char ILDA_COLOUR_PALETTE_HEADER_TYPE = 0x02;
static const char HEADER_START[] = {"I","L","D","A",0x00,0x00,0x00};

#endif
