#include "better.h"

Better::Better(){
		name = "";
		payout = 0;
		for (int i = 0; i < NUM_HORSES_PER_RACE; i++)
			bets[i] = 0;
}
Better::Better(string bname){
		name = bname;
		payout = 0;
		for (int i = 0; i < NUM_HORSES_PER_RACE; i++)
			bets[i] = 0;
}
bool Better::operator<(Better b){
	if (name < b.name) return true;
	return false;
}
bool Better::operator<=(Better b){
	if (name <= b.name) return true;
	return false;
}
bool Better::operator>(Better b){
	if (name > b.name) return true;
	return false;
}
bool Better::operator>=(Better b){
	if (name >= b.name) return true;
	return false;
}
bool Better::operator=(Better b){
	if (name == b.name) return true;
	return false;
}
void Better::setName(string bname){
		name = bname;
}
string Better::getName(){
	return name;
}
enum HRErrorCode Better::addBet(int horse, int bet){
	if (horse < 0 || horse >= NUM_HORSES_PER_RACE){
			return HR_INVALID_HORSE;
	}
	bets[horse]+=bet;
	return HR_SUCCESS;
}

int Better::getBet(int horse){
	if (horse < 0 || horse >= NUM_HORSES_PER_RACE){
			return -1;
	}
	return bets[horse];
}

void Better::print(std::ostream * stm){
	(*stm) << std::left << std::setw(35) << name << "\t" ;
	for (int i=0; i<NUM_HORSES_PER_RACE; i++){
		(*stm) << "$" << std::setw(8) << bets[i];
	}
	(*stm) << std::endl;
}
void Better::setPayout(int x){
	payout = x;
}
int Better::getPayout(){
	return payout;
}