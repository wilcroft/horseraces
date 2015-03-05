#include <iostream>
#include <string>
#include <list>
#include <chrono>
#include <thread>
#include <fstream>

#define NEEDCLI

#include "horserace.h"
#include "better.h"
#include "constants.h"
#include "race.h"
#include "horse.h"
#include "hrutils.h"

#define NAMEFILE "names.txt"

using namespace std;


int main (){
	ifstream namefile;
	filebuf * fb = namefile.rdbuf();

	list <string> names;
	string opt;
	enum HRErrorCode err;
	int i=-1;
	bool fin = false;
	
	fb->open (NAMEFILE, ios::in);

	if (fb->is_open()){
		while (!namefile.eof()){
			namefile >> opt;
			names.push_back(opt);
		}
		fb->close();
	}

	names.sort();
	Horserace foo(names);
	
	while (!fin){
		cls();
		topmenu();
		//cin >> i;
		i = getUserInt();
		switch(i){
			case 1:			// Choose Race
				cout << "Enter race number: " ;
				i = getUserInt();
				cout << endl;
				err = foo.setActiveRace(i);
				if (err == HR_SUCCESS){
					cout << "Race " << i << " has begun!" << endl;
				}
				else {
					cout << "Race " << i << " is not valid!" << endl;
				}
				this_thread::sleep_for(chrono::seconds(1));
				break;
			case 2:			// List Betters
				if (foo.getActiveRace() == -1){
					cout << "No active race..." << endl;
				}
				else{
					list <Better> betters;
					betters = foo.getBetterListActive(&err);
					betTableHeader();
					for (auto& x : betters){
						x.print();
					}
				}
				//this_thread::sleep_for(chrono::seconds(2));
				//char c;'
				cin.sync();
				cin.get();
				break;
			case 3:			// List Odds
				if (foo.getActiveRace() == -1){
					cout << "No active race..." << endl;
				}
				else{
					for (i=0;i<NUM_HORSES_PER_RACE;i++){
						cout << "Horse " << i+1 <<" - ";
						cout << foo.getHorseOddsActive(i) << ":1" << endl;
					}
				}
				cin.sync();
				cin.get();
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
					bet = getUserInt();
					cout << endl << "Horse: ";
					horse = getUserInt();
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
				this_thread::sleep_for(chrono::seconds(1));
				break;
			case 5:			// Add Better
				if (foo.getActiveRace() == -1){
					cout << "No active race..." << endl;
				}
				else{
				}
				this_thread::sleep_for(chrono::seconds(1));
				break;
			case 0:
				fin=true;
			default:
				;
		}
	}

	return 0;
}