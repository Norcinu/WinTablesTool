#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define ONE_POUND_GAME
//#define TWO_POUND_GAME

const int TOTAL_WINLINES	= 10;
const int TOTAL_REELS		= 5;
const int TOTAL_COLUMNS		= 3;

enum {PUFF, CRAB, SHARK, KING, SHELL, JACK, WONGA, TEN, LOBSTER, ACE, STARFISH, DIVER, QUEEN};

const int WIN_ARRAY_SIZE = 50000 / 10;
int WinResultArray[WIN_ARRAY_SIZE] = {0};
int ReelScreen[5][3];


#if defined ONE_POUND_GAME

int WinValueTable[5][3] = {
	50, 200, 1000,	 // 10
	100, 300, 1500,	 // Jack
	150, 500, 2000,  // Queen
	200, 1000, 2500, // King
	300, 1500, 5000  // Ace
};

#elif defined TWO_POUND_GAME

int WinValueTable[5][3] = {
	100, 400, 2000,	 // 10
	100, 600, 3000,	 // Jack
	300, 1000, 4000,  // Queen
	400, 2000, 5000, // King
	600, 3000, 5000  // Ace
};

#endif

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

struct TestSymbol
{
	unsigned char _r1, _r2, _r3, _r4, _r5;
	TestSymbol(unsigned char r1, unsigned char r2, unsigned char r3, unsigned char r4, unsigned char r5) :
	_r1(r1), _r2(r2), _r3(r3), _r4(r4), _r5(r5) {}
};

enum EType {THREE, FOUR, FIVE};

struct ArrayPosition 
{
	EType _type;
	int pos1, pos2, pos3, pos4, pos5;
	ArrayPosition() : pos1(0), pos2(0), pos3(0), pos4(0), pos5(0) {}
};

bool IsAWin(const TestSymbol* t, const ArrayPosition* p);
bool FoundFeature(unsigned char r1, unsigned char r2, unsigned char r3);
bool IsFiveOfAKind(const TestSymbol* t);
bool IsFourOfAKind(const TestSymbol* t);
bool IsThreeOfAKind(const TestSymbol* t);

#endif