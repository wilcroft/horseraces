#include <iostream>
#include <string>
#include <list>

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
				cin >> opt;
				break;
			case 2:
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
				cin >> opt;
				break;
			case 4:
				break;
			case 5:
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