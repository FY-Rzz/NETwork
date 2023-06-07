/*
* @File: raw.h
* @Author: Xie Zhiwei
* @Description: ԭʼ�׽��ֳ���������������raw.cppҪ�õ���ͷ��
*/
#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

// IPͷ
typedef struct IPHeader
{
	u_char ihl : 4,	version : 4;	/*�ײ����� + �汾*/	
	u_char tos;			/*��������*/
	u_short tot_len;	/*�ܳ���*/
	u_short id;			/*��ʶ*/
	u_short flag_off;	/*��Ƭƫ��*/
	u_char ttl;			/*����ʱ��*/
	u_char protocol;	/*Э������*/
	u_short sum;	/*У���*/
	u_long saddr;		/*Դ��ַ*/
	u_long daddr;		/*Ŀ�ĵ�ַ*/

	// ���ͷ
	friend std::ostream& operator<<(std::ostream& os, const IPHeader& ipHeader)
	{
		os << "IP Header" << std::endl;
		os << "Version: " << static_cast<int>(ipHeader.version) << std::endl;
		os << "IHL: " << static_cast<int>(ipHeader.ihl) << std::endl;
		os << "TOS: " << static_cast<int>(ipHeader.tos) << std::endl;
		os << "Total Length: " << ntohs(ipHeader.tot_len) << std::endl;
		os << "Identification: " << ntohs(ipHeader.id) << std::endl;
		os << "Flags Offset: " << ntohs(ipHeader.flag_off) << std::endl;
		os << "TTL: " << static_cast<int>(ipHeader.ttl) << std::endl;
		os << "Protocol: " << static_cast<int>(ipHeader.protocol) << std::endl;
		os << "Sum: " << ntohs(ipHeader.sum) << std::endl;
		os << "Source Address: " << inet_ntoa(*(in_addr*)&ipHeader.saddr) << std::endl;
		os << "Destination Address: " << inet_ntoa(*(in_addr*)&ipHeader.daddr) << std::endl;

		return os;
	}
}IPHeader;

// TCPͷ
typedef struct TCPHeader
{
	u_short sport;		/*Դ�˿�*/
	u_short dport;		/*Ŀ�Ķ˿�*/
	u_int seq;			/*���*/
	u_int ack_seq;		/*ȷ�����*/
	u_char res1 : 4, thl : 4;		/*���� + �ײ�����*/
	u_char flag;		/*��־*/
	u_short win;		/*���ڴ�С*/
	u_short sum;	/*У���*/
	u_short urp;		/*����ָ��*/

	// ���ͷ
	friend std::ostream& operator<<(std::ostream& os, const TCPHeader& tcpHeader) 
	{
		os << "TCP Header" << std::endl;
		os << "Source Port: " << ntohs(tcpHeader.sport) << std::endl;
		os << "Destination Port: " << ntohs(tcpHeader.dport) << std::endl;
		os << "Sequence Number: " << ntohl(tcpHeader.seq) << std::endl;
		os << "Acknowledgement Number: " << ntohl(tcpHeader.ack_seq) << std::endl;
		os << "Data Offset: " << static_cast<int>(tcpHeader.thl) << std::endl;
		os << "Flags: " << static_cast<int>(tcpHeader.flag) << std::endl;
		os << "Window Size: " << ntohs(tcpHeader.win) << std::endl;
		os << "Sum: " << ntohs(tcpHeader.sum) << std::endl;
		os << "Urgent Pointer: " << ntohs(tcpHeader.urp) << std::endl;

		return os;
	}
}TCPHeader;