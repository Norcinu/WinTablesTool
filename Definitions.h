#ifndef DEFINITIONS_H
#define DEFINITIONS_H

const int TOTAL_WINLINES	= 10;
const int TOTAL_REELS		= 5;
const int TOTAL_COLUMNS		= 3;

enum {PUFF, CRAB, SHARK, KING, SHELL, JACK, WONGA, TEN, LOBSTER, ACE, STARFISH, DIVER, QUEEN};

const int WIN_ARRAY_SIZE = 50000 / 10;
int WinResultArray[WIN_ARRAY_SIZE] = {0};
int ReelScreen[5][3];

int WinValueTable[5][3] = {
	50, 200, 1000,	 // 10
	100, 300, 1500,	 // Jack
	150, 500, 2000,  // Queen
	200, 1000, 2500, // King
	300, 1500, 5000  // Ace
};

//int	WinValueTable[3][6] = {
//	/*0,*/ 50, 100, 150, 200, 300, // three of a kind
//	/*0,*/ 200, 300, 500, 1000, 1500, // four of a kind
//	/*0,*/ 1000, 1500, 2000, 2500, 5000 // five of a kind
//};

int WinLines[TOTAL_WINLINES][TOTAL_REELS] = {
	1,1,1,1,1,   // line 1
	0,0,0,0,0,	 // line 2
	2,2,2,2,2,   // line 3
	0,0,1,2,2,	 // line 4
	2,2,1,0,0,   // line 5 
	0,0,0,1,2,   // line 6
	2,2,2,1,0,	 // line 7
	2,1,0,0,0,   // line 8
	0,1,2,2,2,	 // line 9
	2,1,0,1,2,   // line 10
};

int Symbols[38] = 
{
	LOBSTER,
	PUFF,
	ACE, ACE, ACE,
	CRAB,
	KING, KING, KING,
	SHARK,
	QUEEN, QUEEN, QUEEN,
	SHELL,
	JACK, JACK, JACK,
	WONGA,
	TEN, TEN, TEN,
	QUEEN,
	LOBSTER,
	STARFISH,
	JACK,
	DIVER,
	TEN, 
	PUFF,
	JACK,
	CRAB,
	TEN,
	SHARK,
	JACK,
	SHELL,
	TEN,
	WONGA,
	LOBSTER,
	PUFF
};

bool FoundFeature(unsigned char r1, unsigned char r2, unsigned char r3);
bool IsFiveOfAKind(unsigned char r1, unsigned char r2, unsigned char r3, unsigned char r4, unsigned char r5);
bool IsFourOfAKind(unsigned char r1, unsigned char r2, unsigned char r3, unsigned char r4);
bool IsThreeOfAKind(unsigned char r1, unsigned char r2, unsigned char r3);

#endif