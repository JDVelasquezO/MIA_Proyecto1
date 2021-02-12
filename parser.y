%{
#include "scanner.h" //se importa el header del analisis sintactico
#include <QString>
#include <string>
#include "qdebug.h"
#include <iostream>
#include "objmkdisk.h"
using namespace std;
extern int yylineno; //linea actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern int columna; //columna actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern char *yytext; //lexema actual donde esta el parser (analisis lexico) lo maneja BISON

int yyerror(const char* mens)
{
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
class ObjMkdisk *mdisk;
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

// NO TERMINALES
%type<mdisk> COMMANDMKDISK; // lista de instrucciones
%start INICIO
%%

INICIO : LEXPA { }
;

LEXPA:  pmkdisk COMMANDMKDISK{
    $2->executeCommand($2); // ejecuto el metodo "mostrardatos" del objeto retornado en COMANDOMKDISK
}
;

COMMANDMKDISK:
menos psize igual entero {
    int size=atoi($4);
    ObjMkdisk *disk = new ObjMkdisk();
    disk->size = size;
    $$ = disk;
}


