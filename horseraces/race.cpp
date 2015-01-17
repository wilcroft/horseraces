#include "race.h"

Race::Race(){
	totalBets = 0;
	winner = -1;
	houseTake = (float)0.15;
}
Race::Race(list <string> names){
	totalBets = 0;
	winner = -1;
	houseTake = (float)0.15;
	for(auto& x:names){
		betters.emplace_back(x);
	}
	
}

void Race::updateOdds(){
	if (totalBets>0)
		for (int i=0; i < NUM_HORSES_PER_RACE; i++){
			horses[i].odds = (int)floor((totalBets*(1-houseTake))/((float)horses[i].bets));
		}
		
}

enum HRErrorCode Race::setWinner(int x){
	if (x> NUM_HORSES_PER_RACE - 1 || x < 0)
		return HR_INVALID_HORSE;
	else {
		winner = x;
		horses [x].isWinner = true;
		return HR_SUCCESS;
	}
}

bool Race::hasWinner(){
	if (winner != -1)
		return true;
	else
		return false;
}

enum HRErrorCode Race::setHouseTake(float x){
	if (x < 0 || x >=1)
		return HR_INVALID_HOUSE_TAKE;
	houseTake = x;
	return HR_SUCCESS;
}

void Race::addBetter(string x){
	betters.emplace_back(x);
	betters.sort();
}

enum HRErrorCode Race::setHorseName(int x, string s){
	if (x < 0 || x >= NUM_HORSES_PER_RACE)
		return HR_INVALID_HORSE;
	horses[x].setName(s);
	return HR_SUCCESS;
}

string Race::getHorseName (int x, enum HRErrorCode * err){
	if (x < 0 || x >= NUM_HORSES_PER_RACE){
		if (err != nullptr)
			(*err) = HR_INVALID_HORSE;
		return "";
	}
	return horses[x].getName();
}

int Race::getHorseOdds (int x, enum HRErrorCode * err){
	if (x < 0 || x >= NUM_HORSES_PER_RACE){
		if (err != nullptr)
			(*err) = HR_INVALID_HORSE;
		return -1;
	}
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
	return horses[x].getOdds();
}

enum HRErrorCode Race::addBet(string name, int h, int bet){
	if (h  < 0 || h > NUM_HORSES_PER_RACE)
		return HR_INVALID_HORSE;

	if (bet < 0)
		return HR_INVALID_BET;
	
	list<Better>::iterator iter;

	iter = betters.begin();
	while (iter != betters.end() && iter->getName()!=name) iter++;
	if (iter == betters.end()){
		addBetter(name);
		iter = betters.begin();
		while (iter != betters.end() && iter->getName()!=name) iter++;
	}

	iter->addBet(h,bet);
	horses[h].addBet(&(*iter),bet);

	updateOdds();

	return HR_SUCCESS;

}