#ifndef __RACE_H_
#define __RACE_H_

#include <list>
#include <string>
#include <mutex>

#include "constants.h"
#include "horseraceerrors.h"
#include "horse.h"
#include "better.h"

#if defined(__linux__)
#include <math.h>
#endif

#define DEFAULT_HOUSEPCT 0.15

//using namespace std;
using std::cout;
using std::cin;
using std::list;
using std::string;

class Race{

		Horse * horses[NUM_HORSES_PER_RACE];
		long totalBets;
		float houseTake;
		int winner;
		int houseWinnings;
		list <Better> betters;
		std::mutex mtx;

	public:
		Race();
		Race(list <string>);
		~Race();
		void updateOdds();
		enum HRErrorCode setWinner(int);
		bool hasWinner();
		int getWinner();
		enum HRErrorCode setHouseTake (float);
		float getHouseTake();
		void addBetter(string);
		list<Better> getBetterList();
		enum HRErrorCode setHorseName (int, string);
		string getHorseName (int, enum HRErrorCode * err = nullptr);
		int getHorseOdds(int, enum HRErrorCode * err = nullptr);
		enum HRErrorCode addBet(string, int, int);
		int getHouseWinnings();

};
#endif
