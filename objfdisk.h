#ifndef OBJFDISK_H
#define OBJFDISK_H
#include "string"
using namespace std;

class ObjFdisk
{
public:
    ObjFdisk();
    void executeCommand(ObjFdisk *disk);
    void executeDelete(ObjFdisk *disk);
    void printData(string path);

    int size;
    string unity;
    string path;
    string type;
    string fit;
    string del;
    string name;
    int add;
};

#endif // OBJFDISK_H
