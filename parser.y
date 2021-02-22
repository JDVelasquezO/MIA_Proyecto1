%{
#include "string"
#include "scanner.h" //se importa el header del analisis sintactico
#include <QString>
#include "qdebug.h"
#include <iostream>
#include "objmkdisk.h"
#include "objrmdisk.h"
#include "objfdisk.h"
using namespace std;

extern int yylineno; //linea actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern int columna; //columna actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern char *yytext; //lexema actual donde esta el parser (analisis lexico) lo maneja BISON

int yyerror(const char* mens) {
    std::cout << mens <<" "<<yytext<< std::endl;
    return 0;
}
%}

//error-verbose si se especifica la opcion los errores sintacticos son especificados por BISON
%defines "parser.h"
%output "parser.cpp"
%define parse.error verbose
%locations

%union{
    //se especifican los tipo de valores para los no terminales y lo terminales
    char TEXT[256];
    class ObjMkdisk *mkdisk;
    class objrmdisk *rmdisk;
    class ObjFdisk *fdisk;
}

// TERMINALES TIPO TEXT
%token<TEXT> pmkdisk;
%token<TEXT> prmdisk;
%token<TEXT> pfdisk;
%token<TEXT> pmount;
%token<TEXT> punmount;
%token<TEXT> pmkfs;
%token<TEXT> psize;
%token<TEXT> pfit;
%token<TEXT> punit;
%token<TEXT> ppath;
%token<TEXT> ptype;
%token<TEXT> pdelete;
%token<TEXT> pname;
%token<TEXT> padd;
%token<TEXT> pid;
%token<TEXT> pfs;

%token<TEXT> menos;
%token<TEXT> identificador;
%token<TEXT> igual;
%token<TEXT> entero;
%token<TEXT> cadena;
%token<TEXT> caracter;
%token<TEXT> ruta;
%token<TEXT> fit;
%token<TEXT> unity;
%token<TEXT> unityFd;
%token<TEXT> pcomillas;
%token<TEXT> type;
%token<TEXT> type_del;
%token<TEXT> negativo;

// NO TERMINALES
%type<mkdisk> COMMAND_MKDISK;
%type<rmdisk> COMMAND_RMDISK;
%type<fdisk> COMMAND_FDISK;
%start INICIO
%%

INICIO : LEXPA { }
;

LEXPA: COMMAND_MKDISK {
            $1->executeCommand($1);
        }
      | COMMAND_RMDISK {
            $1->executeCommand($1);
        }
      | COMMAND_FDISK {
            $1->executeCommand($1);
        }
;

COMMAND_MKDISK: pmkdisk {
                    ObjMkdisk *disk = new ObjMkdisk();
                    $$ = disk;
                }
               | COMMAND_MKDISK menos psize igual entero { // -size = 2000

                int size = atoi($5);
                $1->size = size;
                $$ = $1;
            }
               | COMMAND_MKDISK menos pfit igual fit { // -f = BF

                string var_fit = $5;
                $1->fit = var_fit;
                $$ = $1;
            }
               | COMMAND_MKDISK menos punit igual unity { // -u = K

                string var_unity = $5;
                $1->unity = var_unity;
                $$ = $1;
            }
               | COMMAND_MKDISK menos ppath igual ruta { // -path = /home/usr/algo.dk

                string var_path = $5;
                $1->path = var_path;
                $$ = $1;
            }

               | COMMAND_MKDISK menos ppath igual cadena { // -path = "/home/hola a todos/algo.dk"

                string var_path = $5;
                var_path.erase(0, 1);
                var_path.erase(var_path.size()-1, 1);
                $1->path = var_path;
                $$ = $1;
           }
;

COMMAND_RMDISK: prmdisk {
                objrmdisk *disk = new objrmdisk();
                $$ = disk;
            }
            | COMMAND_RMDISK menos ppath igual ruta {
                string var_path = $5;
                $1->path = var_path;
                $$ = $1;
            }

            | COMMAND_RMDISK menos ppath igual cadena {
                string var_path = $5;
                var_path.erase(0, 1);
                var_path.erase(var_path.size()-1, 1);
                $1->path = var_path;

                $$ = $1;
            }
;

COMMAND_FDISK: pfdisk{
                ObjFdisk *disk = new ObjFdisk();
                $$ = disk;
            }
            | COMMAND_FDISK menos psize igual entero {

                    int size = atoi($5);
                    $1->size = size;
                    $$ = $1;
              }
            | COMMAND_FDISK menos ptype igual type {

                    string var_type = $5;
                    $1->type = var_type;
                    $$ = $1;
              }
            | COMMAND_FDISK menos punit igual unityFd {

                    string var_unity = $5;
                    $1->unity = var_unity;
                    $$ = $1;
              }
            | COMMAND_FDISK menos ppath igual ruta {

                    string var_path = $5;
                    $1->path = var_path;
                    $$ = $1;
              }
            | COMMAND_FDISK menos ppath igual cadena { // -path = "/home/hola a todos/algo.dk"

                     string var_path = $5;
                     var_path.erase(0, 1);
                     var_path.erase(var_path.size()-1, 1);
                     $1->path = var_path;
                     $$ = $1;
              }
            | COMMAND_FDISK menos pfit igual fit { // -f = BF

                    string var_fit = $5;
                    $1->fit = var_fit;
                    $$ = $1;
            }
            | COMMAND_FDISK menos pname igual cadena {

                    string var_name = $5;
                    $1->name = var_name;
                    $$ = $1;
              }

            | COMMAND_FDISK menos pdelete igual identificador {

                    string var_path = $5;
                    $1->del = var_path;
                    $$ = $1;
             }

            | COMMAND_FDISK menos pname igual identificador {

                    string var_name = $5;
                    $1->name = var_name;
                    $$ = $1;
              }
;




