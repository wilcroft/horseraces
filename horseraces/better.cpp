#include "better.h"

Better::Better(){
		name = "";
		for (int i = 0; i < NUM_HORSES_PER_RACE; i++)
			bets[i] = 0;
}
Better::Better(string bname){
		name = bname;
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
int Better::addBet(int horse, int bet){
	if (horse < 0 || horse >= NUM_HORSES_PER_RACE){
			return -1;
	}
	bets[horse]+=bet;
	return 0;
}