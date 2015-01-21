#include "hrserver.h"

int main (void){

	WSADATA wsaData;

	int ret;
	char inbuf [BUFLEN];

//	struct addrinfo* result = nullptr;
//	struct addrinfo* ptr = nullptr;
//	struct addrinfo hints;
	SOCKET listensocket = INVALID_SOCKET;
	SOCKET clientsocket = INVALID_SOCKET;
	ret = createListenSocket(&listensocket, &wsaData);
	/*
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
		cout << "Startup Failed!" << endl;
		cin.sync();
		cin.get();
		return -1;
	}

	ZeroMemory(&hints, sizeof(hints)); // dumb?
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(nullptr,PORT,&hints,&result) != 0){
		cout << "getaddrinfo failed! " << endl;
		cin.sync();
		cin.get();
		WSACleanup();
		return -1;
	}

	listensocket = socket(result->ai_family,result->ai_socktype, result->ai_protocol);

	if (listensocket == INVALID_SOCKET){
		cout << "socket() failed! : " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		cin.sync();
		cin.get();
		return -1;
	}
	
	if (bind(listensocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listensocket);
        WSACleanup();
        return -1;
    }

	freeaddrinfo(result);
*/
	cout << "Set-up OK!" << endl;

	if ( listen( listensocket, SOMAXCONN ) == SOCKET_ERROR ) {
		cout << "listen() failed!! : " << WSAGetLastError() << endl;
		closesocket(listensocket);
		WSACleanup();
		return -1;
	}
	
	clientsocket = accept(listensocket, nullptr, nullptr);
	if (clientsocket == INVALID_SOCKET){
		cout << "accept() failed!! : " << WSAGetLastError() << endl;
		closesocket(listensocket);
		WSACleanup();
		return -1;
	}

	if (ret !=0 ){
		cin.sync();
		cin.get();
		return ret;
	}

	do{
		ZeroMemory(inbuf, BUFLEN*sizeof(char));
		ret = recv(clientsocket, inbuf, BUFLEN, 0);

		if (ret > 0){
			cout << "Recieved Message (" << strlen(inbuf) << ") : \"" << inbuf << "\"" << endl;
		}
		else if (ret == 0){
			cout << "End of Connection." << endl;
		}
		else{
			cout << "recv() failed! : " << WSAGetLastError() << endl;
			closesocket(listensocket);
			WSACleanup();
			return -1;
		}

	}while (ret>0);

	cin.sync();
	cin.get();
    closesocket(listensocket);
    WSACleanup();
	return 0;
}



int createListenSocket(SOCKET* sock, WSADATA* wsaData){

	struct addrinfo* result = nullptr;
	struct addrinfo* ptr = nullptr;
	struct addrinfo hints;
	//SOCKET listensocket = INVALID_SOCKET;
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