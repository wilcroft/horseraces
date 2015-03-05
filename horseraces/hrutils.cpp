#include "hrutils.h"

void topmenu(){
	cout << "1. Choose Race" << endl;
	cout << "2. List Betters" << endl;
	cout << "3. List Odds" << endl;
	cout << "4. Add Bet" << endl;
	cout << "5. Add Better" << endl;
	cout << "0. Exit" << endl;
}

int getUserInt(){
	int i=0;
	string s;
	bool ok;

	do{
		cin >> s;
		ok=true;
		try{
			i = stoi(s);
		}
		catch(const std::invalid_argument& ia){
			ok = false;
			cout << s << " is not a valid integer. Please try another" << endl;
		}
	}while (!ok);

	return i;

}

void betTableHeader(){
	cout << std::left << std::setw(35) << "Name" << "\t";
	for (int i = 0 ; i < NUM_HORSES_PER_RACE; i++){
		cout << "Horse " << i << "  " ;
	}
	cout << endl;
}

string strToken(string * str, char delim){
	//find the delimiter
	size_t loc = str->find(delim);
	//if delimiter is first character, remove it and look again
	while (loc == 0 && loc != string::npos){
		str->erase(str->begin());
		loc = str->find(delim);
	}
	string tok = str->substr(0,loc);
	//if not found, copy to tok and erase str
	if (loc == string::npos)
		*str = "";
	// if found, copy token and remove it from str
	else
		str->erase(0,loc+1);
	return tok;
}