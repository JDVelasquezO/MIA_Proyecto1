#ifndef NODE_H
#define NODE_H
#include "disk.h"

class Node
{
public:
    Node(char value);
    Node(mountedDisks disk);

    char value;
    mountedDisks disk;
    Node *next;
};

#endif // NODE_H
