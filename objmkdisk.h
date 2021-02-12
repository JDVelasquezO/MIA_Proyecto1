#ifndef OBJMKDISK_H
#define OBJMKDISK_H
#include <iostream>
#include "string"
using namespace std;

class ObjMkdisk
{
public:
    ObjMkdisk();
    void executeCommand(ObjMkdisk *disk);

    int size;
    string fit;
    char unity;
    string path;
};

#endif // OBJMKDISK_H
