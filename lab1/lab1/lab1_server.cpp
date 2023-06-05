/*lab1_server.cpp: ʵ��һ����������
* 
*/
#include <iostream>
#include <winsock2.h>
#pragma comment (lib,"ws2_32.lib")//����

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    // �����׽���
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    // ���÷�������ַ�Ͷ˿�
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;  // ������������ӿ�
    serverAddress.sin_port = htons(1026);

    // ���׽��ֵ���������ַ�Ͷ˿�
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // ������������
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen for connections." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // ���ܿͻ�����������
    SOCKET clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to accept client connection." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // ���տͻ��˷��͵�����
    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead == SOCKET_ERROR) {
        std::cerr << "Failed to receive client request." << std::endl;
    }
    else {
        buffer[bytesRead] = '\0';
        std::cout << "Received request: " << buffer << std::endl;
    }

    // ��ȡ����ʱ�䲢���͸��ͻ���
    time_t currentTime = time(NULL);
    const char* timeString = ctime(&currentTime);
    if (send(clientSocket, timeString, strlen(timeString), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send server time." << std::endl;
    }

    system("pause");

    // �ر��׽���
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

}
