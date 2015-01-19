#ifndef __HORSERACE_H_
#define __HORSERACE_H_

#include <list>
#include <string>

#include "race.h"
#include "better.h"
#include "constants.h"
#include "horseraceerrors.h"

using namespace std;

class Horserace{
		Race * race [NUM_RACES];
		list <string> participants;
		int activeRace;

	public:
		Horserace();
		Horserace(list<string>);
		~Horserace();
		void addParticipant(string);
		list<string> getParticipants ();
		enum HRErrorCode setActiveRace(int);
		int getActiveRace();
		void setNoActiveRace();

		enum HRErrorCode setHorseNameActive(int,string);
		string getHorseNameActive(int, enum HRErrorCode * err = nullptr);
		int getHorseOddsActive(int, enum HRErrorCode * err = nullptr);
		enum HRErrorCode updateOddsActive();
		enum HRErrorCode setWinnerActive(int);
		enum HRErrorCode setHouseTakeActive(float);

		enum HRErrorCode setHorseName(int,int, string);
		string getHorseName(int, int, enum HRErrorCode * err = nullptr);
		int getHorseOdds(int, int, enum HRErrorCode * err = nullptr);
		enum HRErrorCode updateOdds(int);
		enum HRErrorCode setWinner(int, int);
		enum HRErrorCode setHouseTake(int, float);
		
		enum HRErrorCode addBetActive(string, int, int);
		enum HRErrorCode addBet(int,string,int,int);

		list <Better> getBetterListActive(enum HRErrorCode * err = nullptr);
		list <Better> getBetterList(int, enum HRErrorCode * err = nullptr);


};

#endif