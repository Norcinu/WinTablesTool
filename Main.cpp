#include <iostream>
#include <ctime>
#include <cstdlib>

#include "Definitions.h"

bool featureFound = false;

int CheckForWin() 
{
	int rr = 0; // reel row
	int rc = 0; // reel column
	int wlc = 0; // winline column
	int winTableSymbol = 0;
	int featureValue = 0;
	
	static int acccumulatedTotal = 0;

	//if (isfeature) return -1;

	for (int i = 0; i < TOTAL_WINLINES; i++) {		
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
		if (ReelScreen[i+sym1pos][ctr] == ReelScreen[i+sym2pos][++ctr] && 
			ReelScreen[i+sym2pos][ctr] == ReelScreen[i+sym3pos][++ctr] && 
			ReelScreen[i+sym3pos][ctr] == ReelScreen[i+sym4pos][++ctr] && 
			ReelScreen[i+sym4pos][ctr] == ReelScreen[i+sym5pos][++ctr]) {
			switch (sym1pos) {
			case 3: // King
				acccumulatedTotal += WinValueTable[3][2]; break; // £25 
			case 5: // Jack
				acccumulatedTotal += WinValueTable[1][2]; break;
			case 7: // Ten
				acccumulatedTotal += WinValueTable[0][2]; break;
			case 13: // Queen
				acccumulatedTotal += WinValueTable[2][2]; break;
			case 9: // Ace
				acccumulatedTotal += WinValueTable[4][2]; break;
			}

			continue;

			// Four of a kind.
			ctr = 0;
			if (ReelScreen[i+sym1pos][ctr] == ReelScreen[i+sym2pos][++ctr] && 
				ReelScreen[i+sym2pos][ctr] == ReelScreen[i+sym3pos][++ctr] && 
				ReelScreen[i+sym3pos][ctr] == ReelScreen[i+sym4pos][++ctr] ) {
				switch (sym1pos) {
				case 3: // King
					acccumulatedTotal += WinValueTable[3][2]; break; // £25 
				case 5: // Jack
					acccumulatedTotal += WinValueTable[1][2]; break;
				case 7: // Ten
					acccumulatedTotal += WinValueTable[0][2]; break;
				case 13: // Queen
					acccumulatedTotal += WinValueTable[2][2]; break;
				case 9: // Ace
					acccumulatedTotal += WinValueTable[4][2]; break;
				}
			}

			continue;

			// three of a kind
			ctr = 0;
			if (ReelScreen[i+sym1pos][ctr] == ReelScreen[i+sym2pos][++ctr] && 
				ReelScreen[i+sym2pos][ctr] == ReelScreen[i+sym3pos][++ctr]) {
				switch (sym1pos) {
				case 3: // King
					acccumulatedTotal += WinValueTable[3][2]; break; // £25 
				case 5: // Jack
					acccumulatedTotal += WinValueTable[1][2]; break;
				case 7: // Ten
					acccumulatedTotal += WinValueTable[0][2]; break;
				case 13: // Queen
					acccumulatedTotal += WinValueTable[2][2]; break;
				case 9: // Ace
					acccumulatedTotal += WinValueTable[4][2]; break;
				}
			}
		}
	}

	// set featureFound flag if we find a feature,.
	return 1;
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

bool IsFiveOfAKind(unsigned char r1, unsigned char r2, unsigned char r3, 
				   unsigned char r4, unsigned char r5)
{
	return r1 == r2 && r2 == r3 && r3 == r4 && r4 == r5;
}

bool IsFourOfAKind(unsigned char r1, unsigned char r2, unsigned char r3, 
				   unsigned char r4)
{
	return r1 == r2 && r2 == r3 && r3 == r4;
}

bool IsThreeOfAKind(unsigned char r1, unsigned char r2, unsigned char r3)
{
	return r1 == r2 && r2 == r3;
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