#include "horse.h"

Horse::Horse(){
	name = "";
	bets = 0;
	odds = 0;
	isWinner = false;
}

Horse::Horse(std::string newname){
	name = newname;
	bets = 0;
	odds = 0;
	isWinner = false;
}

Horse::~Horse(){
	while (betters.begin() != betters.end()){
		betters.pop_front();
	}
}

std::string Horse::getName(){
	return name;
}
void Horse::setName(std::string newname){
	name = newname;
}
int Horse::getOdds(){
	
	return odds;
}
void Horse::addBet(Better * ptr, int bet){
	betters.push_back(ptr);
	bets += bet;
}