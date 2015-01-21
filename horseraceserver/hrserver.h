#include <iostream>
#include <string>
#include <list>
#include <chrono>
#include <thread>
#include <fstream>
#include <thread>
#include <mutex>

#include <cstring>



#if defined(_WIN32) || defined(_WIN64)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include "../horseraces/horserace.h"
#include "../horseraces/better.h"
#include "../horseraces/constants.h"
#include "../horseraces/race.h"
#include "../horseraces/horse.h"
#include "../horseraces/hrutils.h"

#define NAMEFILE "names.txt"
#define PORT "23456"
#define BUFLEN 512
#define NUMTHRDS 10

int createListenSocket(SOCKET* sock, WSADATA* wsaData);
void handleClient(SOCKET* sock, int i, int* rv);