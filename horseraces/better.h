#ifndef __BETTER_H_
#define __BETTER_H_

#include <string>
#include <iostream>
#include <iomanip>
#include <mutex>

#include "constants.h"
#include "horseraceerrors.h"

//using namespace std;
using std::cout;
using std::cin;
using std::string;

class Better{
		string name;					//name of the better
		int bets [NUM_HORSES_PER_RACE];	//placed bets by horse
		int payout;
		//std::mutex mtx;

	public:
		Better();
		Better(string);
		bool operator<(Better);
		bool operator<=(Better);
		bool operator>(Better);
		bool operator>=(Better);
		bool operator=(Better);
		void setName(string);
		string getName();
		enum HRErrorCode addBet(int,int);
		int getBet(int);

		void print();
		void setPayout(int);
		int getPayout();

};



#endif