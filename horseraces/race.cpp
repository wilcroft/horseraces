#include "race.h"

Race::Race(){
	int i;
	totalBets = 0;
	winner = -1;
	houseTake = (float)DEFAULT_HOUSEPCT;
	houseWinnings = 0;
	for (i=0; i < NUM_HORSES_PER_RACE; i++){
		horses[i]= new Horse();
	}
}
Race::Race(list <string> names){
	int i;
	totalBets = 0;
	winner = -1;
	houseTake = (float)DEFAULT_HOUSEPCT;
	houseWinnings = 0;
	for (i = 0; i < NUM_HORSES_PER_RACE; i++){
		horses[i]= new Horse();
	}
	for(auto& x:names){
		Better * ptr;
		ptr = new Better(x);
		betters.push_back(*ptr);
	}
}

Race::~Race(){
	int i;
	for (i=0; i < NUM_HORSES_PER_RACE; i++){
		delete horses[i];
	}
	while (betters.begin() != betters.end()){
		betters.pop_front();
	}
}

void Race::updateOdds(){
	if (totalBets>0)
		for (int i=0; i < NUM_HORSES_PER_RACE; i++){
			if (horses[i]->bets > 0)
				horses[i]->odds = (int)floor((totalBets*(1-houseTake))/((float)horses[i]->bets));
			else
				horses[i]->odds = 100;
		}
}

enum HRErrorCode Race::setWinner(int x){
	if (x> NUM_HORSES_PER_RACE - 1 || x < -1)
		return HR_INVALID_HORSE;
	else {
		mtx.lock();
		if ( x != -1){
			winner = x;
			horses[x]->isWinner = true;
			updateOdds();
			for (auto& b: betters){
				b.setPayout(horses[x]->getOdds()*b.getBet(x));
			}
			houseWinnings = totalBets - horses[x]->getOdds()*horses[x]->bets;
		}
		else {
			winner = x;
			for (int i = 0; i < NUM_HORSES_PER_RACE; i++){
				horses[i]->isWinner = false;
			}
			updateOdds();
			for (auto& b: betters){
					b.setPayout(0);
			}
			houseWinnings = 0;
		}
		mtx.unlock();
		return HR_SUCCESS;
	}
}

bool Race::hasWinner(){
	if (winner != -1)
		return true;
	else
		return false;
}

int Race::getWinner(){
	return winner;
}

enum HRErrorCode Race::setHouseTake(float x){
	if (x < 0 || x >=1)
		return HR_INVALID_HOUSE_TAKE;
	houseTake = x;
	mtx.lock();
	updateOdds();
	mtx.unlock();
	return HR_SUCCESS;
}

float Race::getHouseTake(){
	return houseTake;
}

void Race::addBetter(string x){
	mtx.lock();
	betters.emplace_back(x);
	betters.sort();
	mtx.unlock();
}

list<Better> Race::getBetterList(){
	list<Better> temp;
	mtx.lock();
	temp = betters;
	mtx.unlock();
	return temp;
}

enum HRErrorCode Race::setHorseName(int x, string s){
	if (x < 0 || x >= NUM_HORSES_PER_RACE)
		return HR_INVALID_HORSE;
	horses[x]->setName(s);
	return HR_SUCCESS;
}

string Race::getHorseName (int x, enum HRErrorCode * err){
	if (x < 0 || x >= NUM_HORSES_PER_RACE){
		if (err != nullptr)
			(*err) = HR_INVALID_HORSE;
		return "";
	}
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
	return horses[x]->getName();
}

int Race::getHorseOdds (int x, enum HRErrorCode * err){
	if (x < 0 || x >= NUM_HORSES_PER_RACE){
		if (err != nullptr)
			(*err) = HR_INVALID_HORSE;
		return -1;
	}
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
	return horses[x]->getOdds();
}

enum HRErrorCode Race::addBet(string name, int h, int bet){
	if (h  < 0 || h >= NUM_HORSES_PER_RACE)
		return HR_INVALID_HORSE;

	if (bet < 0)
		return HR_INVALID_BET;
	
	list<Better>::iterator iter;

	mtx.lock();
	iter = betters.begin();
	while (iter != betters.end() && iter->getName()!=name) iter++;
	if (iter == betters.end()){
		betters.emplace_back(name);
		betters.sort();
		iter = betters.begin();
		while (iter != betters.end() && iter->getName()!=name) iter++;
	}

	iter->addBet(h,bet);
	horses[h]->addBet(&(*iter),bet);
	totalBets+=bet;
	updateOdds();
	mtx.unlock();
	return HR_SUCCESS;

}

int Race::getHouseWinnings(){
	return houseWinnings;
}