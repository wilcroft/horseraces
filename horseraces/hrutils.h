#ifndef __HRUTILS_H_
#define __HRUTILS_H_

#include <iostream>

#if  defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <term.h>
#endif

using namespace std;
void cls();
void topmenu();


#endif