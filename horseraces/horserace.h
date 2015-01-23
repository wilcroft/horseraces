#ifndef __HORSERACE_H_
#define __HORSERACE_H_

#include <list>
#include <string>
#include <mutex>

#include "race.h"
#include "better.h"
#include "constants.h"
#include "horseraceerrors.h"

//using namespace std;
using std::cout;
using std::cin;
using std::list;
using std::string;

class Horserace{
		Race * race [NUM_RACES];
		list <string> participants;
		int activeRace;
		std::mutex mtx;

	public:
		Horserace();
		Horserace(list<string>);
		~Horserace();
		void addParticipant(string);
		list<string> getParticipants ();
		enum HRErrorCode setActiveRace(int);
		int getActiveRace();
		void setNoActiveRace();
		int getHouseWinningsAll();
		enum HRErrorCode setHouseTakeAll(float);

		enum HRErrorCode setHorseNameActive(int,string);
		string getHorseNameActive(int, enum HRErrorCode * err = nullptr);
		int getHorseOddsActive(int, enum HRErrorCode * err = nullptr);
		enum HRErrorCode updateOddsActive();
		enum HRErrorCode setWinnerActive(int);
		int getWinnerActive(enum HRErrorCode * err = nullptr);
		enum HRErrorCode setHouseTakeActive(float);
		float getHouseTakeActive (enum HRErrorCode * err = nullptr);
		int getHouseWinningsActive (enum HRErrorCode * err = nullptr);
		enum HRErrorCode addBetActive(string, int, int);
		list <Better> getBetterListActive(enum HRErrorCode * err = nullptr);

		enum HRErrorCode setHorseName(int,int, string);
		string getHorseName(int, int, enum HRErrorCode * err = nullptr);
		int getHorseOdds(int, int, enum HRErrorCode * err = nullptr);
		enum HRErrorCode updateOdds(int);
		enum HRErrorCode setWinner(int, int);
		int getWinner(int, enum HRErrorCode * err = nullptr);
		enum HRErrorCode setHouseTake(int, float);
		float getHouseTake (int, enum HRErrorCode * err = nullptr);
		int getHouseWinnings (int, enum HRErrorCode * err = nullptr);
		enum HRErrorCode addBet(int,string,int,int);
		list <Better> getBetterList(int, enum HRErrorCode * err = nullptr);
		



};

#endif