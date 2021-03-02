#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "node.h"
#include "disk.h"

class LinkedList
{
public:
    LinkedList();

    void addStart(char value);
    void addPartitions(mountedDisks disk);
    void getValues();
    bool isEmpty();
    int getLength();

    Node* start;
    int length;
};

#endif // LINKEDLIST_H
