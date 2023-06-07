/*
* @File: FTPClient.h
* @Author: Xie Zhiwei
* @Description: ������FTP�ͻ���������ֽṹ��FTP�ͻ�����
*/
#pragma once
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <fstream>
#include <memory>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE 4096

struct File // �ļ�
{
    std::string filename; // �ļ���
    int opId;         // ����ID
    File(std::string name) {
        filename = name;
    }
};

class FtpClient // FTP�ͻ���
{
    struct sockaddr_in ServerAddr; /* Server address */
    char remoteHostname[20];
    int sock;

public:
    FtpClient();
    ~FtpClient();

    void run(); // ����

    bool down(File& f); // ����
    bool up(File& f);   // �ϴ�
    bool list();        // �г��ļ�
    bool quit();        // �˳�
};