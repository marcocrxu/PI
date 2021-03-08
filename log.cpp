#include "log.h"

void Log::writeLog(string info, LogType logType) {
    // 获取时间的String
    string nowTime = getTime();
    out.open(LOG_FILE_NAME, ios::app);
    string logInfo = "";
    switch (logType)
    {
    case ERROR: {
        logInfo += "[ERROR]";
        break;
    }
    case INFO: {
        logInfo += "[INFO]";
        break;
    }
    case WARNING: {
        logInfo += "[WARNING]";
        break;
    } 
    default:
        break;
    }
    logInfo += nowTime;
    logInfo += info;
    std::cout << "writeLog=" << logInfo << std::endl;
    out << logInfo << std::endl;
    out.close();
}

string Log::getTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string res = "[";
    res = res + std::to_string(1900+ltm->tm_year)+"-";
    if (ltm->tm_mon < 9) {
        res += "0";
    }
    res += std::to_string(1 + ltm->tm_mon);
    res += "-";
    if (ltm->tm_mday < 9) {
        res += "0";
    }
    res += std::to_string(ltm->tm_mday);
    res += " ";

    string hour = "";
    if (ltm->tm_hour < 9) {
        hour += "0";
    }
    hour += std::to_string(ltm->tm_hour);
    string min = "";
    if (ltm->tm_min < 9) {
        min += "0";
    }
    min += std::to_string(ltm->tm_min);
    string sec = "";
    if (ltm->tm_sec < 9) {
        sec += "0";
    }
    sec += std::to_string(ltm->tm_sec);
    res = res+hour+":"+min+":"+sec+"]";
    return res;
}

Log::Log(){
    
}