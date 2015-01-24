#include "hrclientlibrary.h"

//Get Functions
/************************************
 * getActiveRace()
 *		Get the number of the currently active race, from 0 to NUM_RACES, 
 *		or -1 if no race is active.
 * 
 * r: the returned value of the active race
 * sock: the active socket connected to the server
 *
 * Returns HR_SUCCESS (0) on success, or the appropriate error from the server
 *
 ************************************/
enum HRErrorCode getActiveRace(int* r, SOCKET * sock){
	string buf;
	char recvbuf [BUFLEN];
	buf = "GA";
	send(*sock, buf.c_str(), buf.length(),0);
	recv(*sock, recvbuf,BUFLEN,0);
	buf = recvbuf;
	if (strToken(&buf)=="OK"){
		(*r) = std::stoi(buf);
		return HR_SUCCESS;
	}
	else{
		return (enum HRErrorCode) std::stoi(buf);
	}
}

enum HRErrorCode getAllHorseNames(int r, list<string> * s, SOCKET * sock);
enum HRErrorCode getAllHorseNamesActive(list<string> * s, SOCKET * sock);
enum HRErrorCode getHorseName(int r, int h, string * s, SOCKET * sock);
enum HRErrorCode getHorseNameActive(int h, string * s, SOCKET * sock);
enum HRErrorCode getParticipantList(SOCKET * sock);
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
enum HRErrorCode addBet(int r, int h, string n, SOCKET * sock);
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
