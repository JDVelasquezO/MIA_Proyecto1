#include "objmount.h"
#include "iostream"
#include "disk.h"
#include "staticobjects.h"
#include <string>
using namespace std;

objMount::objMount()
{

}

bool existPartition(string name, objMount *disk) {
    FILE *file;
    file = fopen(disk->path.c_str(), "rb+");
    fseek(file, 0, SEEK_SET); // Moverse 0 caracteres desde el inicio del archivo
    mbr tempMbr;
    fread(&tempMbr, sizeof(mbr), 1, file);

    for (int i = 0; i < 4; i++) {
        if (name == tempMbr.mbr_partitions[i].part_name) {
            return true;
        }
    }

    return false;
}

partition returnPart(string name, objMount *disk) {
    FILE *file;
    file = fopen(disk->path.c_str(), "rb+");
    fseek(file, 0, SEEK_SET); // Moverse 0 caracteres desde el inicio del archivo
    mbr tempMbr;
    fread(&tempMbr, sizeof(mbr), 1, file);

    for (int i = 0; i < 4; i++) {
        if (name == tempMbr.mbr_partitions[i].part_name) {
            return tempMbr.mbr_partitions[i];
        }
    }
}

string assignLetter() {
    // 201800722
    int carnet = 22;
    int number = 1;
    char letter = 'A';
    // PENDIENTE
    /*Hacer todo el proceso para asignar ids diferentes*/
    string id = to_string(carnet) + to_string(number) + string(1, letter);
    return id;
}

void mountDisk(objMount *disk) {
    if (!disk->path.empty()) {
        if (!disk->name.empty()) {
            FILE *file;

            file = fopen(disk->path.c_str(), "rb+");
            if (file == NULL) {
                cout << "El archivo no existe" << endl;
            } else {
                if (existPartition(disk->name, disk)) {
                    // PENDIENTE: Verificar si ya esta montado
                    string letter = assignLetter();
                    mountedDisks tempDisk;
                    tempDisk.letter = letter;
                    tempDisk.path = disk->path;
                    mounted auxDisk;
                    auxDisk.id = letter;
                    auxDisk.status = 1;
                    auxDisk.path = disk->path;
                    auxDisk.part = returnPart(disk->name, disk);
                    auxDisk.name = disk->name;
                    tempDisk.partitions[0] = auxDisk;
                    staticObjects::linkedMounted->addPartitions(tempDisk);
                    cout << "Se monto correctamente" << endl;
                }
            }
        }
    }
}

void objMount::executeCommand(objMount *disk) {
    mountDisk(disk);
}
