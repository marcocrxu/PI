#pragma once
#include <iostream>
#include <string.h>
#include "log.h"

class PI {
    Log* logger;
public:
    PI();
    void setLogger(Log* logger);
    int takePhoto(std::string filename);
};