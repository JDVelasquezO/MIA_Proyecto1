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

void ObjFdisk::printData(string path){
    FILE*arch;
    arch=fopen(path.c_str(),"rb+");
    if(arch==NULL){
        //return 0;
    }
    mbr MBR;
    fseek(arch, 0, SEEK_SET);
    fread(&MBR,sizeof(mbr),1,arch);
    fclose(arch);
    std::cout << "\n-------------------------------DATOS DEL DISCO----------------------------------\n";
    std::cout << "\nMBR SIGNATURE: "<< MBR.mbr_disk_signature;
    std::cout << "\nMBR SIZE: "<< MBR.mbr_tamano;


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
    cout << sizeof(mbr) << endl;
    // Se recupera lo que hay en el archivo en tempMbr, del tamaño del mbr escrito antes
    fread(&tempMbr, sizeof(mbr), 1, file);

    cout << tempMbr.mbr_tamano << endl;
    if (disk->unity == "K" || disk->unity == "k") {
        sizePart = (disk->size) * 1024;
    } else if (disk->unity == "M" || disk->unity == "m") {
        sizePart = (disk->size) * 1024 * 1024;
    } else {
        sizePart = (disk->size);
    }

    if (disk->fit == "BF") {
        tempMbr.mbr_partitions[0].part_fit = 'BF';
    } else if (disk->fit == "FF") {
        tempMbr.mbr_partitions[0].part_fit = 'FF';
    } else {
        tempMbr.mbr_partitions[0].part_fit = 'WF';
    }

    strcpy(tempMbr.mbr_partitions[0].part_name, disk->name.c_str());
    tempMbr.mbr_partitions[0].part_size = sizePart;
    tempMbr.mbr_partitions[0].part_start = sizeof(mbr);
    tempMbr.mbr_partitions[0].part_status = '1';

    // Si es primaria
    if(disk->type=="P") {
        tempMbr.mbr_partitions[0].part_type = 'P';

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

}

void ObjFdisk::executeCommand(ObjFdisk *disk) {
    createPartition(disk);
    printData(disk->path.c_str());
}
