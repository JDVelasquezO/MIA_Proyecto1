#include "objmkdisk.h"
#include "string"
#include <iostream>
using namespace std;

ObjMkdisk::ObjMkdisk()
{
    unity = 'C';
    path = "/";
    fit = "FF";
}

void ObjMkdisk::executeCommand(ObjMkdisk *disk) {

    cout << "Datos del Disco " << disk->unity << endl;
    cout << "El tamaño es: " << disk->size << endl;
    cout << "La ruta es: " << disk->path << endl;
    cout << "El ajuste es: " << disk->fit << endl;
}
