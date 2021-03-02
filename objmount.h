#ifndef OBJMOUNT_H
#define OBJMOUNT_H
#include "string"
using namespace std;

class objMount
{
public:
    objMount();
    void executeCommand(objMount *disk);

    string path;
    string name;
};

#endif // OBJMOUNT_H
