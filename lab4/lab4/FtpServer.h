/*
* @File: FtpServer.h
* @Author: Xie Zhiwei
* @Description: ftp������ͷ�ļ�
*/
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iomanip>
#include <unordered_map>

#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE 4096

class Client // �ͻ�����Ϣ
{
public:
    sockaddr_in addr; // �ͻ��˵�ַ
    std::string filename; // �ļ���
    std::string op;         // ����
    SOCKET socket;        // �׽���

public:
    // getter & setter
    void setFilename(std::string newname) { filename = newname; }
    std::string getFilename() { return filename; }
    void setOp(std::string op) { this->op = op; }
    std::string getOp() { return op; }
    void setSocket(SOCKET soc) { socket = soc; }
    SOCKET getSocket() { return socket; }

    // ����
    Client(SOCKET sock) {
        socket = sock;
    }
};

// ������������
using Task = std::function<void()>;

class FtpServer  // FTP������
{
private:
    struct sockaddr_in ServerAddr;                           // ��������ַ
    int socket;                                                // �׽���
    fd_set clientSet;                                        // �ͻ��˼���
    size_t maxfd;                                            // �ͻ�������׽�������
    std::unordered_map<SOCKET, std::shared_ptr<Client>> clientSockets; // �ͻ����׽��ּ���
public:
    FtpServer();                                // ���캯��
    ~FtpServer();                               // ��������
    void recvNewJob();                          // ����������
    SOCKET acceptClient();                      // ���տͻ���
    bool getFile(SOCKET sc);                    // ��ȡ�ļ�����Ϣ�Ͳ���
    bool handleFile(std::shared_ptr<Client> client); // �����ļ�
    bool down(std::shared_ptr<Client> client);       // �����ļ�
    bool up(std::shared_ptr<Client> client);         // �ϴ��ļ�
    void disconnectClient(SOCKET sc);           // �Ͽ��ͻ�������
    bool list(std::shared_ptr<Client> client);       // ��ȡ�ļ��б�

private:
    // �̳߳���س�Ա����
    std::vector<std::thread> threadPool; // �̳߳�
    std::queue<Task> taskQueue;          // �������
    std::mutex queueMutex;                  // ������
    std::condition_variable queueCondVar;    // ��������
    bool stop;                          // ֹͣ��־
};