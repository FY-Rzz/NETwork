/*
* @File: lab4_FtpClient.cpp
* @Author: Xie Zhiwei
* @Description: 实现FTP客户端类
*/
#include <ostream>
#include "FtpClient.h"

// 构造函数
FtpClient::FtpClient()
{
    // 初始化Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Failed to initialize Winsock\n";
        exit(1);
    }

    // 初始化服务器地址
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(1026); // 使用1026端口

    //// 获取服务器主机名
    //std::cout << "Enter the server hostname: ";
    //std::cin >> remoteHostname;

    //// 解析服务器主机名为IP地址
    //struct hostent* host = gethostbyname(remoteHostname);
    //if (host == NULL)
    //{
    //    std::cerr << "Failed to resolve hostname\n";
    //    WSACleanup();
    //    exit(1);
    //}
    //ServerAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
    ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 创建套接字
    sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket\n";
        WSACleanup();
        exit(1);
    }

    // 连接到服务器
    if (::connect(sock, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to connect to server\n";
        closesocket(sock);
        WSACleanup();
        exit(1);
    }

    // 接收服务器的欢迎信息
    char buffer[1024];
    int len = ::recv(sock, buffer, sizeof(buffer), 0);
    if (len <= 0)
    {
        std::cerr << "Failed to receive data from server\n";
        closesocket(sock);
        WSACleanup();
        exit(1);
    }

    // 打印服务器的欢迎信息
    std::string data(buffer, len);
    std::cout << data;

}

// 析构函数
FtpClient::~FtpClient()
{
    // 关闭套接字
    closesocket(sock);

    // 清理Winsock
    WSACleanup();
}

// 运行客户端
void FtpClient::run()
{
    std::string op; // 操作类型
    std::string filename; // 文件名

    while (true) // 循环输入操作类型和文件名，直到输入Quit为止
    {
        std::cout << "请输入操作名称（Down，Up，List，Quit）: ";
        std::cin >> op;

        if (op == "Down") // 如果是下载请求
        {
            std::cout << "请输入文件名" << std::endl;
            std::cin >> filename;
            File f(filename); // 创建文件对象
            down(f); // 调用下载函数
        }
        else if (op == "Up") // 如果是上传请求
        {
            std::cout << "请输入文件名" << std::endl;
            std::cin >> filename;
            File f(filename); // 创建文件对象
            up(f); // 调用上传函数
        }
        else if (op == "List") // 如果是列表请求
        {
            list(); // 调用列表函数
        }
        else if (op == "Quit") // 如果是退出请求
        {
            quit(); // 调用退出函数
            break; // 结束循环
        }
        else // 如果是无效的请求
        {
            std::cerr << "非法输入: " << op << "\n";
            continue; // 重新输入
        }

    }

}

// 列出文件
bool FtpClient::list()
{
    char buffer[1024]; // 缓冲区
    int len; // 数据长度

    // 构造列表请求字符串
    std::string op = "List";
    std::ostringstream oss;
    oss << op << "\n";
    std::string data = oss.str();

    // 发送列表请求给服务器
    len = ::send(sock, data.c_str(), data.size(), 0);
    if (len <= 0)
    {
        std::cerr << "向服务器发送数据出错\n";
        return false;
    }

    // 接收服务器的响应
    len = ::recv(sock, buffer, sizeof(buffer), 0);
    if (len <= 0)
    {
        std::cerr << "Failed to receive data from server\n";
        return false;
    }

    // 解析服务器的响应，判断是否成功
    data.assign(buffer, len);
    std::istringstream iss(data);
    int code;
    std::string msg;
    iss >> code >> msg;
    if (code != 150) // 如果不是成功的响应
    {
        std::cerr << "Failed to list files: " << msg << "\n";
        return false;
    }

    // 打印服务器的响应
    std::cout << data;

    //// 接收服务器发送的文件列表信息并打印
    //while ((len = recv(sock, buffer, sizeof(buffer), 0)) >= 0)
    //{
    //    if (len == 0) { break; }
    //    data.assign(buffer, len);
    //    std::cout << data;
    //    if (len < sizeof(buffer)) break; // 如果接收到的数据小于缓冲区大小，说明文件列表传输结束
    //}

    return true;
}

