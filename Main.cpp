#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <cassert>

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
	case KING: // King
		accumulatedTotal += WinValueTable[3][0]; break; // £25 
	case JACK: // Jack
		accumulatedTotal += WinValueTable[1][0]; break;
	case TEN: // Ten
		accumulatedTotal += WinValueTable[0][0]; break;
	case ACE: // Ace
		accumulatedTotal += WinValueTable[4][0]; break;
	case QUEEN: // Queen
		accumulatedTotal += WinValueTable[2][0]; break;
	default:
	//	featureFound = true;
		accumulatedTotal += 0; break;				
	}
}

int CheckForWin() 
{
	using namespace accum;

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
			symbolValue = ReelScreen[0][sym1pos];
			if (symbolValue < ACE)
				GetFivesWin(symbolValue);
		} 
		else if (IsFourOfAKind(0, &symbols)) {
			symbolValue = ReelScreen[0][sym1pos];
			if (symbolValue < ACE)
				GetFoursWin(symbolValue);
		}
		else if (IsFourOfAKind(1, &symbols)) {
			symbolValue = ReelScreen[1][sym2pos];
			if (symbolValue < ACE)
				GetFoursWin(symbolValue);
		}
		else if (IsThreeOfAKind(0, &symbols)) {
			symbolValue = ReelScreen[0][sym1pos];
			if (symbolValue < ACE)
				GetThreesWin(symbolValue);
		}
		else if (IsThreeOfAKind(1, &symbols)) {
			symbolValue = ReelScreen[1][sym2pos];
			if (symbolValue <= ACE)
				GetThreesWin(symbolValue);
		}
		else if (IsThreeOfAKind(2, &symbols)) {
			symbolValue = ReelScreen[2][sym3pos];
			if (symbolValue <= ACE)
				GetThreesWin(symbolValue);
		}
	}

	// set featureFound flag if we find a feature,.
	int total = accumulatedTotal;
	accumulatedTotal = 0;
	return total;
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
		return IsThreeOfAKind(0, t);
	case FOUR:
		return IsFourOfAKind(0,t);
	case FIVE:
		return IsFiveOfAKind(t);
	default:
		return false;
	}
}

bool IsFiveOfAKind(const TestSymbol* t)
{
	assert (t != NULL);
	return ReelScreen[0][t->_r1] == ReelScreen[1][t->_r2] && 
		   ReelScreen[1][t->_r2] == ReelScreen[2][t->_r3] &&
		   ReelScreen[2][t->_r3] == ReelScreen[3][t->_r4] &&
		   ReelScreen[3][t->_r4] == ReelScreen[4][t->_r5];
}

bool IsFourOfAKind(int index, const TestSymbol* t)
{
	assert (t != NULL);
	if (index == 0) {
		return ReelScreen[0][t->_r1] == ReelScreen[1][t->_r2] && 
        	   ReelScreen[1][t->_r2] == ReelScreen[2][t->_r3] &&
			   ReelScreen[2][t->_r3] == ReelScreen[3][t->_r4];
	}
	else {
		return ReelScreen[1][t->_r2] == ReelScreen[2][t->_r3] && 
			   ReelScreen[2][t->_r3] == ReelScreen[3][t->_r4] &&
			   ReelScreen[3][t->_r4] == ReelScreen[4][t->_r5];
	}
}

bool IsThreeOfAKind(int index, const TestSymbol* t)
{
	assert (t != NULL);
	
	if (index == 0) {
		return ReelScreen[0][t->_r1] == ReelScreen[1][t->_r2] && 
			   ReelScreen[1][t->_r2] == ReelScreen[2][t->_r3];
	}
	else if (index == 1) {
		return ReelScreen[1][t->_r2] == ReelScreen[2][t->_r3] && 
			   ReelScreen[2][t->_r3] == ReelScreen[3][t->_r4];
	}
	else {
		return ReelScreen[2][t->_r3] == ReelScreen[3][t->_r4] && 
			   ReelScreen[3][t->_r4] == ReelScreen[4][t->_r5];
	}
}

int main(int argc, char *argv[]) {
	srand(time(NULL));

	int counter = 0;
	int cycle = 20000000; //20000000;
	
	char *filename;
	
#if defined ONE_POUND_GAME
	filename = "WinResults_1PD.txt";
#elif defined TWO_POUND_GAME
	filename = "WinResults_2PD.txt";
#endif
	
	std::ofstream file(filename);
	file << "Cycle size: " << cycle << "\n";
	file << "Prize" << std::setw(8) << std::setiosflags(std::ios::right) << "Count" << "\n" << "\n";

	do {
		cycle--;
		PickReels();

		WinResultArray[CheckForWin() / 10]++;
	
	} while (cycle);

	cycle = 0;	

	while (cycle < WIN_ARRAY_SIZE) {
		cycle++;

		if (WinResultArray[cycle]) {
			int width;
			(cycle < 10) ? width = 9 : width = 8;
			
			if (cycle >= 100) width = 7;
			if (cycle >= 1000) width = 6;

			file << "[" << cycle << "]" << std::setiosflags(std::ios::right) << 
			std::setw(width) << WinResultArray[cycle] << "\n";
		}
	}

	file.close();

	return 0;
}
