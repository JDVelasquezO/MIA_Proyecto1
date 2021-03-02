#include "node.h"
#include "disk.h"

Node::Node(char value)
{
    this->value = value;
}

Node::Node(mountedDisks disk) {
    this->disk = disk;
}
