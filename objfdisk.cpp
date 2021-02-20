#include "objfdisk.h"
#include "iostream"
#include "disk.h"
#include <cstring>
#include "string"

ObjFdisk::ObjFdisk()
{
    unity = "K";
    type = "P";
    fit = "WF";
}

void printData(string path) {
    FILE*arch;
    arch=fopen(path.c_str(),"rb+");
    if(arch==NULL){
        //return 0;
    }
    mbr MBR;
    fseek(arch, 0, SEEK_SET); // Nos posicionamos en el primer byte del archivo
    fread(&MBR,sizeof(mbr),1,arch);
    fclose(arch);
    std::cout << "\n-----------------------------DATOS DEL DISCO--------------------------------\n";
    std::cout << "\nMBR SIGNATURE: "<< MBR.mbr_disk_signature;
    std::cout << "\nMBR SIZE: "<< MBR.mbr_tamano;
    std::cout << "\nTAMAÑO DISPONIBLE: "<< MBR.availableStorage;

    for(int i=0;i<4;i++){
      //  if(MBR.mbr_partitions[i]!=NULL){
        if(MBR.mbr_partitions[i].part_status=='1')  {
            std::cout << "\n\nPARTICION : "<<i;
            std::cout << "\nPARTICION STATUS : "<<MBR.mbr_partitions[i].part_status;
            std::cout << "\nPARTICION TYPE : "<<MBR.mbr_partitions[i].part_type;
            std::cout << "\nPARTICION FIT : "<<MBR.mbr_partitions[i].part_fit;
            std::cout << "\nPARTICION START : "<<MBR.mbr_partitions[i].part_start;
            std::cout << "\nPARTICION SIZE : "<<MBR.mbr_partitions[i].part_size;
            std::cout << "\nPARTICION NAME : "<<MBR.mbr_partitions[i].part_name;
        }
    }
}

void createPartition(ObjFdisk *disk) {
    FILE *file;

    int sizePart;
    file = fopen(disk->path.c_str(), "rb+"); // Se abre con permisos mixtos
    if (file == NULL) {
        cout << "Error el disco no existe" << endl;
       return;
    }

    fseek(file, 0, SEEK_SET); // Moverse 0 caracteres desde el inicio del archivo
    mbr tempMbr;
    // Se recupera lo que hay en el archivo en tempMbr, del tamaño del struct mbr
    fread(&tempMbr, sizeof(mbr), 1, file);
    cout << "ESPACIO DISPONIBLE " << tempMbr.availableStorage << endl;

    if (disk->unity == "K" || disk->unity == "k") {
        sizePart = (disk->size) * 1024;
    } else if (disk->unity == "M" || disk->unity == "m") {
        sizePart = (disk->size) * 1024 * 1024;
    } else {
        sizePart = (disk->size);
    }

    if (sizePart >= tempMbr.availableStorage) {
        cout << "Error, hace falta espacio de almacenamiento" << endl;
    } else {
        tempMbr.availableStorage -= sizePart;
        partition* pointerTempPart = NULL;
        bool n_part = false;
        for (int i = 0; i < 4; i++) {
            if (tempMbr.mbr_partitions[i].part_status == '0'){
                // cout << tempMbr.mbr_partitions[i].part_status << endl;
                pointerTempPart = &tempMbr.mbr_partitions[i];
                n_part = true;
                break;
            }
        }

        if (n_part) {

            if (disk->fit == "BF") {
                pointerTempPart->part_fit = 'BF';
            } else if (disk->fit == "FF") {
                pointerTempPart->part_fit = 'FF';
            } else if (disk->fit == "WF") {
                pointerTempPart->part_fit = 'WF';
            }

            strcpy(pointerTempPart->part_name, disk->name.c_str());
            pointerTempPart->part_size = sizePart;
            pointerTempPart->part_start = sizeof(mbr);
            pointerTempPart->part_status = '1';

            // Si es primaria
            if(disk->type=="P") {
                pointerTempPart->part_type = 'P';

            // Si es extendida
            } else if(disk->type=="E") {

                tempMbr.mbr_partitions[0].part_type = 'E';
                extended ebrPartition;
                ebrPartition.part_fit = '-';
                ebrPartition.part_name[0] = '\0';
                ebrPartition.part_next = -1;
                ebrPartition.part_size = -1;
                ebrPartition.part_start = -1;
                ebrPartition.part_status = '0';

                fseek(file, tempMbr.mbr_partitions[0].part_start, SEEK_SET);
                fwrite(&ebrPartition, sizeof(ebrPartition), 1, file); // Se escribe la particion extendida
            }

            fseek(file, 0, SEEK_SET); // Se modifica el archivo original
            fwrite(&tempMbr, sizeof(mbr), 1, file);
            fclose(file);

            printData(disk->path.c_str());

        } else {
            cout << "Error, ya se llego al límite de particiones" << endl;
        }

    }
}

void ObjFdisk::executeCommand(ObjFdisk *disk) {
    createPartition(disk);
}
