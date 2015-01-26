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

void Horserace::lock(){
	mtx.lock();
}

void Horserace::unlock(){
	mtx.unlock();
}

void Horserace::addParticipant(string s){
	mtx.lock();
	participants.push_back(s);
	participants.sort();
	mtx.unlock();
}

list<string> Horserace::getParticipants(){
	return participants;
}

enum HRErrorCode Horserace::setActiveRace(int x){
	if (x < 0 || x >= NUM_RACES){
		return HR_INVALID_RACE;
	}
	mtx.lock();
	activeRace = x;
	mtx.unlock();
	return HR_SUCCESS;
}

int Horserace::getActiveRace(){
	return activeRace;
}

void Horserace::setNoActiveRace(){
	mtx.lock();
	activeRace = -1;
	mtx.unlock();
}

int Horserace::getHouseWinningsAll(){
	int sum=0;
	for (int i = 0; i < NUM_RACES; i++){
		sum += race[i]->getHouseWinnings();
	}
	return sum;
}
enum HRErrorCode Horserace::setHouseTakeAll(float f){
	if (f < 0 || f >= 1){
		return HR_INVALID_HOUSE_TAKE;
	}
	else{
		for (int i = 0; i < NUM_RACES; i++)
			race[i]->setHouseTake(f);
	}
	return HR_SUCCESS;
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
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
	return getHorseName(activeRace,x, err);
}
	
int Horserace::getHorseOddsActive(int x, enum HRErrorCode * err){
	if (activeRace == -1){
		if (err!=nullptr)
				(*err)=HR_NO_ACTIVE_RACE;
		return -1;
	}
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
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

int Horserace::getWinnerActive(enum HRErrorCode * err){
	if (activeRace == -1){
		if (err!=nullptr)
				(*err)=HR_NO_ACTIVE_RACE;
		return -1;
	}
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
	return race[activeRace]->getWinner();
}

enum HRErrorCode Horserace::setHouseTakeActive(float f){
	if (activeRace == -1)
		return HR_NO_ACTIVE_RACE;
	return race[activeRace]->setHouseTake(f);
}

float Horserace::getHouseTakeActive(enum HRErrorCode * err){
	if (activeRace == -1){
		if (err!=nullptr)
				(*err)=HR_NO_ACTIVE_RACE;
		return -1;
	}
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
	return race[activeRace]->getHouseTake();
}

int Horserace::getHouseWinningsActive(enum HRErrorCode * err){
		if (activeRace == -1){
		if (err!=nullptr)
				(*err)=HR_NO_ACTIVE_RACE;
		return -1;
	}
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
	return race[activeRace]->getHouseWinnings();
}

enum HRErrorCode Horserace::addBetActive(string name, int h, int bet){
	if (activeRace == -1)
		return HR_NO_ACTIVE_RACE;

	return addBet(activeRace,name,h,bet);
}

list<Better> Horserace::getBetterListActive(enum HRErrorCode * err){
	if (activeRace == -1){
		if (err != nullptr)
			(*err) = HR_NO_ACTIVE_RACE;
		 list<Better> empty;
		 return empty;
	}
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
	return getBetterList(activeRace, err);
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
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
	return race[r]->getHorseName(x,err);
}

int Horserace::getHorseOdds(int r, int x, enum HRErrorCode * err){
	if (r < 0 || r>= NUM_RACES){
		if (err != nullptr)
			(*err) = HR_INVALID_RACE;
		return -1;
	}
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
	return race[r]->getHorseOdds(x,err);
}

enum HRErrorCode Horserace::updateOdds(int r){
	if (r < 0 || r >= NUM_RACES)
		return HR_INVALID_RACE;
	race[r]->updateOdds();
	return HR_SUCCESS;
}

enum HRErrorCode Horserace::setWinner(int r, int h){
	if (r < 0 || r >= NUM_RACES)
		return HR_INVALID_RACE;
	return race[r]->setWinner(h);
}

int Horserace::getWinner(int r, enum HRErrorCode * err){
	if (r < 0 || r>= NUM_RACES){
		if (err != nullptr)
			(*err) = HR_INVALID_RACE;
		return -1;
	}
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
	return race[r]->getWinner();
}

enum HRErrorCode Horserace::setHouseTake(int r, float f){
	if (r < 0 || r >= NUM_RACES)
		return HR_INVALID_RACE;
	return race[r]->setHouseTake(f);
}

float Horserace::getHouseTake(int r, enum HRErrorCode * err){
	if (r < 0 || r>= NUM_RACES){
		if (err != nullptr)
			(*err) = HR_INVALID_RACE;
		return -1;
	}
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
	return race[r]->getHouseTake();
}

int Horserace::getHouseWinnings(int r, enum HRErrorCode * err){
	if (r < 0 || r>= NUM_RACES){
		if (err != nullptr)
			(*err) = HR_INVALID_RACE;
		return -1;
	}
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
	return race[r]->getHouseWinnings();
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

list<Better> Horserace::getBetterList(int r, enum HRErrorCode * err){
	if (r < 0 || r>= NUM_RACES){
		if (err != nullptr)
			(*err) = HR_INVALID_RACE;
		list<Better> empty;
		return empty;
	}
	if (err!=nullptr)
		(*err)=HR_SUCCESS;
	return race[r]->getBetterList();
}

		