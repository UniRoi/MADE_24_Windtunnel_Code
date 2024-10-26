#ifndef VERSION_H
#define VERSION_H

#include <stdint.h>
#include <stdio.h>

class version
{
private:
    /* version control for this code can be done here consists of major.minor.patch */
    #define MAJOR 00
    #define MINOR 01
    #define PATCH 01
public:
    version();
    ~version();

    uint32_t getNumber(void);
    void getString(char* version);
};

version::version()
{
}

version::~version()
{
}

uint32_t version::getNumber(void)
{
    uint32_t version = 0;
    version |= MAJOR << 16;
    version |= MINOR << 8;
    version |= PATCH << 0;
    return version;
}

void version::getString(char * version)
{
    if(version != nullptr)
    {
        // Format the version as "00.01.01" and store it in the provided `version` array
        sprintf((char*)version, "%02d.%02d.%02d", MAJOR, MINOR, PATCH);
    }
}

#endif