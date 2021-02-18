#ifndef OBJRMDISK_H
#define OBJRMDISK_H
#include "string"
using namespace std;

class objrmdisk
{
public:
    objrmdisk();
    void executeCommand(objrmdisk *disk);


    string path;
};

#endif // OBJRMDISK_H
