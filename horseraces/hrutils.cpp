#include "hrutils.h"

#if defined(_WIN64) || defined(_WIN32)
void cls(){
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
}
#elif defined(__linux__)
void cls()
  {
  if (!cur_term)
    {
    int result;
    setupterm( NULL, STDOUT_FILENO, &result );
    if (result <= 0) return;
    }

  putp( tigetstr( "clear" ) );
  }

#endif

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
	cout << std::left << std::setw(15) << "Name" << "\t";
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