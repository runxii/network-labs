#pragma once
#include<iostream>
#include<winsock2.h>
#include<string.h>
#include<stdio.h>
#include<time.h>

#pragma comment(lib,"ws2_32.lib")
using namespace std;

enum msgType {//��Ϣ������
	QUIT=1,
	OFFLINE,
	OTHER
};

struct msg {
	msgType type;
	string timestamp;//ʱ��������ڴ�����Ϣ����Ϊ��һ����
	string content;//������Ϣ������
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
	int pos = 4;//msg��type��pos=0,pos=1��pos=3�Ƿָ����" | "
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




