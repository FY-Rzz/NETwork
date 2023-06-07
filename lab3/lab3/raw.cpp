/*
* @File: raw.cpp
* @Author: Xie Zhiwei
* @Description: ԭʼ�׽��ֳ���������
*/
#include <iostream>
#include <WinSock2.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "raw.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

using namespace std;

int main()
{
    // ��ʼ��WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cerr << "Failed to initialize WinSock." << endl;
        return 1;
    }

    // ����ԭʼ�׽���
    SOCKET rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (rawSocket == INVALID_SOCKET)
    {
        cerr << "Failed to create raw socket. Error code: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // ���׽���
    struct sockaddr_in localAddress;
    memset(&localAddress, 0, sizeof(localAddress));
    localAddress.sin_family = AF_INET;
    localAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // ָ�����ص�ַ
    localAddress.sin_port = htons(1026); // ָ��Ҫ�󶨵Ķ˿ں�
    if (bind(rawSocket, (struct sockaddr*)&localAddress, sizeof(localAddress)) == SOCKET_ERROR)
    {
        cerr << "Failed to bind socket. Error code: " << WSAGetLastError() << endl;
        closesocket(rawSocket);
        WSACleanup();
        return 1;
    }

    // ����ģʽ
    DWORD mode = 1;
    char charRecv[10];
    DWORD dwBytesRet = 0;
    if (WSAIoctl(rawSocket, (IOC_IN | IOC_VENDOR | 1), &mode, sizeof(mode), charRecv, sizeof(charRecv), &dwBytesRet, NULL, NULL) == SOCKET_ERROR)
    {
        std::cerr << "���û���ģʽʧ�ܡ�\n";
        closesocket(rawSocket);
        WSACleanup();
        return 1;
    }


    // �����׽���ѡ�����IPͷ��TCPͷ�Ĳ���
    BOOL optVal = TRUE;
    if (setsockopt(rawSocket, IPPROTO_IP, IP_HDRINCL, (const char*)&optVal, sizeof(BOOL)) == SOCKET_ERROR)
    {
        cerr << "Failed to set socket options. Error code: " << WSAGetLastError() << endl;
        closesocket(rawSocket);
        WSACleanup();
        return 1;
    }

    // ���������ڽ�������
    const int bufferSize = 10000;
    char buffer[bufferSize];

    // ��ѭ��
    while (1)
    {
        // �������ݰ�
        int dataSize = recv(rawSocket, buffer, bufferSize, 0);
        if (dataSize == SOCKET_ERROR)
        {
            cerr << "Failed to receive data. Error code: " << WSAGetLastError() << endl;
            closesocket(rawSocket);
            WSACleanup();
            return 1;
        }

        // �������ݰ�
        IPHeader* ipHeader = reinterpret_cast<IPHeader*>(buffer);
        TCPHeader* tcpHeader = reinterpret_cast<TCPHeader*>(buffer + ipHeader->ihl * 4);

        // ��ӡIPͷ��Ϣ
        cout << "--------------------------IP�ײ�--------------------------\n";
        cout << *ipHeader << endl;

        // ��ӡTCPͷ��Ϣ
        cout << "--------------------------TCP�ײ�--------------------------\n";
        cout << *tcpHeader << endl;

        // ��ӡ���ݲ���
        int dataOffset = ipHeader->ihl * 4 + tcpHeader->thl * 4;

        cout << "Data: ";
        for (int i = dataOffset; i < dataSize && i < dataSize; ++i)
        {
            cout << hex << (static_cast<int>(buffer[i]) & 0xFF) << " ";
        }
        cout << endl;

        cout << "----------------------------END----------------------------" << endl;
    }

    // �ر��׽���
    closesocket(rawSocket);

    // ����WinSock
    WSACleanup();

    return 0;
}