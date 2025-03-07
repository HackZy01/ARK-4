#ifndef FTP_H
#define FTP_H

#include "system_entry.h"
#include "common.h"

class FTPManager : public SystemEntry{
    void draw();
    void control(Controller* pad);
    void pause();
    void resume();
    std::string getInfo();
    void setInfo(std::string info);
    std::string getName();
    void setName(std::string name);
    Image* getIcon();
};

#endif

