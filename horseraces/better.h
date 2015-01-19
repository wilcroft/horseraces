#ifndef __BETTER_H_
#define __BETTER_H_

#include <string>
#include <iostream>
#include <iomanip>
#include "constants.h"
#include "horseraceerrors.h"

using namespace std;

class Better{
		string name;					//name of the better
		int bets [NUM_HORSES_PER_RACE];	//placed bets by horse

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
		int addBet(int,int);

		void print();

};



#endif