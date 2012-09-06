#include <iostream>
#include <ctime>
#include <cstdlib>

#include "Definitions.h"

bool featureFound = false;

namespace accum {
	static int accumulatedTotal;
};

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

		// five of a kind
		if (ReelScreen[i+sym1pos][0] == ReelScreen[i+sym2pos][1] && 
			ReelScreen[i+sym2pos][1] == ReelScreen[i+sym3pos][2] && 
			ReelScreen[i+sym3pos][2] == ReelScreen[i+sym4pos][3] && 
			ReelScreen[i+sym4pos][3] == ReelScreen[i+sym5pos][4]) {
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

			continue;

			// Four of a kind.
			ctr = 0;
			if (ReelScreen[i+sym1pos][0] == ReelScreen[i+sym2pos][1] && 
				ReelScreen[i+sym2pos][1] == ReelScreen[i+sym3pos][2] && 
				ReelScreen[i+sym3pos][2] == ReelScreen[i+sym4pos][3] ) {
				switch (sym1pos) {
				case KING: // King 3
					accumulatedTotal += WinValueTable[3][2]; break; // £25 
				case JACK: // Jack 5
					accumulatedTotal += WinValueTable[1][2]; break;
				case TEN: // Ten 7
					accumulatedTotal += WinValueTable[0][2]; break;
				case ACE: // Ace 9
					accumulatedTotal += WinValueTable[4][2]; break;
				case QUEEN: // Queen 13
					accumulatedTotal += WinValueTable[2][2]; break;
				default:
					accumulatedTotal += 0; break;
				}
			}

			continue;

			// three of a kind
			ctr = 0;
			if (ReelScreen[i+sym1pos][c] == ReelScreen[i+sym2pos][++ctr] && 
				ReelScreen[i+sym2pos][ctr] == ReelScreen[i+sym3pos][++ctr]) {
				switch (sym1pos) {
				case 3: // King
					accumulatedTotal += WinValueTable[3][2]; break; // £25 
				case 5: // Jack
					accumulatedTotal += WinValueTable[1][2]; break;
				case 7: // Ten
					accumulatedTotal += WinValueTable[0][2]; break;
				case 9: // Ace
					accumulatedTotal += WinValueTable[4][2]; break;
				case 13: // Queen
					accumulatedTotal += WinValueTable[2][2]; break;
				default:
					accumulatedTotal += 0; break;				
				}
			}
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

bool IsAWin(const TestSymbol* t, const ArrayPosition* p) const
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
	
	
	while (cycle > WIN_ARRAY_SIZE) {
		cycle++;
		if (WinResultArray[cycle])
			std::cout << WinResultArray[cycle] << ": ";
	}

	std::cout << std::endl;
	return 0;
}