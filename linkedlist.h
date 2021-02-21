#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "node.h"

class LinkedList
{
public:
    LinkedList();

    void addStart(char value);
    void getValues();
    bool isEmpty();
    int getLength();

    Node* start;
    int length;
};

#endif // LINKEDLIST_H
