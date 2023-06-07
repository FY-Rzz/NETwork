/*
* @File: FTPClient.h
* @Author: Xie Zhiwei
* @Description: 声明了FTP客户端所需各种结构和FTP客户端类
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

struct File // 文件
{
    std::string filename; // 文件名
    int opId;         // 操作ID
    File(std::string name) {
        filename = name;
    }
};

class FtpClient // FTP客户端
{
    struct sockaddr_in ServerAddr; /* Server address */
    char remoteHostname[20];
    int sock;

public:
    FtpClient();
    ~FtpClient();

    void run(); // 运行

    bool down(File& f); // 下载
    bool up(File& f);   // 上传
    bool list();        // 列出文件
    bool quit();        // 退出
};