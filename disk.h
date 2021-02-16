#ifndef DISK_H
#define DISK_H
#include <string>
using namespace std;

typedef struct {
    char part_status[1];
    char part_type[1];
    char part_fit[1];
    int part_start[1];
    int part_size;
    char part_name[16];
}partition;

typedef struct {
    int mbr_tamano;
    char mbr_fecha_creacion[16];
    int mbr_disk_signature;
    char disk_fit[1];
    // partition mbr_partitions[4];
}mbr;

class disk {
public:
    disk();
};

#endif // DISK_H
