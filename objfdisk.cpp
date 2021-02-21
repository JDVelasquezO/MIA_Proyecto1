#include "objfdisk.h"
#include "iostream"
#include "disk.h"
#include <cstring>
#include "string"
#include "methods.h"
#include "linkedlist.h"
using namespace std;

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

void fillPartitions(bool n_part, LinkedList *linkedListPrimary, LinkedList *linkedListExtended,
                    ObjFdisk *disk, partition *pointerTempPart, FILE *file, mbr &tempMbr, int sizePart) {

    if (n_part) {
        if (linkedListPrimary->length > 3) {
            cout << "Solo pueden existir 3 parciones primarias" << endl;
        } else {

            if (linkedListExtended->length == 1) {
                cout << "Solo puede existir 1 particion extendida " << endl;
            } else {
                if (disk->fit == "BF") {
                    pointerTempPart->part_fit[0] = 'B';
                    pointerTempPart->part_fit[1] = 'F';
                } else if (disk->fit == "FF") {
                    pointerTempPart->part_fit[0] = 'F';
                    pointerTempPart->part_fit[1] = 'F';
                } else if (disk->fit == "WF") {
                    pointerTempPart->part_fit[0] = 'W';
                    pointerTempPart->part_fit[1] = 'F';
                }

                strcpy(pointerTempPart->part_name, disk->name.c_str());
                pointerTempPart->part_size = sizePart;
                pointerTempPart->part_start = sizeof(mbr);
                pointerTempPart->part_status = '1';

                // Si es primaria
                if(disk->type=="P") {
                    pointerTempPart->part_type = 'P';

                // Si es extendida
                } else if(disk->type=="E" || disk->type=="e") {

                    pointerTempPart->part_type = 'E';
                    extended ebrPartition;
                    ebrPartition.part_fit[0] = '\0';
                    ebrPartition.part_name[0] = '\0';
                    ebrPartition.part_next = -1;
                    ebrPartition.part_size = -1;
                    ebrPartition.part_start = -1;
                    ebrPartition.part_status = '0';

                    fseek(file, tempMbr.mbr_partitions[0].part_start, SEEK_SET);
                    fwrite(&ebrPartition, sizeof(ebrPartition), 1, file); // Se escribe la particion extendida
                }
                // Si es logica
                else if (disk->type == "L" || disk->type == "l") {
                    cout << "Particion Logica" << endl;
                }

                fseek(file, 0, SEEK_SET); // Se modifica el archivo original
                fwrite(&tempMbr, sizeof(mbr), 1, file);
                fclose(file);

                printData(disk->path.c_str());
            }
        }


    } else {
        cout << "Error, ya se llego al límite de particiones" << endl;
    }
}

void validateRequeriments(int sizePart, mbr tempMbr, ObjFdisk *disk, FILE *file) {
    LinkedList *linkedListPrimary = new LinkedList();
    LinkedList *linkedListExtended = new LinkedList();

    if (sizePart >= tempMbr.availableStorage) {
        cout << "Error, hace falta espacio de almacenamiento" << endl;
    } else {
        tempMbr.availableStorage -= sizePart;
        partition* pointerTempPart = NULL;
        bool n_part = false;
        for (int i = 0; i < 4; i++) {
            if (disk->type == "E" || disk->type == "e") {
                linkedListExtended->addStart('E');
            } else if (disk->type == "L" || disk->type == "l") {
                cout << "Particion Logica" << endl;
            } else {
                linkedListPrimary->addStart('P');
            }

            if (tempMbr.mbr_partitions[i].part_status == '0') {
                pointerTempPart = &tempMbr.mbr_partitions[i];
                n_part = true;
                break;
            }
        }

        fillPartitions(n_part, linkedListPrimary, linkedListExtended, disk, pointerTempPart, file,
                       tempMbr, sizePart);
    }
}

void createPartition(ObjFdisk *disk) {
    FILE *file;

    int sizePart;
    file = fopen(disk->path.c_str(), "rb+"); // Se abre con permisos mixtos
    if (file == NULL) {
        cout << "Error: El disco no existe" << endl;
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

    validateRequeriments(sizePart, tempMbr, disk, file);
}

string createFolder(ObjFdisk *disk) {
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

void ObjFdisk::executeCommand(ObjFdisk *disk) {

    string path = createFolder(disk);
    if (Methods::isDirExist(path)) {
        createPartition(disk);
    } else {
        cout << "La ruta no existe";
    }

}
