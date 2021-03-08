#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "PI.h"
#include "log.h"

#define PITakePhoto         1
#define PIGetPhoto          2
// #define PIGetPhoto          3
#define PISendPhotoSuccess  4
#define PISendPhotoEnd      5
#define PIEndSocket         6

#define SERVER_PORT         8888
#define LISTEN_QUEUE_SISE   10
#define PICTURE_DIR         "./photo/"
#define PER_READ_SIZE       1024

PI pi;

using namespace std;

Log logger;

bool isPhotoExisted(string filePath) {
    ifstream fin(filePath);
    bool res = fin.good();
    fin.close();
    return res;
}

int readPhotoAndSendToSocket(string fileName, int clientfd) {
    string filePath = string(PICTURE_DIR) + fileName;
    if (!isPhotoExisted(filePath)) {
        // 如果照片不存在
        return -1;
    }
    ifstream fin(filePath, ios::binary);
    // 每次读1024个字节
    char photo[PER_READ_SIZE+4];
    int PISendPhotoSuccess_local = PISendPhotoSuccess;
    int PISendPhotoEnd_local = PISendPhotoEnd;
    while(!fin.eof()) {
        fin.read(photo+4, PER_READ_SIZE);
        // 这样读出来的buf是OK的
        // 将这个buf写到socket里面去
        // 在photo前面加一个标识，4个字节，int类型

        memcpy(photo, &PISendPhotoSuccess_local, 4);
        int len = write(clientfd, photo, PER_READ_SIZE+4);
        if (len != 1028) {
            logger.writeLog("写socket字符数组大小有问题，filepath=" + filePath, ERROR);
        }
    }
    memcpy(photo, &PISendPhotoEnd_local, 4);
    int len = write(clientfd, photo, 4);
    fin.close();
    return 0;
}

int main() {
    pi.setLogger(&logger);
    int socketfd;

    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error");
        exit(1);
    }

    // 2 bind 绑定本地地址和端口
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;//ipv4
    serveraddr.sin_port = htons(SERVER_PORT); //端口
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//响应任意网卡的请求
    if(bind(socketfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
    {
        perror("bind error");
        exit(1);
    }

    if(listen(socketfd, LISTEN_QUEUE_SISE) < 0)
    {
        perror("listen error");
        exit(1);
    }

    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    while(1)
    {    
        // 4 accept 从队列拿出第一个
        // clientaddr获取客户端的地址信息，是传出参数
        int clientfd = accept(socketfd, (struct sockaddr*)&clientaddr, &clientaddr_len);
        printf("get one connection\n");
        if (clientfd  < 0)
        {
            perror("accept error");
            continue;
        }
        // 5 read/write
        printf("get one connection\n");
        printf("%s\n",inet_ntoa(clientaddr.sin_addr));

        while(true) {
            char buff[1024] = {0};
            int lenRecv = recv(clientfd, buff, sizeof(buff)-1, 0);
            std::cout << lenRecv << std::endl;

            // 将前四个字节转化成数字
            uint32_t order = *(buff);
            std:: cout << order << std::endl;

            // 将后20个字节转化成字符串
            char* filename = buff + 4;
            std::cout << filename << std::endl;
            

            bool isEnd = false;

            switch(order) {
                case PITakePhoto: {
                    cout << "PI Take Photo" << endl;
                    string filename_string = string(filename);
                    logger.writeLog("get PITake Photo order, filename="+filename_string, INFO);
                    pi.takePhoto(filename_string);
                    break;
                }
                case PIGetPhoto: {
                    string filename_string = string(filename);
                    logger.writeLog("get PIGet Photo order, filename="+filename_string, INFO);
                    // 如果文件不存在
                    if (!isPhotoExisted(PICTURE_DIR + filename_string)) {
                        logger.writeLog("file do not exist! filename="+filename_string, WARNING);
                    }

                    // 如果文件存在，那就开始读文件并且传输文件
                    readPhotoAndSendToSocket(filename_string, clientfd);
                    break;
                }
                case PIEndSocket: {
                    isEnd = true;
                    break;
                }
                default: {
                    logger.writeLog("get wrong order=", ERROR);
                    isEnd = true;
                    break;
                }
            }
            // 如果需要推出
            if(isEnd) {
                break;
            }


        }
    
        // 6 close
        close(clientfd);
    }
    return 0;
}

