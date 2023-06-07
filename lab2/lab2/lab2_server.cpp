/*
* @File: lab2_server.cpp
* @Author: Xie Zhiwei
* @Description: 并发socket编程服务器
*/
#include <iostream>
#include <winsock2.h>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

class Server {
public:
    Server() : listenSocket(INVALID_SOCKET) {}

    bool Initialize() {
        // 初始化 Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Failed to initialize Winsock." << std::endl;
            return false;
        }

        // 创建监听套接字
        listenSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (listenSocket == INVALID_SOCKET) {
            std::cerr << "Failed to create socket." << std::endl;
            WSACleanup();
            return false;
        }

        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;  // 监听所有网络
        serverAddress.sin_port = htons(1026);

        // 绑定套接字到服务器地址
        if (bind(listenSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
            std::cerr << "Failed to bind socket." << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return false;
        }

        // 监听连接请求
        if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "Failed to listen for connections." << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return false;
        }

        return true;
    }

    void Run() {
        std::cout << "Server started. Waiting for connections..." << std::endl;

        fd_set readfds;
        int maxSocket = listenSocket;
        std::vector<SOCKET> clientSockets;

        while (true) {
            // 清空并设置文件描述符集合
            FD_ZERO(&readfds);
            FD_SET(listenSocket, &readfds);

            // 将客户端套接字添加到文件描述符集合中
            for (const SOCKET& clientSocket : clientSockets) {
                FD_SET(clientSocket, &readfds);
                maxSocket = max(maxSocket, clientSocket);
            }

            // 调用 select() 函数等待就绪的套接字
            if (select(maxSocket + 1, &readfds, NULL, NULL, NULL) == SOCKET_ERROR) {
                std::cerr << "Select error." << std::endl;
                break;
            }

            // 处理监听套接字上的连接请求
            if (FD_ISSET(listenSocket, &readfds)) {
                sockaddr_in clientAddress;
                int clientAddressSize = sizeof(clientAddress);
                SOCKET clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
                if (clientSocket == INVALID_SOCKET) {
                    std::cerr << "Failed to accept client connection." << std::endl;
                    continue;
                }

                std::cout << "New client connected. Socket: " << clientSocket << std::endl;
                clientSockets.push_back(clientSocket);
            }

            // 处理客户端套接字上的数据
            for (auto it = clientSockets.begin(); it != clientSockets.end(); ++it) {
                SOCKET clientSocket = *it;
                if (FD_ISSET(clientSocket, &readfds)) {
                    char buffer[1024];
                    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                    if (bytesRead == SOCKET_ERROR) {
                        std::cerr << "Failed to receive client request." << std::endl;
                    }
                    else if (bytesRead == 0) {
                        std::cout << "Client disconnected. Socket: " << clientSocket << std::endl;
                        closesocket(clientSocket);
                        clientSockets.erase(it);
                        --it;
                    }
                    else {
                        buffer[bytesRead] = '\0';
                        std::cout << "Received request from client " << clientSocket << ": " << buffer << std::endl;

                        time_t currentTime = time(NULL);
                        const char* timeString = ctime(&currentTime);
                        if (send(clientSocket, timeString, strlen(timeString), 0) == SOCKET_ERROR) {
                            std::cerr << "Failed to send server time to client " << clientSocket << "." << std::endl;
                        }
                    }
                }
            }
        }

        // 关闭所有客户端套接字
        for (const SOCKET& clientSocket : clientSockets) {
            closesocket(clientSocket);
        }

        closesocket(listenSocket);
        WSACleanup();
    }

private:
    SOCKET listenSocket;
};

int main() {
    Server server;
    if (server.Initialize()) {
        server.Run();
    }

    return 0;
}


