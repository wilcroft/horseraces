#include "hrserver.h"

std::mutex thrdlk;
std::mutex iolk;
bool thrdActv [NUMTHRDS];

int main (void){

	WSADATA wsaData;

	int ret;
	SOCKET listensocket = INVALID_SOCKET;
	SOCKET * clientsocket;
	list<string> names = getNamesFromFile();
	Horserace * hr = new Horserace(names);

	//setup and create a socket for listening
	ret = createListenSocket(&listensocket, &wsaData);
	std::thread * clientThread [NUMTHRDS];

	cout << "Set-up OK!" << endl;

	//open created socket for listening
	if ( listen( listensocket, NUMTHRDS ) == SOCKET_ERROR ) {
		cout << "listen() failed!! : " << WSAGetLastError() << endl;
		closesocket(listensocket);
		WSACleanup();
		return -1;
	}
	cout << "Waiting for connection" << endl;

	//Loop to listen for new connections
	while (true){
		clientsocket = new SOCKET;
		(*clientsocket) = INVALID_SOCKET;
		(*clientsocket) = accept(listensocket, nullptr, nullptr);
		if ((*clientsocket) == INVALID_SOCKET){
			cout << "accept() failed!! : " << WSAGetLastError() << endl;
			closesocket(listensocket);
			WSACleanup();
			return -1;
		}

		iolk.lock();
		cout << "Connection recieved!" << endl;
		iolk.unlock();

		if (ret !=0 ){
			cin.sync();
			cin.get();
			return ret;
		}

		//New connection; create a new thread to handle the connection
		thrdlk.lock();
		for (int i = 0; i<NUMTHRDS;i++){
			if (thrdActv[i]==false){
				clientThread[i] = new std::thread(handleClient, clientsocket, i, hr);
				thrdActv[i] = true;
				break;
			}
		}
		thrdlk.unlock();

	}

	cin.sync();
	cin.get();
    closesocket(listensocket);
    WSACleanup();
	delete hr;
	return 0;
}

int createListenSocket(SOCKET* sock, WSADATA* wsaData){

	struct addrinfo* result = nullptr;
	struct addrinfo* ptr = nullptr;
	struct addrinfo hints;
	int sockcount = 0;
	(*sock) = INVALID_SOCKET;

	if (WSAStartup(MAKEWORD(2,2), wsaData) != 0){
		cout << "Startup Failed!" << endl;
		return -1;
	}

	ZeroMemory(&hints, sizeof(hints)); // dumb?
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(nullptr,PORT,&hints,&result) != 0){
		cout << "getaddrinfo failed! " << endl;
		WSACleanup();
		return -1;
	}
	if (result == nullptr){
		cout << " bad interface: " << endl;
		WSACleanup();
		return -1;
	}

	ptr = result;
	while (ptr != nullptr){
		sockcount++;
		ptr = ptr->ai_next;
	}

	cout << "Found " << sockcount << " addresses!" << endl;

	(*sock) = socket(result->ai_family,result->ai_socktype, result->ai_protocol);

	if ((*sock) == INVALID_SOCKET){
		cout << "socket() failed! : " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return -1;
	}
	
	if (bind((*sock), result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
        WSACleanup();
        return -1;
    }

	freeaddrinfo(result);

	return 0;
}

void handleClient(SOCKET* sock, int i, Horserace * hr){
	int ret;
	char inbuf [BUFLEN];
	string bufcpy;
	do{
		ZeroMemory(inbuf, BUFLEN*sizeof(char));
		ret = recv((*sock), inbuf, BUFLEN, 0);

		if (ret > 0){
			bufcpy = inbuf;
			iolk.lock();
			cout << "Recieved Message (" << strlen(inbuf) << ") : \"" << bufcpy << "\"" << endl;
			iolk.unlock();
			handleRequest(bufcpy, hr, sock);
		}
		else if (ret == 0){
			iolk.lock();
			cout << "End of Connection." << endl;
			iolk.unlock();
		}
		else{
			iolk.lock();
			cout << "recv() failed! : " << WSAGetLastError() << endl;
			iolk.unlock();
			closesocket((*sock));
			WSACleanup();
		}

	}while (ret>0);
	
	//*(rv)=ret;

	if (shutdown((*sock),SD_SEND) == SOCKET_ERROR){
		iolk.lock();
		cout << "Shutdown failed! : " << WSAGetLastError() << endl;
		iolk.unlock();
		closesocket((*sock));
		WSACleanup();
		//(*rv) = -1;
	}
	
	closesocket((*sock));
	delete sock;
	thrdlk.lock();
	thrdActv[i] = false;
	thrdlk.unlock();

}

