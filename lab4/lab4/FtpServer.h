/*
* @File: FtpServer.h
* @Author: Xie Zhiwei
* @Description: ftp服务器头文件
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

class Client // 客户端信息
{
public:
    sockaddr_in addr; // 客户端地址
    std::string filename; // 文件名
    std::string op;         // 操作
    SOCKET socket;        // 套接字

public:
    // getter & setter
    void setFilename(std::string newname) { filename = newname; }
    std::string getFilename() { return filename; }
    void setOp(std::string op) { this->op = op; }
    std::string getOp() { return op; }
    void setSocket(SOCKET soc) { socket = soc; }
    SOCKET getSocket() { return socket; }

    // 构造
    Client(SOCKET sock) {
        socket = sock;
    }
};

// 定义任务类型
using Task = std::function<void()>;

class FtpServer  // FTP服务器
{
private:
    struct sockaddr_in ServerAddr;                           // 服务器地址
    int socket;                                                // 套接字
    fd_set clientSet;                                        // 客户端集合
    size_t maxfd;                                            // 客户端最大套接字数量
    std::unordered_map<SOCKET, std::shared_ptr<Client>> clientSockets; // 客户端套接字集合
public:
    FtpServer();                                // 构造函数
    ~FtpServer();                               // 析构函数
    void recvNewJob();                          // 接收新任务
    SOCKET acceptClient();                      // 接收客户端
    bool getFile(SOCKET sc);                    // 获取文件的信息和操作
    bool handleFile(std::shared_ptr<Client> client); // 处理文件
    bool down(std::shared_ptr<Client> client);       // 下载文件
    bool up(std::shared_ptr<Client> client);         // 上传文件
    void disconnectClient(SOCKET sc);           // 断开客户端连接
    bool list(std::shared_ptr<Client> client);       // 获取文件列表

private:
    // 线程池相关成员变量
    std::vector<std::thread> threadPool; // 线程池
    std::queue<Task> taskQueue;          // 任务队列
    std::mutex queueMutex;                  // 互斥量
    std::condition_variable queueCondVar;    // 条件变量
    bool stop;                          // 停止标志
};