/*lab1_server.cpp: 实验一服务器程序
* 
*/
#include <iostream>
#include <winsock2.h>
#pragma comment (lib,"ws2_32.lib")//加载

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    // 创建套接字
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    // 设置服务器地址和端口
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;  // 监听所有网络接口
    serverAddress.sin_port = htons(1026);

    // 绑定套接字到服务器地址和端口
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // 监听连接请求
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen for connections." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // 接受客户端连接请求
    SOCKET clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to accept client connection." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // 接收客户端发送的请求
    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead == SOCKET_ERROR) {
        std::cerr << "Failed to receive client request." << std::endl;
    }
    else {
        buffer[bytesRead] = '\0';
        std::cout << "Received request: " << buffer << std::endl;
    }

    // 获取本机时间并发送给客户端
    time_t currentTime = time(NULL);
    const char* timeString = ctime(&currentTime);
    if (send(clientSocket, timeString, strlen(timeString), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send server time." << std::endl;
    }

    system("pause");

    // 关闭套接字
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

}
