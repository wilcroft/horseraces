#ifndef __HRCLIENTLIBRARY_H_
#define __HRCLIENTLIBRARY_H_

#include <string>
#include <list>
#include <vector>

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
	typedef int SOCKET;
	typedef void WSADATA;
	#define INVALID_SOCKET 0
	#define SOCKET_ERROR -1
#endif

#include "../horseraces/constants.h"
#include "../horseraces/horseraceerrors.h"
#include "../horseraces/better.h"
#include "../horseraces/hrutils.h"


using std::list;
using std::string;
using std::vector;

//Get Functions
enum HRErrorCode getActiveRace(int* r, SOCKET * sock);
enum HRErrorCode getAllHorseNames(int r, list<string> * s, SOCKET * sock);
enum HRErrorCode getAllHorseNamesActive(list<string> * s, SOCKET * sock);
enum HRErrorCode getHorseName(int r, int h, string * s, SOCKET * sock);
enum HRErrorCode getHorseNameActive(int h, string * s, SOCKET * sock);
enum HRErrorCode getParticipantList(list<string> * s, SOCKET * sock);
enum HRErrorCode getAllHorseOdds(int r, vector<int> * o, SOCKET * sock);
enum HRErrorCode getAllHorseOddsActive (vector<int>* o, SOCKET * sock);
enum HRErrorCode getHorseOdds (int r, int h, int * o, SOCKET * sock);
enum HRErrorCode getHorseOddsActive(int h, int * o, SOCKET * sock);
enum HRErrorCode getPayoutList(int r, list<Better> * l, SOCKET * sock);
enum HRErrorCode getBetList(int r, list<Better> * l, SOCKET * sock);
enum HRErrorCode getBetListActive(list<Better> * l, SOCKET * sock);
enum HRErrorCode getHouseWinnings(int r, int * w, SOCKET * sock);
enum HRErrorCode getHouseWinningsActive(int * w, SOCKET * sock);
enum HRErrorCode getAllHouseWinnings(int * w, SOCKET * sock);
enum HRErrorCode getHouseTakePct(int r, float * t, SOCKET * sock);
enum HRErrorCode getHouseTakePctActive(float * t, SOCKET * sock);
enum HRErrorCode getWinningHorse(int r, int * h, SOCKET * sock);
enum HRErrorCode getWinningHorseActive(int * h, SOCKET * sock);

//Add Functions
enum HRErrorCode addBet(int r, int h, int b, string n, SOCKET * sock);
enum HRErrorCode addParticipant(string n, SOCKET * sock);

//Set Functions
enum HRErrorCode setHorseName(int r, int h, string n, SOCKET * sock);
enum HRErrorCode setHorseNameActive(int h, string n, SOCKET * sock);
enum HRErrorCode setHouseTake(int r, float t, SOCKET * sock);
enum HRErrorCode setAllHouseTake(float t, SOCKET * sock);
enum HRErrorCode setHouseTakeActive(float t, SOCKET * sock);
enum HRErrorCode setActiveRace(int r, SOCKET * sock);
enum HRErrorCode setNoActiveRace(SOCKET * sock);
enum HRErrorCode setWinningHorse(int r, int h, SOCKET * sock);
enum HRErrorCode setWinningHorseActive(int h, SOCKET * sock);

int createClientSocket(string addr, string port, SOCKET* sock, WSADATA* wsaData=nullptr);
enum HRErrorCode closeClientSocket(SOCKET* sock);

int lineCount(string s, char c = '\n');
#endif
