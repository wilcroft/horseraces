#include <iostream>
#include <string>
#include <list>
#include <chrono>
#include <thread>

#include "horserace.h"
#include "better.h"
#include "constants.h"
#include "race.h"
#include "horse.h"
#include "hrutils.h"

using namespace std;


int main (){
	list <string> names;
	string opt;
	enum HRErrorCode err;
	int i=-1;
	bool fin = false;

	names.push_back("A");
	names.push_back("B");
	names.push_back("C");
	names.push_back("D");
	names.push_back("E");
	names.push_back("F");
	Horserace foo(names);
	Horserace bar(names);
	bar.addParticipant("G");
	bar.addParticipant("H");

	while (!fin){
		cls();
		topmenu();
		cin >> i;
		switch(i){
			case 1:
				cout << "Enter race number: " ;
				cin >> i;
				cout << endl;
				err = foo.setActiveRace(i);
				if (err == HR_SUCCESS){
					cout << "Race " << i << " has begun!" << endl;
				}
				else {
					cout << "Race " << i << " is not valid!" << endl;
				}
				this_thread::sleep_for(chrono::milliseconds(750));
				break;
			case 2:
				if (foo.getActiveRace() == -1){
					cout << "No active race..." << endl;
				}
				else{
				}
				break;
			case 3:
				if (foo.getActiveRace() == -1){
					cout << "No active race..." << endl;
				}
				else{
					for (i=0;i<NUM_HORSES_PER_RACE;i++){
						cout << "Horse " << i+1 <<" - ";
						cout << foo.getHorseOddsActive(i) << ":1" << endl;
					}
				}
				this_thread::sleep_for(chrono::milliseconds(750));
				break;
			case 4:			// Add Bet
				if (foo.getActiveRace() == -1){
					cout << "No active race..." << endl;
				}
				else{
					string name;
					int bet;
					int horse;
					cout << "Better Name: " ;
					cin >> name;
					cout << endl << "Bet Amount: $" ;
					cin >> bet;
					cout << endl << "Horse: ";
					cin >> horse;
					cout << endl;
					err = foo.addBetActive(name, horse, bet);
					if (err == HR_SUCCESS)
						cout << "OK!" << endl;
					else if (err == HR_INVALID_BET)
						cout << "A bet of $" << bet << " is invalid!" << endl;
					else if (err == HR_INVALID_HORSE)
						cout << "Horse Number " << horse << " is invalid!" << endl;
					else
						cout << "Unknown error!!!" << endl;


				}
				cin >> opt;
				break;
			case 5:
				if (foo.getActiveRace() == -1){
					cout << "No active race..." << endl;
				}
				else{
				}
				break;
			case 0:
				fin=true;
			default:
				;
		}
	}


	getchar();
	return 0;
}