#include<iostream>
#include<string.h>
#include<winsock2.h>
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
	 ========     ====              ===========     =============   ====      ====    ============
   ============   ====             =============   ===============  ======    ====    ============
 ======    ====   ====                 =====       =====            ========  ====        ====
 ====             ====                 =====       ============     ==============        ====
 ====             ====                 =====       ============     ==============        ====
 ======    ====   ====                 =====       =====            ====  ========        ====
   ============   ==============   =============   ===============  ====    ======        ====
	 ========     ==============    ===========     =============   ====      ====        ====
==================================================================================================

*/
int main()
{
	//initialize a winsock
	WSADATA wsaData;
	WORD ver = MAKEWORD(2, 2);//version 2.2

	int wsok = WSAStartup(ver, &wsaData);
	if (wsok != 0)
	{
		cerr << "------- Client: Cannot initialize winsock, quitting.-------" << endl;
		return 0;
	}
	else {
		cout << "=====================================================================================" << endl;
		cout << "    =======     ===             =========    ============  ===      ===  ============" << endl;
		cout << " ============   ===            ===========   ============  =====    ===  ============" << endl;
	
	}

	
	//start a socket
	
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//AF_INET=2,The Internet Protocol version 4 (IPv4) address family, TCP protocol
	if (sockClient == INVALID_SOCKET)
	{
		cerr << "-------Client: socket() failed, error: "<<WSAGetLastError()<<"-------"<<endl;
		WSACleanup();//clean up
		return 0;//exit
	}
	else {
		cout << "=====      ===  ===                ===       ====          =======  ===      ====" << endl;
		cout << "===             ===                ===       ===========   ============      ====" << endl;
		cout << "===             ===                ===       ===========   ============      ====" << endl;
	}
	//connect to a server
	
	
	sockaddr_in addrServer;////assume server's IP address is 127.0.0.1 (localhost)
	addrServer.sin_family = AF_INET;//IPv4
	addrServer.sin_port = htons(PORT);
	addrServer.sin_addr.S_un.S_addr = inet_addr(IP_ADDR);

	int infoC=connect(sockClient, (SOCKADDR*)&addrServer, sizeof(addrServer));
	if (infoC != 0) {
		cerr << "------- Client: connect() failed. Error:" << WSAGetLastError() << "-------" << endl;
		WSACleanup();
		return 0;
	}
	else
	{
		cout << "=====      ===  ===                ===       =====         ===  =======      ====" << endl;
		cout << " ============   ============   ===========   ============  ===    =====      ====" << endl;
		cout << "   =======      ============    =========    ============  ===      ===      ====" << endl;

	}

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% c/s���ӳɹ������Կ�ʼ���෢����Ϣ %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/////
	cout << "================================== ALL SET BEGIN CAHT ===============================" << endl;

	while (true) {
		//send message to server.

		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% ��ʼ������/���ܵ���Ϣmsg %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/////

		char recvBuf[BUFLEN];
		char sendBuf[BUFLEN];
		memset(&recvBuf, 0, sizeof(recvBuf));
		memset(&sendBuf, 0, sizeof(sendBuf));//��Ҫ���ϣ���Ȼstrcpy_s��������ִ���
		msg sendMsg;
		msg recvMsg;

		//cout << "-------Client: Type your message.-------" << endl;
		
		//getline from input
		
		char input[BUFLEN];
		cin.getline(input, BUFLEN);
		sendMsg.content = input;

		if (strcmp(input, "quit") == 0 || strcmp(input, "�˳�") == 0) {
			sendMsg.type = QUIT;
		}
		else {
			if (strcmp(input, "offline") == 0 || strcmp(input, "����") == 0) {
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

		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% ��Ϣ���͡�ʱ��������ݾ��������ɣ�����%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/////


		int byteSent,byteRecv;
		byteSent = send(sockClient, sendBuf, sizeof(sendBuf), 0);
		memset(&sendBuf, 0, sizeof(sendBuf));

		if (sendMsg.type ==2 ) {
			WSACleanup();
			cout << "CLIENT | "<<sendMsg.timestamp <<" | ASK TO OFFLINE" << endl;
			break;
		}

		if (byteSent == SOCKET_ERROR) {
			cerr << "-------Client: send() failed. error:" << WSAGetLastError() <<"-------" << endl;
			break;
		}
		else {
			//cout << "-------Client: send() ready.-------" << endl;
			
		
		
		byteRecv = recv(sockClient, recvBuf, sizeof(recvBuf), 0);
		string recvd = recvBuf;
		//cout << "recvBuf: " << recvBuf << endl;
		//cout << "string: " << recvd << endl;
		recvMsg = stom(recvd);
		memset(&recvBuf, 0, sizeof(recvBuf));
		if (byteRecv == SOCKET_ERROR) {
			cerr << "-------Client: recv() from server failed. Error: " << WSAGetLastError() <<"-------" << endl;
			break;
		}
		
		if (recvMsg.type==1) {//����quit��Ϣ�ֶΣ�����յ�����Ϣ��quit��ֱ���˳�socket
			WSACleanup();
			cout << "SERVER | " << recvMsg.timestamp << " | ASK TO QUIT" << endl;
			break;
		}
		else{ //recvMsg.type=OTHER

			// print received msg from server
			cout << "SERVER | "<<recvMsg.timestamp<<" | " << recvMsg.content << endl;
			}

		}
	}

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% �����Ϣ���� �˳� %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%/////

	closesocket(sockClient);
	cout << "================================ CLIENT: SOCKET CLOSED ==============================" << endl;

	WSACleanup();

	system("pause");
	return 0;
}
