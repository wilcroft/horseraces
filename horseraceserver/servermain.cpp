#include "hrserver.h"

std::mutex thrdlk;
std::mutex iolk;
std::mutex betlk[NUM_RACES];
std::mutex paylk[NUM_RACES];
bool thrdActv [NUMTHRDS];

int main (void){

	WSADATA wsaData;

	int ret;
	SOCKET listensocket = INVALID_SOCKET;
	SOCKET * clientsocket;
	list<string> names = getNamesFromFile();
	Horserace * hr = new Horserace(names);
	getHorseNamesFromFile("horses.txt",hr);

	//setup and create a socket for listening
	ret = createListenSocket(&listensocket, &wsaData);
	std::thread * clientThread [NUMTHRDS];

	cout << "Set-up OK!" << endl;

	//open created socket for listening
	if ( listen( listensocket, NUMTHRDS ) == SOCKET_ERROR ) {
		std::cerr << "listen() failed!! : " << WSAGetLastError() << endl;
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
			std::cerr << "accept() failed!! : " << WSAGetLastError() << endl;
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
		std::cerr << "Startup Failed!" << endl;
		return -1;
	}

	ZeroMemory(&hints, sizeof(hints)); // dumb?
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(nullptr,PORT,&hints,&result) != 0){
		std::cerr << "getaddrinfo failed! " << endl;
		WSACleanup();
		return -1;
	}
	if (result == nullptr){
		std::cerr << " bad interface: " << endl;
		WSACleanup();
		return -1;
	}

	ptr = result;
	while (ptr != nullptr){
		sockcount++;
		ptr = ptr->ai_next;
	}

	DEBUG_MSG("Found " << sockcount << " addresses!");

	int blks[4];
	for (int i=0; i<4; i++){
		blks[i] = ((int)(result->ai_addr) >> 8*i) & 0xFF;
	}
	/*cout << "IP: " << result->ai_canonname << endl;
	cout << "IP: " << blks[3] << ":" << blks[2] << ":" << blks[1] ;
	cout << ":" << blks[0] << endl;
	cout << "Port: " << PORT << endl;*/

	(*sock) = socket(result->ai_family,result->ai_socktype, result->ai_protocol);

	if ((*sock) == INVALID_SOCKET){
		std::cerr << "socket() failed! : " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return -1;
	}
	
	if (bind((*sock), result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        std::cerr << "bind() failed! : " <<  WSAGetLastError() << endl;
		freeaddrinfo(result);
        WSACleanup();
        return -1;
    }

	freeaddrinfo(result);
	
    char ac[80];
    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
        std::cerr << "Error " << WSAGetLastError() <<
                " when getting local host name." << endl;
        return 1;
    }
    struct hostent *phe = gethostbyname(ac);
    if (phe == 0) {
        std::cerr << "Yow! Bad host lookup." << endl;
        return 1;
    }

    struct in_addr addr;
    memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
	cout << "IP: " << inet_ntoa(addr) << endl;
	cout << "Port: " << PORT << endl;

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
			DEBUG_MSG("Recieved Message (" << strlen(inbuf) << ") : \"" << bufcpy << "\"");
			iolk.unlock();
			handleRequest(bufcpy, hr, sock);
		}
		else if (ret == 0){
			iolk.lock();
			DEBUG_MSG("End of Connection.");
			iolk.unlock();
		}
		else{
			iolk.lock();
			std::cerr << "recv() failed! : " << WSAGetLastError() << endl;
			iolk.unlock();
			closesocket((*sock));
			WSACleanup();
		}

	}while (ret>0);
	
	//*(rv)=ret;

	if (shutdown((*sock),SD_SEND) == SOCKET_ERROR){
		iolk.lock();
		std::cerr << "Shutdown failed! : " << WSAGetLastError() << endl;
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
	char temp[BUFLEN];
	
	fb->open (s, std::ios::in);

	if (fb->is_open()){
		while (!namefile.eof()){
			namefile.getline(temp,BUFLEN);
			names.push_back(temp);
		}
		fb->close();
	}

	names.sort();
	return names;
}
void getHorseNamesFromFile(string s, Horserace * hr){
	std::ifstream namefile;
	std::filebuf * fb = namefile.rdbuf();

	int i=0;
	char temp[BUFLEN];
	
	fb->open (s, std::ios::in);

	if (fb->is_open()){
		while (!namefile.eof() && i < NUM_RACES*NUM_HORSES_PER_RACE){
			namefile.getline(temp,BUFLEN);
			hr->setHorseName( i/NUM_HORSES_PER_RACE,i%NUM_HORSES_PER_RACE,temp);
			i++;
		}
		fb->close();
	}

}
void handleRequest(string req, Horserace * hr, SOCKET* sock){
	string buf;
	//char retbuf [BUFLEN];
	// store first to chars (OP code) to op
	if (req.length()<2){
		iolk.lock();
		std::cerr << "INVALID OPERATION!!! "<< endl;
		iolk.unlock();
		return;
	}
	string op = "";
	op.push_back(req[0]);
	op.push_back(req[1]);
	//remove OP code from req;
	req.erase(0,2);
	enum HRErrorCode err = HR_SUCCESS;

	if (op == "GA"){		//Get the active race
		int active = hr->getActiveRace();
		buf="OK " + std::to_string(active);
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else if (op == "GH"){
		int r = std::stoi(strToken(&req));
		string hstr = strToken(&req);
		if (hstr == "" || hstr == "\n"){
			list<string> hnames;
			int i = 0;
			hr->lock();
			while (err == HR_SUCCESS && i<NUM_HORSES_PER_RACE){
				hnames.push_back(hr->getHorseName(r,i,&err));
				i++;
			}
			hr->unlock();
			if (err == HR_SUCCESS){
				buf = "OK " + std::to_string(NUM_HORSES_PER_RACE);
				send(*sock, buf.c_str(), buf.length(),0);
				for (auto& x : hnames){
					buf = x + "\n";
					send(*sock, buf.c_str(), buf.length(),0);	
				}
			}
			else{
				buf = "ER " + std::to_string(err);
				send(*sock, buf.c_str(), buf.length(),0);
			}
		}
		else{
			string hname = hr->getHorseName(r,std::stoi(hstr),&err);
			if (err == HR_SUCCESS)
				buf = "OK " + hname;
			else
				buf = "ER " + std::to_string(err);
			send(*sock, buf.c_str(), buf.length(),0);
		}
	}
	else if (op == "Gh"){
		string hstr = strToken(&req);
		enum HRErrorCode err;
		if (hstr == "" || hstr == "\n"){
			list<string> hnames;
			err = HR_SUCCESS;
			int i = 0;
			hr->lock();
			while (err == HR_SUCCESS && i<NUM_HORSES_PER_RACE){
				hnames.push_back(hr->getHorseNameActive(i,&err));
				i++;
			}
			hr->unlock();
			if (err == HR_SUCCESS){
				buf = "OK " + std::to_string(NUM_HORSES_PER_RACE) + "\n";
				send(*sock, buf.c_str(), buf.length(),0);
				for (auto& x : hnames){
					buf = x + "\n";
					send(*sock, buf.c_str(), buf.length(),0);	
				}
			}
			else{
				buf = "ER " + std::to_string(err);
				send(*sock, buf.c_str(), buf.length(),0);
			}
		}
		else{
			string hname = hr->getHorseNameActive(std::stoi(hstr),&err);
			if (err == HR_SUCCESS)
				buf = "OK " + hname;
			else
				buf = "ER " + std::to_string(err);
			send(*sock, buf.c_str(), buf.length(),0);
		}
	}
	else if (op == "GP"){
		list<string> names = hr->getParticipants();
		int size = names.size();
		buf = "OK " + std::to_string(size) + "\n";
		send(*sock, buf.c_str(), buf.length(),0);
		for (auto& x: names){
			buf = x + "\n";
			send(*sock, buf.c_str(), buf.length(),0);	
		}
	}
	else if (op == "GO"){
		int r = std::stoi(strToken(&req));
		string hstr = strToken(&req);
		enum HRErrorCode err;
		if (hstr == "" || hstr == "\n"){
			list<int> odds;
			err = HR_SUCCESS;
			int i = 0;
			hr->lock();
			while (err == HR_SUCCESS && i<NUM_HORSES_PER_RACE){
				odds.push_back(hr->getHorseOdds(r,i,&err));
				i++;
			}
			hr->unlock();
			if (err == HR_SUCCESS){
				buf = "OK " + std::to_string(NUM_HORSES_PER_RACE) + "\n";
				send(*sock, buf.c_str(), buf.length(),0);
				for (auto& x : odds){
					buf = std::to_string(x) + "\n";
					send(*sock, buf.c_str(), buf.length(),0);	
				}
				//	send(*sock, std::.c_str(), std::to_string(x).length(),0);	
			}
			else{
				buf = "ER " + std::to_string(err);
				send(*sock, buf.c_str(), buf.length(),0);
			}
		}
		else{
			int odds = hr->getHorseOdds(r,std::stoi(hstr),&err);
			if (err == HR_SUCCESS)
				buf = "OK " + std::to_string(odds);
			else
				buf = "ER " + std::to_string(err);
			send(*sock, buf.c_str(), buf.length(),0);
		}
	}
	else if (op == "Go"){
		string hstr = strToken(&req);
		enum HRErrorCode err;
		if (hstr == "" || hstr == "\n"){
			list<int> odds;
			err = HR_SUCCESS;
			int i = 0;
			hr->lock();
			while (err == HR_SUCCESS && i<NUM_HORSES_PER_RACE){
				odds.push_back(hr->getHorseOddsActive(i,&err));
				i++;
			}
			hr->unlock();
			if (err == HR_SUCCESS){
				buf = "OK " + std::to_string(NUM_HORSES_PER_RACE) + "\n";
				send(*sock, buf.c_str(), buf.length(),0);
				for (auto& x : odds){
					buf = std::to_string(x) + "\n";
					send(*sock, buf.c_str(), buf.length(),0);	
				}
			}
			else{
				buf = "ER " + std::to_string(err);
				send(*sock, buf.c_str(), buf.length(),0);
			}
		}
		else{
			int odds = hr->getHorseOddsActive(std::stoi(hstr),&err);
			if (err == HR_SUCCESS)
				buf = "OK " + std::to_string(odds);
			else
				buf = "ER " + std::to_string(err);
			send(*sock, buf.c_str(), buf.length(),0);
		}
	}
	else if (op == "G$"); // omitted
	else if (op == "GB"); // omitted
	else if (op == "Gb"); // omitted
	else if (op == "GW"){
		string r = strToken(&req);
		enum HRErrorCode err;
		if (r == "" || r == "\n"){
			int w = hr->getHouseWinningsAll();
			buf = "OK " + std::to_string(w);
			send(*sock, buf.c_str(), buf.length(),0);
		}
		else{
			int w = hr->getHouseWinnings(std::stoi(r),&err);
			if (err == HR_SUCCESS)
				buf = "OK " + std::to_string(w);
			else
				buf = "ER " + std::to_string(err);
			send(*sock, buf.c_str(), buf.length(),0);
		}
	} 
	else if (op == "Gw"){
		enum HRErrorCode err;
		int winnings = hr->getHouseWinningsActive(&err);
		if (err == HR_SUCCESS)
			buf = "OK " + std::to_string(winnings);
		else
			buf = "ER " + std::to_string(err);
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else if (op == "GT"){
		string r = strToken(&req);
		enum HRErrorCode err;
		if (r == "" || r == "\n"){
			//int w = hr->getHouseWinningsAll();
			buf = "ER " + std::to_string(HR_INVALID_RACE);
			send(*sock, buf.c_str(), buf.length(),0);
		}
		else{
			float w = hr->getHouseTake(std::stoi(r),&err);
			if (err == HR_SUCCESS)
				buf = "OK " + std::to_string(w);
			else
				buf = "ER " + std::to_string(err);
			send(*sock, buf.c_str(), buf.length(),0);
		}
	} 
	else if (op == "Gt"){
		enum HRErrorCode err;
		float w = hr->getHouseTakeActive(&err);
		if (err == HR_SUCCESS)
			buf = "OK " + std::to_string(w);
		else
			buf = "ER " + std::to_string(err);
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else if (op == "GV"){
		string r = strToken(&req);
		enum HRErrorCode err;
		if (r == "" || r == "\n"){
			//int w = hr->getHouseWinningsAll();
			buf = "ER " + std::to_string(HR_INVALID_RACE);
			send(*sock, buf.c_str(), buf.length(),0);
		}
		else{
			int w = hr->getWinner(std::stoi(r),&err);
			if (err == HR_SUCCESS)
				buf = "OK " + std::to_string(w);
			else
				buf = "ER " + std::to_string(err);
			send(*sock, buf.c_str(), buf.length(),0);
		}
	}
	else if (op == "Gv"){
		enum HRErrorCode err;
		int h = hr->getWinnerActive(&err);
		if (err == HR_SUCCESS)
			buf = "OK " + std::to_string(h);
		else
			buf = "ER " + std::to_string(err);
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else if (op == "AB"){
		int r = std::stoi(strToken(&req));
		int h = std::stoi(strToken(&req));
		int bet = std::stoi(strToken(&req));
		while ((req.front()) == ' '){
			req.erase(0,1);
		}
		while (req.back() == ' ' || req.back() == '\n'){
			req.pop_back();
		}
		err = hr->addBet(r,req,h,bet);
		if (err == HR_SUCCESS)
			buf = "OK";
		else
			buf = "ER " + std::to_string(err);
		send(*sock, buf.c_str(), buf.length(),0);
		if (err == HR_SUCCESS)
			writeBetListToFile(hr, r);
	}
	else if (op == "AP"){
		while ((req.front()) == ' '){
			req.erase(0,1);
		}
		while (req.back() == ' ' || req.back() == '\n'){
			req.pop_back();
		}
		hr->addParticipant(req);
		buf = "OK";
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else if (op == "SH"){
		int r = std::stoi(strToken(&req));
		int h = std::stoi(strToken(&req));
		while ((req.front()) == ' '){
			req.erase(0,1);
		}
		while (req.back() == ' ' || req.back() == '\n'){
			req.pop_back();
		}
		err = hr->setHorseName(r,h,req);
		if (err == HR_SUCCESS)
			buf = "OK";
		else
			buf = "ER " + std::to_string(err);
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else if (op == "Sh"){
		int h = std::stoi(strToken(&req));
		while ((req.front()) == ' '){
			req.erase(0,1);
		}
		while (req.back() == ' ' || req.back() == '\n'){
			req.pop_back();
		}
		err = hr->setHorseNameActive(h,req);
		if (err == HR_SUCCESS)
			buf = "OK";
		else
			buf = "ER " + std::to_string(err);
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else if (op == "ST"){
		float pct = std::stof(strToken(&req));
		string r = strToken(&req);
		if (r == "" || r == "\n")
			err = hr->setHouseTakeAll(pct);
		else
			err = hr->setHouseTake(std::stoi(r), pct);
		if (err == HR_SUCCESS)
			buf = "OK";
		else
			buf = "ER " + std::to_string(err);
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else if (op == "St"){
		float pct = std::stof(req);
		err = hr->setHouseTakeActive(pct);
		if (err == HR_SUCCESS)
			buf = "OK";
		else
			buf = "ER " + std::to_string(err);
		send(*sock, buf.c_str(), buf.length(),0);
	}
	else if (op == "SA"){
		int active;
		active = std::stoi(req);
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
		err = hr->setWinner(race,win);
		if (err == HR_SUCCESS){
			buf = "OK";
		}
		else{
			buf = "ER " + std::to_string(err);
		}
		send(*sock, buf.c_str(), buf.length(),0);
		if (err == HR_SUCCESS)
			writePayoutListToFile(hr, race);
	}
	else if (op == "Sv"){
		int win = std::stoi(req);
		hr->lock();
		err = hr->setWinnerActive(win);
		int r = hr->getActiveRace();
		hr->unlock();
		if (err == HR_SUCCESS){
			buf = "OK";
		}
		else{
			buf = "ER " + std::to_string(err);
		}
		send(*sock, buf.c_str(), buf.length(),0);
		if (err == HR_SUCCESS)
			writePayoutListToFile(hr, r);
	}
	else{
		iolk.lock();
		std::cerr << "INVALID OPERATION!!! "<< endl;
		iolk.unlock();
	}
}

void writePayoutListToFile(Horserace * hr, int r,string fname){
	std::ofstream payfile;
	std:: filebuf * fb = payfile.rdbuf();

	if (r>=0){
		list<Better> betters = hr->getBetterList(r);
		if (fname == "")
			fname = PAYOUT_PREFIX + std::to_string(r+1) + PAYOUT_SUFFIX;
		paylk[r].lock();
		fb->open(fname,std::ios::out);
		payfile << std::left << std::setw(35) << "Name" << "\t";
		payfile << "Winnings" << endl;
		for (auto& x: betters){
			int pay = x.getPayout();
			if (pay > 0){
				payfile << std::left << std::setw(35) << x.getName() << "\t";
				payfile << "$" << std::to_string(x.getPayout()) << endl;
			}
		}
		payfile << endl; 
		payfile << std::right << std::setw(35) << "House Winnings: ";
		payfile << "$" << std::to_string(hr->getHouseWinnings(r)) << endl;
		fb->close();
		paylk[r].unlock();
	}
}

void writeBetListToFile(Horserace * hr, int r,string fname){
	std::ofstream betfile;
	std:: filebuf * fb = betfile.rdbuf();
	if (r>=0){
		list<Better> betters = hr->getBetterList(r);
		if (fname == "")
			fname = BETLIST_PREFIX + std::to_string(r+1) + BETLIST_SUFFIX;
		betlk[r].lock();
		fb->open(fname,std::ios::out);
		betfile << std::left << std::setw(35) << "Name" << "\t";
		for (int i = 0 ; i < NUM_HORSES_PER_RACE; i++){
			betfile << "Horse " << i+1 << "  " ;
		}
		betfile << endl;
		for (auto& x: betters){
			x.print(&betfile);
		}

		fb->close();
		betlk[r].unlock();
	}
}