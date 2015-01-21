#ifndef __HORSE_H_
#define __HORSE_H_

#include <string>
#include <list>
#include "constants.h"
#include "horseraceerrors.h"

//using namespace std;

using std::list;
using std::string;

class Race;
class Better;

class Horse{
		friend Race;
		
		string name;
		long bets;
		int odds;
		bool isWinner;
		list <Better*> betters;
	
	public:
		Horse();
		Horse(std::string);
		~Horse();
		
		std::string getName();
		void setName(std::string);
		int getOdds();
		void addBet(Better*, int);
		
};	
#endif