#include "objmkdisk.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "time.h"
#include "disk.h"
using namespace std;

ObjMkdisk::ObjMkdisk()
{
    unity = "k";
    path = "/";
    fit = "FF";
}

bool isDirExist(const std::string& path) {

    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

bool makePath(const std::string& path) {

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
        return isDirExist(path);

    default:
        return false;
    }
}

string createFolder(ObjMkdisk *disk) {
    string route = disk->path.c_str();
    string delimiter = "/";

    size_t pos = 0;
    string token;
    string newRoute = "";
    while ((pos = route.find(delimiter)) != string::npos) {
        token = route.substr(0, pos);
        // cout << token << std::endl;
        newRoute += token + "/";
        // cout << newRoute << endl;
        route.erase(0, pos + delimiter.length());
    }

    return newRoute;
}

void ObjMkdisk::assignUnity(ObjMkdisk *disk){

    FILE *file;
    char buffer[1024];

    string newRoute = createFolder(disk);
    makePath(newRoute);

    file = fopen(disk->path.c_str(), "wb");
    if (disk->unity == "K" || disk->unity == "k") {
        // newMbr.mbr_tamano = (disk->size)*1024;
        for (int i = 0; i < 1024; i++) {
            buffer[i] = '\0';
        }

        for (int i = 0; i < disk->size; i++) {
            fwrite(&buffer, 1024, 1, file);
        }

        fclose(file);
    }

    if (disk->unity == "M" || disk->unity == "m") {
        // newMbr.mbr_tamano = (disk->size)*1024*1024;
        for (int i = 0; i < 1024; i++) {
            buffer[i] = '\0';
        }

        for (int i = 0; i < (disk->size * 1024); i++) {
            fwrite(&buffer, 1024, 1, file);
        }

        fclose(file);
    }
}

void ObjMkdisk::executeCommand(ObjMkdisk *disk) {

    cout << "------------INFORMACION DEL DISCO---------------------" << endl;
    cout << "Datos del Disco " << disk->unity << endl;
    cout << "El tamaño es: " << disk->size << endl;
    cout << "La ruta es: " << disk->path << endl;
    cout << "El ajuste es: " << disk->fit << endl;

    // mbr newMbr;


    assignUnity(disk);
}


















