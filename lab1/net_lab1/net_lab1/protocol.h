#pragma once
#include<iostream>
#include<winsock2.h>
#include<string.h>
#include<stdio.h>
#include<time.h>

#pragma comment(lib,"ws2_32.lib")
using namespace std;

enum msgType {//消息的类型
	QUIT=1,
	OFFLINE,
	OTHER
};

struct msg {
	msgType type;
	string timestamp;//时间戳，放在传输消息中作为其一部分
	string content;//传输消息的内容
};

//method: msg to stirng
string mtos(msg m) {
	string s;
	if (m.type == 1) {
		s = '1';
	}
	else {
		if (m.type == 2)
			s = '2';
		else
			s = '3';
	}
	s.append(" | ");
	s.append(m.timestamp);
	s.append(" | ");
	s.append(m.content);
	return s;
}

msg stom(string s) {
	msg m;
	int pos = 4;//msg的type是pos=0,pos=1到pos=3是分割符号" | "
	if (s[0] == '1')
		m.type = QUIT;
	else {
		if (s[0] == '2')
			m.type = OFFLINE;
		else
		{
			m.type = OTHER;
		}
	}
	while (s[pos] != ' ') {
		pos++;
	}
	m.timestamp = s.substr(4, pos - 4);
	m.content = s.substr(pos + 3);
	return m;
}




