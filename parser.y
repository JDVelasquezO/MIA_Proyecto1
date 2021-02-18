%{
#include "scanner.h" //se importa el header del analisis sintactico
#include <QString>
#include <string>
#include "qdebug.h"
#include <iostream>
#include "objmkdisk.h"
#include "objrmdisk.h"

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
%token<TEXT> igual;
%token<TEXT> entero;
%token<TEXT> cadena;
%token<TEXT> caracter;
%token<TEXT> ruta;
%token<TEXT> fit;
%token<TEXT> unity;

// NO TERMINALES
%type<mkdisk> COMMAND_MKDISK;
%type<rmdisk> COMMAND_RMDISK;
%start INICIO
%%

INICIO : LEXPA { }
;

LEXPA:  pmkdisk COMMAND_MKDISK {
            $2->executeCommand($2);
        }
      | prmdisk COMMAND_RMDISK {
            $2->executeCommand($2);
        }
;

COMMAND_MKDISK: menos psize igual entero {

                int size = atoi($4);
                ObjMkdisk *disk = new ObjMkdisk();
                disk->size = size;
                $$ = disk;
            }
               | COMMAND_MKDISK menos pfit igual fit {

                string var_fit = $5;
                $1->fit = var_fit;
                $$ = $1;
            }
               | COMMAND_MKDISK menos punit igual unity {

                string var_unity = $5;
                $1->unity = var_unity;
                $$ = $1;
            }
               | COMMAND_MKDISK menos ppath igual ruta {

                string var_path = $5;
                $1->path = var_path;
                $$ = $1;
            }

               | COMMAND_MKDISK menos ppath igual cadena {

                string var_path = $5;
                var_path.erase(0, 1);
                var_path.erase(var_path.size()-1, 1);
                $1->path = var_path;
                $$ = $1;
           }
;

COMMAND_RMDISK: menos ppath igual ruta {

                string var_path = $4;
                objrmdisk *disk = new objrmdisk();
                disk->path = var_path;
                $$ = disk;
            }
            | COMMAND_RMDISK menos ppath igual cadena {

                 string var_path = $5;
                 var_path.erase(0, 1);
                 var_path.erase(var_path.size()-1, 1);
                 $1->path = var_path;
                 $$ = $1;
            }
;
