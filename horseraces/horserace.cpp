#include "horserace.h"

Horserace::Horserace(){
	int i;
	activeRace = -1;
	for (i=0; i<NUM_RACES; i++){
		race[i]=new Race();
	}

}
Horserace::Horserace(list<string> names){
	int i;
	activeRace = -1;
	participants = names;
	participants.sort();
	for (i=0; i<NUM_RACES; i++){
		race[i]=new Race(participants);
	}
}

Horserace::~Horserace(){
	int i;
	for (i=0;i<NUM_RACES; i++){
		delete race[i];
	}
}

void Horserace::addParticipant(string s){
	participants.push_back(s);
	participants.sort();
}

list<string> Horserace::getParticipants(){
	return participants;
}

enum HRErrorCode Horserace::setActiveRace(int x){
	if (x < 0 || x >= NUM_RACES){
		return HR_INVALID_RACE;
	}
	activeRace = x;
	return HR_SUCCESS;
}

int Horserace::getActiveRace(){
	return activeRace;
}

void Horserace::setNoActiveRace(){
	activeRace = -1;
}

/*
 *	Active-Race Methods
 */
enum HRErrorCode Horserace::setHorseNameActive(int x, string s){
	if (activeRace == -1)
		return HR_NO_ACTIVE_RACE;
	return setHorseName(activeRace,x,s);
}

string Horserace::getHorseNameActive(int x, enum HRErrorCode * err){
	if (activeRace == -1){
		if (err!=nullptr)
				(*err)=HR_NO_ACTIVE_RACE;
		return "";
	}
	return getHorseName(activeRace,x, err);
}
	
int Horserace::getHorseOddsActive(int x, enum HRErrorCode * err){
	if (activeRace == -1){
		if (err!=nullptr)
				(*err)=HR_NO_ACTIVE_RACE;
		return -1;
	}
	return getHorseOdds(activeRace,x, err);
}

enum HRErrorCode Horserace::updateOddsActive(){
	if (activeRace == -1)
		return HR_NO_ACTIVE_RACE;
	race[activeRace]->updateOdds();
	return HR_SUCCESS;
}
enum HRErrorCode Horserace::setWinnerActive(int x){
	if (activeRace == -1)
		return HR_NO_ACTIVE_RACE;
	return race[activeRace]->setWinner(x);
}

enum HRErrorCode Horserace::setHouseTakeActive(float f){
	if (activeRace == -1)
		return HR_NO_ACTIVE_RACE;
	return race[activeRace]->setHouseTake(f);
}



/*
 *	Race-related Methods
 */

enum HRErrorCode Horserace::setHorseName(int r, int x, string s){
	if (r < 0 || r >= NUM_RACES)
		return HR_INVALID_RACE;
	return race[r]->setHorseName(x,s);
}

string Horserace::getHorseName(int r, int x, enum HRErrorCode * err){
	if (r < 0 || r>=NUM_RACES){
		if (err !=nullptr)
			(*err) = HR_INVALID_RACE;
		return "";
	}

	return race[r]->getHorseName(x,err);
}

int Horserace::getHorseOdds(int r, int x, enum HRErrorCode * err){
	if (r < 0 || r>= NUM_RACES){
		if (err != nullptr)
			(*err) = HR_INVALID_RACE;
		return -1;
	}

	return race[r]->getHorseOdds(x,err);
}

enum HRErrorCode Horserace::addBetActive(string name, int h, int bet){
	if (activeRace == -1)
		return HR_NO_ACTIVE_RACE;

	return addBet(activeRace,name,h,bet);
}

enum HRErrorCode Horserace::addBet(int r, string name, int h, int bet){
	if (r < 0 || r >= NUM_RACES)
			return HR_INVALID_RACE;

	list<string>::iterator iter;

	iter=participants.begin();
	while (iter!=participants.end() && *iter!=name) iter++;
	if (iter == participants.end()) addParticipant(name);

	return race[r]->addBet(name, h, bet);		
}
	