/*
* @File: raw.cpp
* @Author: Xie Zhiwei
* @Description: 原始套接字程序主函数
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
    // 初始化WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cerr << "Failed to initialize WinSock." << endl;
        return 1;
    }

    // 创建原始套接字
    SOCKET rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (rawSocket == INVALID_SOCKET)
    {
        cerr << "Failed to create raw socket. Error code: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // 绑定套接字
    struct sockaddr_in localAddress;
    memset(&localAddress, 0, sizeof(localAddress));
    localAddress.sin_family = AF_INET;
    localAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 指定本地地址
    localAddress.sin_port = htons(1026); // 指定要绑定的端口号
    if (bind(rawSocket, (struct sockaddr*)&localAddress, sizeof(localAddress)) == SOCKET_ERROR)
    {
        cerr << "Failed to bind socket. Error code: " << WSAGetLastError() << endl;
        closesocket(rawSocket);
        WSACleanup();
        return 1;
    }

    // 混杂模式
    DWORD mode = 1;
    char charRecv[10];
    DWORD dwBytesRet = 0;
    if (WSAIoctl(rawSocket, (IOC_IN | IOC_VENDOR | 1), &mode, sizeof(mode), charRecv, sizeof(charRecv), &dwBytesRet, NULL, NULL) == SOCKET_ERROR)
    {
        std::cerr << "设置混杂模式失败。\n";
        closesocket(rawSocket);
        WSACleanup();
        return 1;
    }


    // 设置套接字选项，启用IP头和TCP头的捕获
    BOOL optVal = TRUE;
    if (setsockopt(rawSocket, IPPROTO_IP, IP_HDRINCL, (const char*)&optVal, sizeof(BOOL)) == SOCKET_ERROR)
    {
        cerr << "Failed to set socket options. Error code: " << WSAGetLastError() << endl;
        closesocket(rawSocket);
        WSACleanup();
        return 1;
    }

    // 缓冲区用于接收数据
    const int bufferSize = 10000;
    char buffer[bufferSize];

    // 主循环
    while (1)
    {
        // 接收数据包
        int dataSize = recv(rawSocket, buffer, bufferSize, 0);
        if (dataSize == SOCKET_ERROR)
        {
            cerr << "Failed to receive data. Error code: " << WSAGetLastError() << endl;
            closesocket(rawSocket);
            WSACleanup();
            return 1;
        }

        // 解析数据包
        IPHeader* ipHeader = reinterpret_cast<IPHeader*>(buffer);
        TCPHeader* tcpHeader = reinterpret_cast<TCPHeader*>(buffer + ipHeader->ihl * 4);

        // 打印IP头信息
        cout << "--------------------------IP首部--------------------------\n";
        cout << *ipHeader << endl;

        // 打印TCP头信息
        cout << "--------------------------TCP首部--------------------------\n";
        cout << *tcpHeader << endl;

        // 打印数据部分
        int dataOffset = ipHeader->ihl * 4 + tcpHeader->thl * 4;

        cout << "Data: ";
        for (int i = dataOffset; i < dataSize && i < dataSize; ++i)
        {
            cout << hex << (static_cast<int>(buffer[i]) & 0xFF) << " ";
        }
        cout << endl;

        cout << "----------------------------END----------------------------" << endl;
    }

    // 关闭套接字
    closesocket(rawSocket);

    // 清理WinSock
    WSACleanup();

    return 0;
}