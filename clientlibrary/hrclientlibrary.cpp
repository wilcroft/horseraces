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
/************************************
 * getAllHorseNames()
 *		Get the names of all horses for a given race as a list of strings
 * 
 * r: the returned value of the active race
 * sock: the active socket connected to the server
 *
 * Returns HR_SUCCESS (0) on success, or the appropriate error from the server
 *
 ************************************/
enum HRErrorCode getAllHorseNames(int r, list<string> * s, SOCKET * sock){
	string buf;
	list<string> names;
	char cbuf [BUFLEN];
	buf = "GH " + std::to_string(r);
	send(*sock, buf.c_str(), buf.length(), 0);
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		int num = std::stoi(buf);
		buf = "";
		do{
			ZeroMemory(cbuf,BUFLEN*sizeof(char));
			recv(*sock, cbuf, BUFLEN, 0);
			buf += cbuf;
		} while (lineCount(buf) < num);
		do {
			names.push_back(strToken(&buf,'\n'));
		}while (buf != "");
		(*s) = names;
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode getAllHorseNamesActive(list<string> * s, SOCKET * sock){
	string buf;
	string linea;
	list<string> names;
	char cbuf [BUFLEN];
	buf = "Gh";
	send(*sock, buf.c_str(), buf.length(), 0);
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	linea = strToken(&buf, '\n');
	if (strToken(&linea)=="OK"){
		int num = std::stoi(linea);
//		buf = "";
		while (lineCount(buf) < num){
			ZeroMemory(cbuf,BUFLEN*sizeof(char));
			recv(*sock, cbuf, BUFLEN, 0);
			buf += cbuf;
		} 
		do {
			names.push_back(strToken(&buf,'\n'));
		}while (buf != "");
		(*s) = names;
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(linea);
}
enum HRErrorCode getHorseName(int r, int h, string * s, SOCKET * sock){
	string buf;
	list<string> names;
	char cbuf [BUFLEN];
	buf = "GH " + std::to_string(r) + " " + std::to_string(h);
	send(*sock, buf.c_str(), buf.length(), 0);
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		*s = buf;
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode getHorseNameActive(int h, string * s, SOCKET * sock){
	string buf;
	list<string> names;
	char cbuf [BUFLEN];
	buf = "Gh " + std::to_string(h);
	send(*sock, buf.c_str(), buf.length(), 0);
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		*s = buf;
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode getParticipantList(list<string> * s, SOCKET * sock){
	string buf;
	list<string> names;
	char cbuf [BUFLEN];
	buf = "GP";
	send(*sock, buf.c_str(), buf.length(), 0);
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		int num = std::stoi(buf);
		buf = "";
		while (lineCount(buf) < num){
			ZeroMemory(cbuf,BUFLEN*sizeof(char));
			recv(*sock, cbuf, BUFLEN, 0);
			buf += cbuf;
		} 
		do {
			names.push_back(strToken(&buf,'\n'));
		}while (buf != "");
		(*s) = names;
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode getAllHorseOdds(int r, vector<int> * o, SOCKET * sock){
	string buf;
	vector<int> odds;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "GO " + std::to_string(r);
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		int num = std::stoi(strToken(&buf));
//		buf = "";
		do{
			ZeroMemory(cbuf,BUFLEN*sizeof(char));
			recv(*sock, cbuf, BUFLEN, 0);
			buf += cbuf;
		} while (lineCount(buf) < num);
		do{
			odds.push_back(std::stoi(strToken(&buf,'\n')));
		} while (buf != "");
		(*o) = odds;
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode getAllHorseOddsActive (vector<int>* o, SOCKET * sock){
	string buf;
	string linea;
	vector<int> odds;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "Go";
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	linea = strToken(&buf, '\n');
	if (strToken(&linea)=="OK"){
		int num = std::stoi(linea);
//		buf = "";
		if (buf != "" && buf != "\n")
			int i = 0;
		while (lineCount(buf) < num){
			ZeroMemory(cbuf,BUFLEN*sizeof(char));
			recv(*sock, cbuf, BUFLEN, 0);
			buf += cbuf;
		}
		do{
			odds.push_back(std::stoi(strToken(&buf,'\n')));
		} while (buf != "");
		(*o) = odds;
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(linea);
}
enum HRErrorCode getHorseOdds (int r, int h, int * o, SOCKET * sock){
	return HR_UNIMPLEMENTED;
}
enum HRErrorCode getHorseOddsActive(int h, int * o, SOCKET * sock){
	return HR_UNIMPLEMENTED;
}
enum HRErrorCode getPayoutList(int r, list<Better> * l, SOCKET * sock){
	return HR_UNIMPLEMENTED;
}
enum HRErrorCode getBetList(int r, list<Better> * l, SOCKET * sock){
	return HR_UNIMPLEMENTED;
}
enum HRErrorCode getBetListActive(list<Better> * l, SOCKET * sock){
	return HR_UNIMPLEMENTED;
}
enum HRErrorCode getHouseWinnings(int r, int * w, SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "GW " + std::to_string(r);
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		*w = std::stoi(buf);
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode getHouseWinningsActive(int * w, SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "Gw";
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		*w = std::stoi(buf);
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode getAllHouseWinnings(int * w, SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "GW";
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		*w = std::stoi(buf);
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode getHouseTakePct(int r, float * t, SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "GT " + std::to_string(r);
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		(*t) = std::stof(buf);
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode getHouseTakePctActive(float * t, SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "Gt";
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		(*t) = std::stof(buf);
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode getWinningHorse(int r, int * h, SOCKET * sock){
	return HR_UNIMPLEMENTED;
}
enum HRErrorCode getWinningHorseActive(int * h, SOCKET * sock){
	string buf;
	//vector<int> odds;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "Gv";
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		*h = std::stoi(buf);
		/*buf = "";
		do{
			ZeroMemory(cbuf,BUFLEN*sizeof(char));
			recv(*sock, cbuf, BUFLEN, 0);
			buf += cbuf;
		} while (lineCount(buf) < num);
		do{
			odds.push_back(std::stoi(strToken(&buf,'\n')));
		} while (buf != "");*/
		//(*o) = odds;
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}

//Add Functions
enum HRErrorCode addBet(int r, int h, int b, string n, SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "AB " + std::to_string(r) + " " + std::to_string(h) + " " +
		std::to_string(b) + " " + n;
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode addParticipant(string n, SOCKET * sock){
	return HR_UNIMPLEMENTED;
}

//Set Functions
enum HRErrorCode setHorseName(int r, int h, string n, SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "SH " + std::to_string(r) + " " + std::to_string(h) + " " + n;
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode setHorseNameActive(int h, string n, SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "Sh " + std::to_string(h) + " " + n;
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode setHouseTake(int r, float t, SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "ST " + std::to_string(t) + " " + std::to_string(r);
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode setAllHouseTake(float t, SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "ST " + std::to_string(t);
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode setHouseTakeActive(float t, SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "St " + std::to_string(t);
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode setActiveRace(int r, SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "SA " + std::to_string(r);
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode setNoActiveRace(SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "SX";
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode setWinningHorse(int r, int h, SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "SV " + std::to_string(r) + " " + std::to_string(h);
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}
enum HRErrorCode setWinningHorseActive(int h, SOCKET * sock){
	string buf;
	char cbuf [BUFLEN];
	ZeroMemory(cbuf,BUFLEN*sizeof(char));
	buf = "Sv " + std::to_string(h);
	send(*sock, buf.c_str(), buf.length(), 0);
	recv(*sock, cbuf, BUFLEN, 0);
	buf = cbuf;
	if (strToken(&buf)=="OK"){
		return HR_SUCCESS;
	}
	else
		return (enum HRErrorCode)std::stoi(buf);
}

int createClientSocket(string addr, string port, SOCKET* sock, WSADATA* wsaData){
	(*sock) = INVALID_SOCKET;

	struct addrinfo * result = NULL,
					* ptr = NULL,
					hints;

	if (WSAStartup(MAKEWORD(2,2), wsaData) != 0){
		std:: cerr << "Startup failed! " << endl;
		return -1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(addr.c_str(), port.c_str(), &hints, &result) != 0){
		std::cerr << "getaddrinfo() failed!" << endl;
		WSACleanup();
		return -1;
	}

	for (ptr=result; ptr!=nullptr; ptr=ptr->ai_next){
		(*sock)=socket(ptr->ai_family,ptr->ai_socktype, ptr->ai_protocol);
		if (*sock == INVALID_SOCKET){
			std::cerr << "socket() failed! : " << WSAGetLastError() << endl;
			WSACleanup();
			return -1;
		}
		if (connect(*sock,ptr->ai_addr, (int) ptr->ai_addrlen) == SOCKET_ERROR){
			closesocket(*sock);
			(*sock) = INVALID_SOCKET;
		}
		else
			break;
	}

	freeaddrinfo(result);

	if (*sock == INVALID_SOCKET){
		std::cerr << "Couldn't find server at " << addr << ":" << port << endl;
		WSACleanup();
		return -1;
	}

	return 0;

}

int lineCount(string s, char c){
	int sum = 0;
	for (unsigned int i=0; i<s.length(); i++){
		if (s[i]==c) sum++;
	}
	return sum;
}