#include "objrmdisk.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "methods.h"
#include "string"
using namespace std;

objrmdisk::objrmdisk()
{

}

string createFolder(objrmdisk *disk) {
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

bool removeDisk(objrmdisk *disk) {
    string path = createFolder(disk);
    if (Methods::isDirExist(path)) {
        remove(disk->path.c_str());
        return true;
    } else {
        return false;
    }
}

void objrmdisk::executeCommand(objrmdisk *disk) {

    cout << "\n" << endl;
    cout << "------------INFORMACION DEL DISCO---------------------" << endl;
    cout << "La ruta es: " << disk->path << endl;
    if (removeDisk(disk)) {
        cout << "Disco eliminado exitosamente" << endl;
    } else {
        cout << "No se pudo eliminar archivo" << endl;
    }
}
