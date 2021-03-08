#pragma once
#include <iostream> 
#include <string.h>
#include <ctime>
#include <fstream>

#define LOG_FILE_NAME "./log.txt"

using namespace std;


enum LogType {
    ERROR,
    WARNING,
    INFO
};

class Log {
public:
    Log();
    void writeLog(string info, LogType logType);
private:
    ofstream out;
    int logFd;
    string getTime();
};
