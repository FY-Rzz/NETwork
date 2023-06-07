/*
* @File: raw.h
* @Author: Xie Zhiwei
* @Description: 原始套接字程序声明，声明了raw.cpp要用到的头部
*/
#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

// IP头
typedef struct IPHeader
{
	u_char ihl : 4,	version : 4;	/*首部长度 + 版本*/	
	u_char tos;			/*服务类型*/
	u_short tot_len;	/*总长度*/
	u_short id;			/*标识*/
	u_short flag_off;	/*分片偏移*/
	u_char ttl;			/*生存时间*/
	u_char protocol;	/*协议类型*/
	u_short sum;	/*校验和*/
	u_long saddr;		/*源地址*/
	u_long daddr;		/*目的地址*/

	// 输出头
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

// TCP头
typedef struct TCPHeader
{
	u_short sport;		/*源端口*/
	u_short dport;		/*目的端口*/
	u_int seq;			/*序号*/
	u_int ack_seq;		/*确认序号*/
	u_char res1 : 4, thl : 4;		/*保留 + 首部长度*/
	u_char flag;		/*标志*/
	u_short win;		/*窗口大小*/
	u_short sum;	/*校验和*/
	u_short urp;		/*紧急指针*/

	// 输出头
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