#ifndef __HRUTILS_H_
#define __HRUTILS_H_

#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip>

#include "constants.h"

#if  defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <term.h>
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

void cls();
void topmenu();
int getUserInt();
void betTableHeader();

string strToken(string * str, char delim = ' ');


#endif