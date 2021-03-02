#include "linkedlist.h"
#include "linkedlist.h"
#include "node.h"
#include "iostream"
#include "disk.h"
using namespace std;

LinkedList::LinkedList()
{
    this->start = nullptr;
    this->length = 0;
}

void LinkedList::addStart(char value) {
    Node *node = new Node(value);
    node->next = this->start;
    this->start = node;
    this->length += 1;
}

void LinkedList::addPartitions(mountedDisks disk) {
    Node *node = new Node(disk);
    node->next = this->start;
    this->start = node;
    this->length += 1;
}

void LinkedList::getValues() {
    Node *pointer = this->start;
    while (pointer != nullptr) {
        cout << pointer->value << endl;
        pointer = pointer->next;
    }
}
