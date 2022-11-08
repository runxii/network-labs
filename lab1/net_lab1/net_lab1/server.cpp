#include<iostream>
#include<winsock2.h>
#include<string.h>
#include<stdio.h>
#include<time.h>
#include"protocol.h"

//need to link with ws2_32.lib
#pragma comment(lib,"ws2_32.lib")

using namespace std;
#define BUFLEN 512
#define PORT 7171
#define IP_ADDR "127.0.0.1"

/*
==================================================================================================
  ========       ==============  ============      ===       ===   ==============   ============
============     ==============  ==============   =====     =====  ==============   ==============
====      ==     ====            =====     =====  =====     =====  ====             ====      ====
============     ============    =====    ======  ======   ======  ============     ====    ======
  ============   ============    ===============  ==============   ============     ==============
==        ====   ====            =============      ==========     ====             ============
==============   ==============  =====  ========       =====       ==============   ====  ========
  ==========     ==============  ====     =====          =         ==============   ====    ======
==================================================================================================

*/

int main()
{
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 初始化server；create，bind，listen %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/////
	//initialize the winsock
	WSADATA wsaData;
	WORD ver = MAKEWORD(2, 2);//version 2.2

	int wsok = WSAStartup(ver, &wsaData);

	if (wsok != 0)
	{
		cerr << "-------Server: Cannot initialize winsock, error: "<< WSAGetLastError() << "-------" << endl;
		return 0;
	}
	else
	{
		cout << "=====================================================================================" << endl;
		cout << "  =======       ===========  ==========     ===      ===   ============  ==========   " << endl;
	}
	//create a socket
	

	SOCKET sockServer = socket(AF_INET, SOCK_STREAM, 0);
	if (sockServer == INVALID_SOCKET)
	{
		cerr << "-------Server: socket() failed, error: "<< WSAGetLastError()<< "-------" << endl; //error handling
		return 0;
	}
	else {
		cout << "============    ===========  ============  =====    =====  ============  ============ " << endl;
		cout << "====      ==    ====         ====     ==== =====    =====  ====          ===     ==== " << endl;
	}
	//bind the socket to an ip address and port

	sockaddr_in  addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;//IPv4
	addr.sin_port = htons(PORT);//host to network short. 7171 is the listening port
	addr.sin_addr.S_un.S_addr = inet_addr(IP_ADDR);//也可以使用inet_pton

	int infoS=bind(sockServer, (SOCKADDR*)&addr, sizeof(addr));
	if (infoS == SOCKET_ERROR)
	{
		cerr << "-------Server: bind() error," << WSAGetLastError() << "-------" << endl;
		return 0;
	}
	else
	{
		cout << "============    =========    ====    ===== ======  ======  =========     ===    ===== " << endl;
		cout << "  ===========   =========    ============  ==============  =========     =========== " << endl;
	}
	//tell winsock the socket is for listening
	int listenInfo=listen(sockServer, 5);
	if (listenInfo == SOCKET_ERROR) {
		cerr << "-------Server: listen() faild. error: " << WSAGetLastError() << "-------" << endl;
		closesocket(sockServer);
		WSACleanup();
		return 0;
	}
	else
	{
		cout << "==        ===   ====         ===========    ==========     ====          =========   " << endl;
		cout << "=============   ===========  ====  ======      ======      ============  ===  ======" << endl;
	}

	//waite for a connection

	sockaddr_in addrClient;
	int lenClient = sizeof(addrClient);
	

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 建立client和service的连接 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/////
	SOCKET sockConn = accept(sockServer, (SOCKADDR*)&addrClient, &lenClient);
	if (sockConn == INVALID_SOCKET)
	{
		cerr << "-------Server: accept() faild. error: " << WSAGetLastError() << "-------" << endl;
		closesocket(sockConn);
		WSACleanup();
		return 0;
	}
	else
	{
		cout << "  =========     ===========  ===     =====       =         ============  ===    =====" << endl;
	}
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% c/s连接成功，可以开始互相发送消息 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/////
	cout << "================================== ALL SET BEGIN CAHT ===============================" << endl;


	while (true){
		// receive untill client ask for quitting

		char sendBuf[BUFLEN];
		char recvBuf[BUFLEN];
		memset(sendBuf, 0, sizeof(sendBuf));
		memset(recvBuf, 0, sizeof(recvBuf));
		msg recvMsg;
		msg sendMsg;
		int byteRecv, byteSent;

		byteRecv = recv(sockConn, recvBuf, sizeof(recvBuf) , 0);
		if (byteRecv == SOCKET_ERROR)
		{
			cerr << "-------Server: recv() failed. Error:"<<WSAGetLastError()<< "-------" << endl;
			closesocket(sockConn);
			WSACleanup();
			break;
		}

		if (byteRecv == 0)
		{
			cout << "-------Server: Client disconnected anomally.-------" << endl;
			closesocket(sockConn); //close connection socket
			break;
		}
		else
		{
			//	cout << "-------Server: received. -------" <<endl;
		}
		string recvd = recvBuf;
		recvMsg = stom(recvd);
		
		if (recvMsg.type==2) {//定义quit消息字段，如果收到的信息是quit就直接退出socket
			closesocket(sockConn);
			WSACleanup();
			cout << "CLIENT | " << recvMsg.timestamp <<" | ASK TO OFFLINE" << endl;
			break;
		}
		else{ //recvMsg.type=OTHER

			// print received msg from client
			cout << "CLIENT | "<<recvMsg.timestamp<<" | " << recvMsg.content << endl;
			memset(recvBuf, 0, sizeof(recvBuf));

			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 成功接收消息，开始回复client %%%%%%%%%%%%%%%%%%%%///
			//cout << "-------Server: Type your reply.-------" << endl;
			char input[BUFLEN];
			cin.getline(input, BUFLEN);
			sendMsg.content = input;
			if (strcmp(input, "quit") == 0 || strcmp(input, "退出") == 0) {
				sendMsg.type = QUIT;
			}
			else {
				if (strcmp(input, "offline") == 0 || strcmp(input, "下线") == 0) {
					sendMsg.type = OFFLINE;
				}
				else
					sendMsg.type = OTHER;
			}
			//add timestamp to sendBuf
			struct tm stime;
			time_t now = time(0);
			localtime_s(&stime, &now);

			char tmp[32] = { NULL };
			strftime(tmp, sizeof(tmp), "%Y-%m-%d-%H:%M:%S", &stime);
			sendMsg.timestamp = tmp;

			strcpy_s(sendBuf, mtos(sendMsg).c_str());
			
			/*cout << "sentBuf: " << sendBuf << endl;
			cout << "sendMsg type: " << sendMsg.type << endl;*/
			byteSent = send(sockConn, sendBuf, sizeof(sendBuf), 0);
			if (byteSent == SOCKET_ERROR) {
				cerr << "-------Server: send() failed, error:" << WSAGetLastError() << "-------" << endl;
				break;
			}
			else {
				//cout << "-------Server: message sent.-------" << endl;
				memset(sendBuf, 0, sizeof(sendBuf));// clean sentbuf

				if (sendMsg.type == 1) {
					WSACleanup();
					cout << "SERVER | " << sendMsg.timestamp << " | ASK TO QUIT" << endl;
					break;
				}
			}
		}
		
}

	// close the socket
	closesocket(sockConn);
	//cout << "-------Server: client disconneced successfully.-------" << endl;
	//close the listening socket

	closesocket(sockServer);
	cout << "================================ SERVER: SOCKET CLOSED ==============================" << endl;


	//shutdown winsock
	WSACleanup();

	system("pause");
	return 0;
}

