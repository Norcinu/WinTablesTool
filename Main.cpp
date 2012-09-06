#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iomanip>

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
	//	featureFound = true;
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
	//	featureFound = true;
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
	//	featureFound = true;
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
	
	//accumulatedTotal = 0;

	//if (isfeature) return -1;
	//if (accumulatedTotal != 0) accumulatedTotal = 0;
	
	for (int i = 0; i < TOTAL_WINLINES; ++i) {		
		int sym1pos = WinLines[i][0];
		int sym2pos = WinLines[i][1];
		int sym3pos = WinLines[i][2];
		int sym4pos = WinLines[i][3];
		int sym5pos = WinLines[i][4];
		
		int symbolValue = 0;
		
		// need to get what the symbol in that position is. Currently passing in 0-2.
		TestSymbol symbols(sym1pos, sym2pos, sym3pos, sym4pos, sym5pos);
		if (IsFiveOfAKind(&symbols)) {
			//std::cout << "isfives: ";
			symbolValue = ReelScreen[i][sym1pos];
			GetFivesWin(symbolValue);
			continue;
		}
		
		// four from the left
		if (IsFourOfAKind(&symbols)) {
			//std::cout << "isfours1: ";
			symbolValue = ReelScreen[i][sym1pos];
			GetFoursWin(symbolValue);
			continue;
		}
		
		symbols._r1 = sym2pos; 
		symbols._r2 = sym3pos; 
		symbols._r3 = sym4pos; 
		symbols._r4 = sym5pos;
		if (IsFourOfAKind(&symbols)) {
			//std::cout << "isfours2: ";
			symbolValue = ReelScreen[i][sym2pos];
			GetFoursWin(symbolValue);
			continue;
		}

		// 3 from the left
		symbols._r1 = sym1pos; 
		symbols._r2 = sym2pos; 
		symbols._r3 = sym3pos;
		if (IsThreeOfAKind(&symbols)) {
			//std::cout << "isthrees1: ";
			symbolValue = ReelScreen[i][sym1pos];
			GetThreesWin(symbolValue);
			continue;
		}

		// three from centre
		symbols._r1 = sym2pos; 
		symbols._r2 = sym3pos; 
		symbols._r3 = sym4pos;
		if (IsThreeOfAKind(&symbols)) {
			//std::cout << "isthrees2: ";
			symbolValue = ReelScreen[i][sym2pos];
			GetThreesWin(symbolValue);
			continue;
		}

		// three from the right
		symbols._r1 = sym3pos;
		symbols._r2 = sym4pos;
		symbols._r3 = sym5pos;
		if (IsThreeOfAKind(&symbols)) {
			//std::cout << "isthrees3: ";
			symbolValue = ReelScreen[i][sym3pos];
			GetThreesWin(symbolValue);
			continue;
		}
	}

	// set featureFound flag if we find a feature,.
	int total = accumulatedTotal;
	accumulatedTotal = 0;
	if (total > 0)
		return total; 
	
	return 0;
	//return total;
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
		return IsThreeOfAKind(t);
	case FOUR:
		return IsFourOfAKind(t);
	case FIVE:
		return IsFiveOfAKind(t);
	default:
		return false;
	}
}

bool IsFiveOfAKind(const TestSymbol* t)
{
	return ReelScreen[0][t->_r1] == ReelScreen[1][t->_r2] && 
		   ReelScreen[1][t->_r2] == ReelScreen[2][t->_r3] &&
		   ReelScreen[2][t->_r3] == ReelScreen[3][t->_r4] &&
		   ReelScreen[3][t->_r4] == ReelScreen[4][t->_r5];
}

bool IsFourOfAKind(const TestSymbol* t)
{
	return ReelScreen[0][t->_r1] == ReelScreen[1][t->_r2] && 
		   ReelScreen[1][t->_r2] == ReelScreen[2][t->_r3] &&
		   ReelScreen[2][t->_r3] == ReelScreen[3][t->_r4];
}

bool IsThreeOfAKind(const TestSymbol* t)
{
	return ReelScreen[0][t->_r1] == ReelScreen[1][t->_r2] && 
		   ReelScreen[1][t->_r2] == ReelScreen[2][t->_r3];
}

int main(int argc, char *argv[]) {
	srand(time(NULL));

	int counter = 0;
	int cycle = 1000000;
	do {
		cycle--;
		PickReels();

		// have we found a feature?
		//while (featureFound) {
		//	PickReels();
		//	featureFound = false;
		//}

		int win = CheckForWin();// / 10;
		if (win > 0) {
			WinResultArray[counter] = win;
			counter++;
		}
	
	} while (cycle && counter < WIN_ARRAY_SIZE-1);
	
	cycle = 0;

	std::ofstream file("WinResults.txt");

	while (cycle < WIN_ARRAY_SIZE) {
		cycle++;
		if (WinResultArray[cycle])
			if (cycle == 1)
				file << std::setw(0) <<  WinResultArray[cycle];// << ": ";
			else
				file << std::setw(5) << WinResultArray[cycle];
	}

	file.close();

	return 0;
}