// 上传文件
bool FtpClient::up(File& f0)
{
    std::fstream f;
    char buffer[1024]; // 缓冲区
    int len; // 数据长度

    // 构造上传请求字符串
    std::string op = "Up";
    std::string filename = f0.filename;
    std::ostringstream oss;
    oss << op << " " << filename << "\n";
    std::string data = oss.str();

    // 发送上传请求给服务器
    len = ::send(sock, data.c_str(), data.size(), 0);
    if (len <= 0)
    {
        std::cerr << "向服务器发送数据出错\n";
        return false;
    }

    // 接收服务器的响应
    len = ::recv(sock, buffer, sizeof(buffer), 0);
    if (len <= 0)
    {
        std::cerr << "Failed to receive data from server\n";
        return false;
    }

    // 解析服务器的响应，判断是否成功
    data.assign(buffer, len);
    std::istringstream iss(data);
    int code;
    std::string msg;
    iss >> code >> msg;
    if (code != 150) // 如果不是成功的响应
    {
        std::cerr << "Failed to upload file: " << msg << "\n";
        return false;
    }

    // 打开文件
    f.open(filename, std::ios::in);
    if (!f.is_open()) // 如果文件打开失败或不存在
    {
        std::cerr << "Failed to open file: " << filename << "\n";
        return false;
    }
    //std::cout << "File opened: " << f.is_open() << std::endl;
    // 读取文件内容并发送给服务器
    while (f.good())
    {
        //std::cout << "in\n";
        f.read(buffer, sizeof(buffer));
        len = f.gcount();
        std::cout << "文件大小：" << len << "B" << " ";
        send(sock, buffer, len, 0);
    }
    std::cout << "发送成功" << std::endl;

    // 关闭文件
    f.close();
    return true;
}

// 退出客户端
bool FtpClient::quit()
{
    char buffer[1024]; // 缓冲区
    int len; // 数据长度

    // 构造退出请求字符串
    std::string op = "Quit";
    std::ostringstream oss;
    oss << op << "\n";
    std::string data = oss.str();

    // 发送退出请求给服务器
    len = ::send(sock, data.c_str(), data.size(), 0);
    if (len <= 0)
    {
        std::cerr << "向服务器发送数据出错\n";
        return false;
    }
}

// 下载文件
bool FtpClient::down(File& f0)
{
    std::fstream f;
    char buffer[1024]; // 缓冲区
    char* tmpbuf = buffer;
    int len; // 数据长度

    // 构造下载请求字符串
    std::string op = "Down";
    std::string filename = f0.filename;
    std::ostringstream oss;
    oss << op << " " << filename << "\n";
    std::string data = oss.str();

    // 发送下载请求给服务器
    len = ::send(sock, data.c_str(), data.size(), 0);
    if (len <= 0)
    {
        std::cerr << "向服务器发送数据出错\n";
        return false;
    }

    // 接收服务器的响应
    len = ::recv(sock, buffer, sizeof(buffer), 0);
    if (len <= 0)
    {
        std::cerr << "Failed to receive data from server\n";
        return false;
    }

    // 解析服务器的响应，判断是否成功
    data.assign(buffer, len);
    std::istringstream iss(data);
    int code;
    std::string msg;
    int fileLength;
    iss >> code  >> fileLength;
    std::cout << "响应码：" << code << " " << "文件长度：" << fileLength << "B" << std::endl;
    if (code != 150) // 如果不是成功的响应
    {
        std::cerr << "下载失败: " << msg << "\n";
        return false;
    }

    // 打开文件，如果已存在则覆盖
    f.open(filename, std::ios::out | std::ios::binary);
    if (!f.is_open()) // 如果文件打开失败
    {
        std::cerr << "Failed to open file: " << filename << "\n";
        return false;
    }

    // 接收服务器发送的文件内容并写入文件
    int totalReceived = 0; // 记录已接收的数据长度
    while (1)
    {
        len = recv(sock, buffer, sizeof(buffer), 0);
        if (len <= 0) { break; }
        totalReceived += len;
        f.write(buffer, totalReceived>fileLength?(fileLength- totalReceived+len):len);
        if (totalReceived >= fileLength) { break; } // 如果已接收到的数据长度达到文件长度，则文件传输结束
    }

    // 关闭文件
    f.close();
    return true;
}

int main() {
    // 创建FtpClient对象 
    FtpClient client;
    client.run();

}
