#ifndef OBJMKDISK_H
#define OBJMKDISK_H
#include <iostream>
#include <dirent.h>
#include "string"
using namespace std;

class ObjMkdisk
{
public:
    ObjMkdisk();
    void executeCommand(ObjMkdisk *disk);
    void assignUnity(ObjMkdisk *disk);

    int size;
    string fit;
    string unity;
    string path;
};

#endif // OBJMKDISK_H
