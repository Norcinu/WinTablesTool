#include <iostream>
#include <ctime>
#include <cstdlib>

#include "Definitions.h"

bool featureFound = false;

namespace accum {
	static int accumulatedTotal;
};

void GetFivesWin( int sym1pos )
{
	using namespace accum;

	switch (sym1pos) {
	case KING: // King
		accumulatedTotal += WinValueTable[3][2]; break; // £25 
	case JACK: // Jack
		accumulatedTotal += WinValueTable[1][2]; break;
	case TEN: // Ten
		accumulatedTotal += WinValueTable[0][2]; break;
	case ACE: // Ace
		accumulatedTotal += WinValueTable[4][2]; break;
	case QUEEN: // Queen
		accumulatedTotal += WinValueTable[2][2]; break;
	default:
		accumulatedTotal += 0; break;
	}
}

void GetFoursWin(int sym)
{
	using namespace accum;
	switch (sym) {
	case KING: // King 3
		accumulatedTotal += WinValueTable[3][1]; break; // £10
	case JACK: // Jack 5
		accumulatedTotal += WinValueTable[1][1]; break;
	case TEN: // Ten 7
		accumulatedTotal += WinValueTable[0][1]; break;
	case ACE: // Ace 9
		accumulatedTotal += WinValueTable[4][1]; break;
	case QUEEN: // Queen 13
		accumulatedTotal += WinValueTable[2][1]; break;
	default:
		accumulatedTotal += 0; break;
	}
}

void GetThreesWin(int sym)
{
	using namespace accum;
	switch (sym) {
	case 3: // King
		accumulatedTotal += WinValueTable[3][0]; break; // £25 
	case 5: // Jack
		accumulatedTotal += WinValueTable[1][0]; break;
	case 7: // Ten
		accumulatedTotal += WinValueTable[0][0]; break;
	case 9: // Ace
		accumulatedTotal += WinValueTable[4][0]; break;
	case 13: // Queen
		accumulatedTotal += WinValueTable[2][0]; break;
	default:
		accumulatedTotal += 0; break;				
	}
}

int CheckForWin() 
{
	using namespace accum;

	int rr = 0; // reel row
	int rc = 0; // reel column
	int wlc = 0; // winline column
	int winTableSymbol = 0;
	int featureValue = 0;
	
	accumulatedTotal = 0;

	//if (isfeature) return -1;
	if (accumulatedTotal != 0) accumulatedTotal = 0;
	
	for (int i = 0; i < TOTAL_WINLINES; ++i) {		
		int temprr;
		if (wlc == 1) 
			temprr = rr+1;
		if (wlc == 2)
			temprr = rr+2;

		int sym1pos = WinLines[i][0];
		int sym2pos = WinLines[i][1];
		int sym3pos = WinLines[i][2];
		int sym4pos = WinLines[i][3];
		int sym5pos = WinLines[i][4];
		
		int ctr = 0;
		
		TestSymbol symbols(sym1pos, sym2pos, sym3pos, sym4pos, sym5pos);
		ArrayPosition positions;
		if (IsFiveOfAKind(&symbols, &positions)) {
			GetFivesWin(sym1pos);
			continue;
		}
				
		// four from the left
		if (IsFourOfAKind(&symbols, &positions)) {
			GetFoursWin(sym1pos);
			continue;
		}
		
		symbols._r1 = sym2pos; 
		symbols._r2 = sym3pos; 
		symbols._r3 = sym4pos; 
		symbols._r4 = sym5pos;
		if (IsFourOfAKind(&symbols, &positions)) {
			GetFoursWin(sym2pos);
			continue;
		}

		// 3 from the left
		symbols._r1 = sym1pos; 
		symbols._r2 = sym2pos; 
		symbols._r3 = sym3pos;
		if (IsThreeOfAKind(&symbols, &positions)) {
			GetThreesWin(sym1pos);
			continue;
		}

		// three from centre
		symbols._r1 = sym2pos; 
		symbols._r2 = sym3pos; 
		symbols._r3 = sym4pos;
		if (IsThreeOfAKind(&symbols, &positions)) {
			GetThreesWin(sym2pos);
			continue;
		}

		// three from the right
		symbols._r1 = sym3pos;
		symbols._r2 = sym4pos;
		symbols._r3 = sym5pos;
		if (IsThreeOfAKind(&symbols, &positions)) {

		}
	}

	// set featureFound flag if we find a feature,.
	return accumulatedTotal;
}

void PickReels()
{
	int index = rand() % 36 + 1;
	int reel = 0;
	int reel_position = 0;
	
	for (; reel < 5; reel++){
		ReelScreen[reel][reel_position] = Symbols[index-1];
		ReelScreen[reel][++reel_position] = Symbols[index];
		ReelScreen[reel][++reel_position] = Symbols[index+1];
		
		index = rand() % 36 + 1;
		reel_position = 0;
	}
}

bool IsAWin(const TestSymbol* t, const ArrayPosition* p)
{
	switch (p->_type) {
	case THREE:
		return IsThreeOfAKind(t, p);
	case FOUR:
		return IsFourOfAKind(t, p);
	case FIVE:
		return IsFiveOfAKind(t, p);
	default:
		return false;
	}
}
//bool IsFiveOfAKind(unsigned char r1, unsigned char r2, unsigned char r3, 
//				   unsigned char r4, unsigned char r5)
bool IsFiveOfAKind(const TestSymbol* t, const ArrayPosition* p)
{
	return ReelScreen[t->_r1][0] == ReelScreen[t->_r2][1] && 
		   ReelScreen[t->_r2][1] == ReelScreen[t->_r3][2] &&
		   ReelScreen[t->_r3][2] == ReelScreen[t->_r4][3] &&
		   ReelScreen[t->_r4][3] == ReelScreen[t->_r5][4];
}

bool IsFourOfAKind(const TestSymbol* t, const ArrayPosition* p)
{
	return ReelScreen[t->_r1][0] == ReelScreen[t->_r2][1] && 
		   ReelScreen[t->_r2][1] == ReelScreen[t->_r3][2] &&
		   ReelScreen[t->_r3][2] == ReelScreen[t->_r4][3];
}

bool IsThreeOfAKind(const TestSymbol* t, const ArrayPosition* p)
{
	return ReelScreen[t->_r1][0] == ReelScreen[t->_r2][1] && 
		   ReelScreen[t->_r2][1] == ReelScreen[t->_r3][2];
}

int main(int argc, char *argv[]) {
	srand(time(NULL));

	int cycle = 1000000;
	do {
		cycle--;
		PickReels();

		// have we found a feature?
		while (featureFound) {
			PickReels();
			featureFound = false;
		}

		WinResultArray[CheckForWin() / 10];
	
	} while (cycle);
	
	
	while (cycle < WIN_ARRAY_SIZE) {
		cycle++;
		if (WinResultArray[cycle])
			std::cout << WinResultArray[cycle] << ": ";
	}

	std::cout << std::endl;
	return 0;
}
