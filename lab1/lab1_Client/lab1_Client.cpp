// lab1_Client.cpp : 实验一客户端程序
//

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#pragma comment (lib,"ws2_32.lib")//加载

int main()
{
    // 初始化Winsock库
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    // 创建套接字
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    // 设置服务器地址和端口
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(1026);

    // 连接到服务器
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to the server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // 发送请求
    const char* request = "请求消息";
    if (send(clientSocket, request, strlen(request), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send request to the server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // 接收服务器发送的时间信息
    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead == SOCKET_ERROR) {
        std::cerr << "Failed to receive server response." << std::endl;
    }
    else {
        buffer[bytesRead] = '\0';
        std::cout << "Server time: " << buffer << std::endl;
    }

    system("pause");

    // 关闭套接字
    closesocket(clientSocket);
    WSACleanup();

}


