#ifndef DISK_H
#define DISK_H
#include <string>
using namespace std;

typedef struct {
    char part_status;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
    char part_type;
}partition;

typedef struct: public partition {
    int part_next;
}extended;

typedef struct {
    int mbr_tamano;
    int availableStorage;
    char mbr_fecha_creacion[16];
    int mbr_disk_signature;
    char disk_fit;
    partition mbr_partitions[4];
}mbr;

typedef struct {
    string id;
    int status;
    string path;
    partition part;
    string name;
}mounted;

typedef struct {
    string letter;
    string path;
    mounted partitions[90];
}mountedDisks;

class disk {
public:
    disk();
};

#endif // DISK_H
