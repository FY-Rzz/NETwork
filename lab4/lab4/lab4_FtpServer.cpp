/*
* @File: lab4_FtpServer.cpp
* @Author: Xie Zhiwei
* @Description: ftp服务器实现文件
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <Windows.h>
#include <filesystem>
#include <sstream>
#include "FtpServer.h"

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT 12345
#define MAX_BUFFER_SIZE 4096


//// 构造函数
//FtpServer::FtpServer()
//{
//    // 初始化Winsock
//    WSADATA wsaData;
//    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//    {
//        std::cerr << "Winsock初始化失败\n";
//        exit(1);
//    }
//
//    // 初始化服务器地址
//    ServerAddr.sin_family = AF_INET;
//    ServerAddr.sin_port = htons(1026); // 使用1026端口
//    ServerAddr.sin_addr.s_addr = INADDR_ANY;
//
//    // 创建套接字
//    socket = ::socket(AF_INET, SOCK_STREAM, 0);
//    if (socket == INVALID_SOCKET)
//    {
//        std::cerr << "socket创建失败\n";
//        WSACleanup();
//        exit(1);
//    }
//
//    // 绑定套接字到服务器地址
//    if (::bind(socket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
//    {
//        std::cerr << "socket绑定失败\n";
//        closesocket(socket);
//        WSACleanup();
//        exit(1);
//    }
//
//    // 监听套接字
//    if (::listen(socket, 5) == SOCKET_ERROR)
//    {
//        std::cerr << "socket listen失败\n";
//        closesocket(socket);
//        WSACleanup();
//        exit(1);
//    }
//
//    // 初始化客户端集合
//    FD_ZERO(&clientSet);
//    FD_SET(socket, &clientSet);
//    maxfd = socket;
//}
//
//// 析构函数
//FtpServer::~FtpServer()
//{
//    // 关闭所有客户端套接字
//    for (auto& pair : clientSockets)
//    {
//        disconnectClient(pair.first);
//    }
//
//    // 关闭服务器套接字
//    closesocket(socket);
//
//    // 清理Winsock
//    WSACleanup();
//}
//
//// 接收新任务
//void FtpServer::recvNewJob()
//{
//    fd_set tempSet = clientSet; // 复制客户端集合，避免被select修改
//
//    // 使用select等待套接字就绪
//    int ret = ::select(maxfd + 1, &tempSet, NULL, NULL, NULL);
//    if (ret == SOCKET_ERROR)
//    {
//        std::cerr << "select失败\n";
//        return;
//    }
//
//    // 遍历客户端集合，检查哪些套接字就绪
//    for (int i = 0; i <= maxfd; i++)
//    {
//        if (FD_ISSET(i, &tempSet))
//        {
//            if (i == socket) // 如果是服务器套接字就绪，说明有新的客户端连接请求
//            {
//                SOCKET sc = acceptClient(); // 接收客户端
//                if (sc != INVALID_SOCKET)
//                {
//                    FD_SET(sc, &clientSet);          // 将新的客户端套接字加入集合
//                    if (sc > maxfd) maxfd = sc;      // 更新最大套接字值
//                    clientSockets[sc] = std::make_shared<Client>(sc); // 创建客户端对象并加入套接字集合
//                }
//            }
//            else // 如果是客户端套接字就绪，说明有新的文件请求
//            {
//                SOCKET sc = i;
//                bool ok = getFile(sc); // 获取文件信息和操作
//                if (!ok) // 如果获取失败，说明客户端断开连接或出错
//                {
//                    disconnectClient(sc);     // 断开客户端连接
//                    FD_CLR(sc, &clientSet);   // 将客户端套接字从集合中移除
//                    clientSockets.erase(sc);  // 将客户端对象从套接字集合中移除
//                }
//                else // 如果获取成功，说明客户端有合法的文件请求
//                {
//                    handleFile(clientSockets[sc]); // 处理文件请求
//                }
//            }
//        }
//    }
//}
//
//// 接收客户端
//SOCKET FtpServer::acceptClient()
//{
//    struct sockaddr_in ClientAddr; // 客户端地址
//    int len = sizeof(ClientAddr);
//
//    // 接受客户端连接请求
//    SOCKET sc = ::accept(socket, (struct sockaddr*)&ClientAddr, &len);
//    if (sc == INVALID_SOCKET)
//    {
//        std::cerr << "client连接失败\n";
//        return INVALID_SOCKET;
//    }
//
//    // 打印客户端信息
//    std::cout << "连接client from " << inet_ntoa(ClientAddr.sin_addr) << ":" << ntohs(ClientAddr.sin_port) << "\n";
//
//    // 发送欢迎信息
//    std::string welcome = "220 Welcome to FTP Server\n";
//    ::send(sc, welcome.c_str(), welcome.size(), 0);
//
//    return sc;
//}


FtpServer::FtpServer() {
    // 初始化服务器配置
    // ...
    // 初始化Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Winsock初始化失败\n";
        exit(1);
    }

    // 初始化服务器地址
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(1026); // 使用1026端口
    ServerAddr.sin_addr.s_addr = INADDR_ANY;

    // 创建套接字
    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket == INVALID_SOCKET)
    {
        std::cerr << "socket创建失败\n";
        WSACleanup();
        exit(1);
    }

    // 绑定套接字到服务器地址
    if (::bind(socket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
    {
        std::cerr << "socket绑定失败\n";
        closesocket(socket);
        WSACleanup();
        exit(1);
    }

    // 监听套接字
    if (::listen(socket, 5) == SOCKET_ERROR)
    {
        std::cerr << "socket listen失败\n";
        closesocket(socket);
        WSACleanup();
        exit(1);
    }

    // 初始化客户端集合
    FD_ZERO(&clientSet);
    FD_SET(socket, &clientSet);
    maxfd = socket;

    // 创建线程池，根据需要设置线程池大小
    int poolSize = 10; // 设置线程池大小为10
    stop = false;

    for (int i = 0; i < poolSize; ++i) {
        threadPool.emplace_back([this] {
            while (true) {
                Task task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    // 等待任务队列非空
                    queueCondVar.wait(lock, [this] {
                        return stop || !taskQueue.empty();
                        });

                    // 如果线程池停止且任务队列为空，则退出线程
                    if (stop && taskQueue.empty()) {
                        return;
                    }

                    // 取出一个任务进行处理
                    task = std::move(taskQueue.front());
                    taskQueue.pop();
                }

                // 执行任务
                task();
            }
            });
    }
}

FtpServer::~FtpServer() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    queueCondVar.notify_all();

    // 等待所有线程结束
    for (std::thread& thread : threadPool) {
        thread.join();
    }

    // 清理其他资源
    // 关闭所有客户端套接字
    for (auto& pair : clientSockets)
    {
        disconnectClient(pair.first);
    }

    // 关闭服务器套接字
    closesocket(socket);

    // 清理Winsock
    WSACleanup();
}

void FtpServer::recvNewJob() {
    fd_set tempSet = clientSet;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 10000; // 设置超时时间为10毫秒

    // 使用select等待套接字就绪
    int ret = ::select(maxfd + 1, &tempSet, nullptr, nullptr, &timeout);
    if (ret == SOCKET_ERROR) {
        std::cerr << "select失败\n";
        return;
    }

    // 遍历客户端集合，检查哪些套接字就绪
    for (int i = 0; i <= maxfd; ++i) {
        if (FD_ISSET(i, &tempSet)) {
            if (i == socket) {
                SOCKET sc = acceptClient(); // 接收客户端
                if (sc != INVALID_SOCKET) {
                    std::shared_ptr<Client> client = std::make_shared<Client>(sc);
                    {
                        std::lock_guard<std::mutex> lock(queueMutex);
                        taskQueue.push([this, client] {
                            FD_SET(client->socket, &clientSet);
                            if (client->socket > maxfd) {
                                maxfd = client->socket;
                            }
                            clientSockets[client->socket] = client;
                            });
                    }
                    queueCondVar.notify_one();
                }
            }
            else {
                SOCKET sc = i;
                std::shared_ptr<Client> client = clientSockets[sc];
                {
                    // 互斥量上锁
                    std::lock_guard<std::mutex> lock(queueMutex);
                    taskQueue.push([this, client] {
                        bool ok = getFile(client->socket); // 获取文件信息和操作
                        if (!ok) {
                            disconnectClient(client->socket);
                            FD_CLR(client->socket, &clientSet);
                            clientSockets.erase(client->socket);
                        }
                        else {
                            handleFile(client);
                        }
                        });
                }
                queueCondVar.notify_one();
            }
        }
    }
}

SOCKET FtpServer::acceptClient() {
    struct sockaddr_in ClientAddr;
    int len = sizeof(ClientAddr);

    SOCKET sc = ::accept(socket, (struct sockaddr*)&ClientAddr, &len);
    if (sc == INVALID_SOCKET) {
        std::cerr << "client连接失败\n";
        return INVALID_SOCKET;
    }

    std::cout << "连接client from " << inet_ntoa(ClientAddr.sin_addr) << ":" << ntohs(ClientAddr.sin_port) << "\n";

    std::string welcome = "220 Welcome to FTP Server\n";
    ::send(sc, welcome.c_str(), welcome.size(), 0);

    return sc;
}


// 获取文件信息和操作
bool FtpServer::getFile(SOCKET sc)
{
    char buffer[1024]; // 缓冲区

    // 接收客户端发送的数据
    int len = ::recv(sc, buffer, sizeof(buffer), 0);
    if (len <= 0)
    {
        std::cerr << "Failed to receive data from client\n";
        return false;
    }

    // 解析客户端发送的数据，获取文件名和操作类型
    std::string data(buffer, len);
    std::istringstream iss(data);
    std::string op, filename;
    iss >> op >> filename;

    // 设置客户端对象的文件名和操作类型属性
    auto client = clientSockets[sc];
    client->setFilename(filename);
    client->setOp(op);

    return true;
}

// 处理文件
bool FtpServer::handleFile(std::shared_ptr<Client> client)
{
    std::string op = client->getOp(); // 获取操作类型

    if (op == "Down") // 如果是下载请求
    {
        return down(client); // 调用下载函数
    }
    else if (op == "Up") // 如果是上传请求
    {
        return up(client); // 调用上传函数
    }
    else if (op == "List") // 如果是列表请求
    {
        return list(client); // 调用列表函数
    }
    else if (op == "Quit") // 如果是退出请求
    {
        disconnectClient(socket); // 调用退出函数
    }
    else // 如果是无效的请求
    {
        std::cerr << "Invalid operation: " << op << "\n";
        return false;
    }
}

// 下载文件
bool FtpServer::down(std::shared_ptr<Client> client)
{
    //std::string filename = client->getFilename(); // 获取文件名
    //SOCKET sc = client->getSocket(); // 获取套接字
    //char buffer[1024]; // 缓冲区
    //int len; // 数据长度

    //// 打开文件
    //std::ifstream file(filename, std::ios::out | std::ios::binary);
    //if (!file.is_open()) // 如果文件不存在或打开失败
    //{
    //    std::cerr << "Failed to open file: " << filename << "\n";
    //    sprintf(buffer, "550 File not found\n"); // 发送错误信息给客户端
    //    send(sc, buffer, strlen(buffer), 0);
    //    return false;
    //}

    //// 发送成功信息给客户端
    //sprintf(buffer, "150 Opening data connection\n");
    //send(sc, buffer, strlen(buffer), 0);

    //// 读取文件内容并发送给客户端
    //while (file.good())
    //{
    //    file.read(buffer, sizeof(buffer));
    //    len = file.gcount();
    //    send(sc, buffer, len, 0);
    //}

    //// 关闭文件
    //file.close();

    //// 发送完成信息给客户端
    ////sprintf(buffer, "226 Transfer complete\n");
    //send(sc, buffer, strlen(buffer), 0);

    //return true;
    std::string filename = client->getFilename(); // 获取文件名
    SOCKET sc = client->getSocket(); // 获取套接字
    char buffer[1024]; // 缓冲区
    int len; // 数据长度

    // 打开文件
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) // 如果文件不存在或打开失败
    {
        std::cerr << "Failed to open file: " << filename << "\n";
        sprintf(buffer, "550 File not found\n"); // 发送错误信息给客户端
        send(sc, buffer, strlen(buffer), 0);
        return false;
    }

    // 获取文件长度
    file.seekg(0, std::ios::end);
    int fileLength = file.tellg();
    file.seekg(0, std::ios::beg);

    // 发送成功信息和文件长度给客户端
    sprintf(buffer, "150 %d\n", fileLength);
    send(sc, buffer, strlen(buffer), 0);

    // 读取文件内容并发送给客户端
    int totalSent = 0; // 记录已发送的数据长度
    while (totalSent < fileLength)
    {
        file.read(buffer, sizeof(buffer));
        len = file.gcount();
        send(sc, buffer, len, 0);
        totalSent += len;
    }

    // 关闭文件
    file.close();

    // 发送完成信息给客户端
    //sprintf(buffer, "226 Transfer complete\n");
    send(sc, buffer, strlen(buffer), 0);

    return true;

}

// 上传文件
bool FtpServer::up(std::shared_ptr<Client> client)
{
    std::string filename = client->getFilename(); // 获取文件名
    SOCKET sc = client->getSocket(); // 获取套接字
    char buffer[1024]; // 缓冲区
    int len; // 数据长度

    // 打开文件，如果已存在则覆盖
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) // 如果文件打开失败
    {
        std::cerr << "Failed to open file: " << filename << "\n";
        sprintf(buffer, "550 File not accessible\n"); // 发送错误信息给客户端
        send(sc, buffer, strlen(buffer), 0);
        return false;
    }

    // 发送成功信息给客户端
    sprintf(buffer, "150 Opening data connection\n");
    send(sc, buffer, strlen(buffer), 0);

    // 接收客户端发送的文件内容并写入文件
    while ((len = recv(sc, buffer, sizeof(buffer), 0)) >= 0)
    {
        if (len == 0) { break; }
        file.write(buffer, len);
        if (len < sizeof(buffer)) break; // 如果接收到的数据小于缓冲区大小，说明文件传输结束
    }

    // 关闭文件
    file.close();

    // 发送完成信息给客户端
    sprintf(buffer, "226 Transfer complete\n");
    send(sc, buffer, strlen(buffer), 0);

    return true;
}

// 断开客户端连接
void FtpServer::disconnectClient(SOCKET sc)
{
    // 发送退出信息给客户端
    char buffer[1024];
    sprintf(buffer, "221 Goodbye\n");
    send(sc, buffer, strlen(buffer), 0);

    // 关闭套接字
    closesocket(sc);

    // 打印断开信息
    std::cout << "Disconnected client\n";
}

// 处理列表请求
bool FtpServer::list(std::shared_ptr<Client> client)
{
    // 获取服务器默认目录下的文件列表
    std::vector<std::string> files;
    std::vector<size_t> sizes;
    for (auto& entry : std::filesystem::directory_iterator("."))
    {
        if (entry.is_regular_file()) // 只获取普通文件，忽略目录等
        {
            files.push_back(entry.path().filename().string()); // 获取文件名
            sizes.push_back(entry.file_size()); // 获取文件大小
        }
    }

    // 构造文件列表信息字符串
    std::ostringstream oss;
    oss << "150 Here comes the directory listing.\n";
    for (size_t i = 0; i < files.size(); i++)
    {
        oss << std::left << std::setw(20) << files[i] << "\t" << sizes[i] << "\n"; // 左对齐，宽度为20，用制表符分隔文件名和大小
    }
    oss << "226 Directory send OK.\n";
    std::string data = oss.str();

    // 发送文件列表信息给客户端
    SOCKET sc = client->getSocket();
    int len = ::send(sc, data.c_str(), data.size(), 0);
    if (len <= 0)
    {
        std::cerr << "Failed to send data to client\n";
        return false;
    }

    return true;
}


int main() {
    // 创建FtpServer对象 
    FtpServer server;

    // 循环接收新任务 
    while (true) { // 接收新客户端或文件 
        server.recvNewJob(); 
    }

    return 0;
}