list <string> getNamesFromFile(string s){
	std::ifstream namefile;
	std::filebuf * fb = namefile.rdbuf();

	list <string> names;
	string temp;
	
	fb->open (s, std::ios::in);

	if (fb->is_open()){
		while (!namefile.eof()){
			namefile >> temp;
			names.push_back(temp);
		}
		fb->close();
	}

	names.sort();
	return names;
}

void handleRequest(string req, Horserace * hr, SOCKET* sock){
	char opA, opB;
	string buf;
	//char retbuf [BUFLEN];
	// store first to chars (OP code) to op
	if (req.length()<2){
		iolk.lock();
		cout << "INVALID OPERATION!!! "<< endl;
		iolk.unlock();
		return;
	}
	string op = "";
	op.push_back(req[0]);
	op.push_back(req[1]);
	//remove OP code from req;
	req.erase(0,2);

	if (op == "GA"){		//Get the active race
		int active = hr->getActiveRace();
		buf="OK " + std::to_string(active);
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else if (op == "GH");
	else if (op == "Gh");
	else if (op == "GP"){
		list<string> names = hr->getParticipants();
		int size = names.size();
		buf = "OK " + std::to_string(size);
		send(*sock, buf.c_str(), buf.length(),0);
		for (auto& x: names){
			send(*sock, x.c_str(), x.length(),0);
		}
	}
	else if (op == "GO");
	else if (op == "Go");
	else if (op == "G$");
	else if (op == "GB");
	else if (op == "Gb");
	else if (op == "GW");
	else if (op == "Gw"){
		enum HRErrorCode err;
		int winnings = hr->getHouseWinningsActive(&err);
		if (err == HR_SUCCESS)
			buf = "OK " + std::to_string(winnings);
		else
			buf = "ER " + std::to_string(err);
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else if (op == "GT");
	else if (op == "Gt");
	else if (op == "GV");
	else if (op == "Gv");
	else if (op == "AB"){
		iolk.lock();
		while (req != ""){
			cout << strToken(&req, ' ') << endl;
		}
		iolk.unlock();
	}
	else if (op == "AP");
	else if (op == "SH");
	else if (op == "Sh");
	else if (op == "ST");
	else if (op == "St");
	else if (op == "SA"){
		int active;
		active = std::stoi(req);
		enum HRErrorCode err;
		//iolk.lock();
		//cout << "Setting Race '" << active << "' as active." << endl;
		//iolk.unlock();
		err = hr->setActiveRace(active);
		if (err == HR_SUCCESS){
			buf = "OK";
		}
		else{
			buf = "ER " + std::to_string(err);
		}
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else if (op == "SX"){
		hr->setNoActiveRace();
		buf = "OK";
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else if (op == "SV"){
		int win;
		int race;
		race = std::stoi(strToken(&req));
		win = std::stoi(strToken(&req));
		enum HRErrorCode err = hr->setWinner(race,win);
		if (err == HR_SUCCESS){
			buf = "OK";
		}
		else{
			buf = "ER " + std::to_string(err);
		}
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else if (op == "Sv"){
		int win = std::stoi(req);
		enum HRErrorCode err = hr->setWinnerActive(win);
		if (err == HR_SUCCESS){
			buf = "OK";
		}
		else{
			buf = "ER " + std::to_string(err);
		}
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else{
		iolk.lock();
		cout << "INVALID OPERATION!!! "<< endl;
		iolk.unlock();
	}

	

}