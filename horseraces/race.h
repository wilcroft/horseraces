#ifndef __RACE_H_
#define __RACE_H_

#include <list>
#include <string>

#include "constants.h"
#include "horseraceerrors.h"
#include "horse.h"
#include "better.h"

using namespace std;

class Race{
	
		Horse * horses[NUM_HORSES_PER_RACE];
		long totalBets;
		float houseTake;
		int winner;
		list <Better> betters;
		
	public:
		Race();
		Race(list <string>);
		~Race();
		void updateOdds();
		enum HRErrorCode setWinner(int);
		bool hasWinner();
		enum HRErrorCode setHouseTake (float);
		void addBetter(string);
		enum HRErrorCode setHorseName (int, string);
		string getHorseName (int, enum HRErrorCode * err = nullptr);
		int getHorseOdds(int, enum HRErrorCode * err = nullptr);
		enum HRErrorCode addBet(string, int, int);
		
};
#endif