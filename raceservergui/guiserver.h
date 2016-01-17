#ifndef __GUISERVER_H_
#define __GUISERVER_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <list>
//#include <chrono>
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
#elif defined (__linux__)
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <ifaddrs.h>
	typedef int SOCKET;
	typedef void WSADATA;
	#define INVALID_SOCKET 0
	#define SOCKET_ERROR -1
	#define SD_BOTH SHUT_RDWR
	#define WSACleanup()
	#define WSAGetLastError() ""
	#define closesocket(p) close (p)
#endif

#include "../horseraces/horserace.h"
#include "../horseraces/better.h"
#include "../horseraces/constants.h"
#include "../horseraces/race.h"
#include "../horseraces/horse.h"
#include "../horseraces/hrutils.h"

#define NAMEFILE "names.txt"
#define PAYOUT_PREFIX "payout-race"
#define PAYOUT_SUFFIX ".txt"
#define BETLIST_PREFIX "betlist-race"
#define BETLIST_SUFFIX ".txt"
#define NUMTHRDS 10
	
extern Horserace * hr;
extern std::mutex thrdlk;
extern std::mutex iolk;
extern std::mutex betlk[NUM_RACES];
extern std::mutex paylk[NUM_RACES];
extern bool thrdActv [NUMTHRDS];
extern std::string serverIP;
extern std::ostream logstream;
extern std::ostream errstream;

void serverFunc(bool * isActive);//, std::iostream * serverMsgs = nullptr);
int createListenSocket(SOCKET* sock, WSADATA* wsaData=nullptr);
void handleClient(SOCKET* sock, int i, Horserace* hr);
void handleRequest(string req, Horserace * hr, SOCKET* sock);
list<string> getNamesFromFile(string s = NAMEFILE);
void getHorseNamesFromFile(string s, Horserace * hr);
void writePayoutListToFile(Horserace * hr, int r,string fname = "");
void writeBetListToFile(Horserace * hr, int r,string fname = "");


#define LOGGING 1
#if LOGGING > 0
#define LOG_MSG(str) do {iolk.lock(); logstream << str << std::endl; iolk.unlock();} while( false )
#define ERR_MSG(str) do {iolk.lock(); errstream << str << std::endl; iolk.unlock();} while( false )
#define DBG_MSG(str) do { } while ( false )
#elif LOGGING > 1
#define LOG_MSG(str) do {iolk.lock(); logstream << str << std::endl; iolk.unlock();} while( false )
#define ERR_MSG(str) do {iolk.lock(); errstream << str << std::endl; iolk.unlock();} while( false )
#define DBG_MSG(str) do {iolk.lock(); logstream << str << std::endl; iolk.unlock();} while ( false )
#else
#define LOG_MSG(str) do { } while ( false )
#define ERR_MSG(str) do { } while ( false )
#define DBG_MSG(str) do { } while ( false )
#endif

//#define SERVER_LOG(str) if (serverMsgs != nullptr) { (*serverMsgs) << str << std::endl; } while( false )


#endif