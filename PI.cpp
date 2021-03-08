#include "PI.h"


PI::PI() {
    logger = nullptr;
}

int PI::takePhoto(std::string filename) {
    if (this->logger == nullptr) {
        std::cout << "you do not set a logger" << std::endl; 
    }
    // 应该注意不能直接使用用户输入的字符串
    logger->writeLog("PI take Photo, filename="+filename, INFO);

    return 0;
}

void PI::setLogger(Log* logger) {
    this->logger = logger;
}