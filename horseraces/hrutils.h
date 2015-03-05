#ifndef __HRUTILS_H_
#define __HRUTILS_H_

#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip>

#include "constants.h"

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <string.h>
#define ZeroMemory(p,sz) memset((p),0,(sz))
#ifdef NEEDCLI
#include <term.h>
#include <curses.h>
#endif
#endif

//using namespace std;
using std::cout;
using std::cin;
using std::string;
using std::endl;

#define DEBUG 1
#if DEBUG > 0
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

#ifdef NEEDCLI
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
#endif
void topmenu();
int getUserInt();
void betTableHeader();

string strToken(string * str, char delim = ' ');


#endif
