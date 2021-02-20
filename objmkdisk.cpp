#include "objmkdisk.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "methods.h"
#include "time.h"
#include "disk.h"
using namespace std;

ObjMkdisk::ObjMkdisk()
{
    unity = "m";
    path = "/";
    fit = "FF";
}

bool makePath(const string &path) {

    mode_t mode = 0755;
    int ret = mkdir(path.c_str(), mode);
    if (ret == 0)
        return true;

    switch (errno)
    {
    case ENOENT:
        // parent didn't exist, try to create it
        {
            int pos = path.find_last_of('/');
            if (pos == std::string::npos)
                return false;
            if (!makePath( path.substr(0, pos) ))
                return false;
        }
        return 0 == mkdir(path.c_str(), mode);

    case EEXIST:
        // done!
        return Methods::isDirExist(path);

    default:
        return false;
    }
}

string createFolder(ObjMkdisk *disk) {
    string route = disk->path.c_str();

    //cout << route << endl;
    //cout << route.length() << endl;
    if (route[0] == '\"') {
        route.erase(0, 1);
        route.erase(route.length()-1, 1);
    }
    //cout << route << endl;

    string delimiter = "/";

    size_t pos = 0;
    string token;
    string newRoute = "";
    while ((pos = route.find(delimiter)) != string::npos) {
        token = route.substr(0, pos);
        // cout << token << std::endl;
        newRoute += token + "/";
        //cout << newRoute << endl;
        route.erase(0, pos + delimiter.length());
    }

    return newRoute;
}

void fillDiskWithMbr(mbr newMbr, ObjMkdisk *disk) {

    FILE *file;
    // SE CREA PARTICION QUE DESPUES ESTARA EN EL MBR
    partition firstPartition;
    firstPartition.part_status = '0';
    firstPartition.part_type = '-';
    firstPartition.part_fit = '-';
    firstPartition.part_size = 0;
    firstPartition.part_start = 0;
    firstPartition.part_name[0] = '\0';
    for (int i = 0; i < 4; i++) {
        newMbr.mbr_partitions[i] = firstPartition;
    }

    file = fopen(disk->path.c_str(), "wb+"); // Se abre el archivo bin con permisos mixtos
    if(file != NULL){
        fseek(file, 0, SEEK_SET);
        fwrite(&newMbr, sizeof(newMbr), 1, file);
        fclose(file);
        cout << "Se creó el disco correctamente" << endl;
        cout << "Se agrego el MBR correctamente " << endl;
    }
}

void createMBR(mbr newMbr, ObjMkdisk *disk) {
    // OBTENEMOS LA HORA LOCAL
    time_t now = time(0);
    tm* localtm = localtime(&now);
    string timeNow = asctime(localtm);
    // -----------------------

    // Llenamos el mbr con los atributos necesarios
    strcpy(newMbr.mbr_fecha_creacion, timeNow.c_str());
    newMbr.mbr_disk_signature = rand() % 100;
    char* fitPointer = &newMbr.disk_fit;
    strcpy(fitPointer, disk->fit.c_str());

    fillDiskWithMbr(newMbr, disk);
}

void ObjMkdisk::assignUnity(ObjMkdisk *disk){

    mbr newMbr; // mbr a crear con el disco. No pertenece a ninguna particion
    FILE *file; // archivo que simulara ser disco
    char buffer[1024]; // arreglo que se llenara de bytes
    string newRoute = createFolder(disk); // ruta nueva solo de dirs
    makePath(newRoute); // Se crean los directorios

    // ASIGNACION DE TAMAÑO AL DISCO Y AL MBR
    if (disk->unity == "K" || disk->unity == "k") {
        file = fopen(disk->path.c_str(), "wb"); // Creamos el archivo binario en la ruta del folder
        newMbr.mbr_tamano = (disk->size)*1024;
        for (int i = 0; i < 1024; i++) { // Es hasta 1024 porque es 1KB
            buffer[i] = '\0'; // Se llena la variable con el caracter 0. 1 char = 1 byte
        }

        for (int i = 0; i < disk->size; i++) { // Desde 0 hasta el el tamaño
            fwrite(&buffer, 1024, 1, file); // Escribe i veces el kilobyte
        }

        fclose(file);
    }

    if (disk->unity == "M" || disk->unity == "m") {
        file = fopen(disk->path.c_str(), "wb"); // Creamos el archivo binario en la ruta del folder
        newMbr.mbr_tamano = (disk->size)*1024*1024;
        for (int i = 0; i < 1024; i++) {
            buffer[i] = '\0';
        }

        for (int i = 0; i < (disk->size * 1024); i++) { // Se multiplica *1024 porque 1MB = 1024KB
            fwrite(&buffer, 1024, 1, file);
        }

        fclose(file);
    }

    // Llenamos las propiedades del mbr
    createMBR(newMbr, disk);
}

void ObjMkdisk::executeCommand(ObjMkdisk *disk) {

    cout << "------------INFORMACION DEL DISCO---------------------" << endl;
    cout << "Datos del Disco " << disk->unity << endl;
    cout << "El tamaño es: " << disk->size << endl;
    cout << "La ruta es: " << disk->path << endl;
    cout << "El ajuste es: " << disk->fit << endl;

    assignUnity(disk);

    FILE *file;
    file = fopen(disk->path.c_str(), "rb");
    // fseek(file, 0, SEEK_SET);
    mbr MBR;
    fread(&MBR, sizeof(mbr), 1, file);
    cout << MBR.mbr_fecha_creacion << endl;
    for (int i = 0; i < 4; i++) {
        /*if (tempMbr.mbr_partitions[i].part_status == '0'){
            tempPart = tempMbr.mbr_partitions[i];
            break;
        }*/
        cout << MBR.mbr_partitions[i].part_status << endl;
    }
}



