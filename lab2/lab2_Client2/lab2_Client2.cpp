/*
* @File: lab2_Client.cpp
* @Author: Xie Zhiwei
* @Description: 并发套接字编程客户端程序
*/
#include <iostream>
#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main() {
    // 初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    // 创建监听套接字
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    // 设置服务器地址
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(1026);

    // 连接服务器
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to the server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    while (true) {
        char* request = new char[100];
        std::cin >> request;
        if (strcmp(request, "exit") == 0)
        {
            break;
        }
        // 发送请求给服务器
        if (send(clientSocket, request, strlen(request), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send request to the server." << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        char buffer[1024];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead == SOCKET_ERROR) {
            std::cerr << "Failed to receive server response." << std::endl;
        }
        else {
            buffer[bytesRead] = '\0';
            std::cout << "Server time: " << buffer << std::endl;
        }
    }


    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
