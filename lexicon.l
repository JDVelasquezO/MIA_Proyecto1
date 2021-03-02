%option case-insensitive
%option yylineno

%{
#include <iostream>
#include <QString>
#include <string>
#include <QList>
#include "parser.h"

    int yylex();
    void yyerror(const char *s);
    int linea = 0;
    int columna = 1;
    int anterior = 0;


%}
%option noyywrap
%option nounput

entero [0-9]+
letra [a-zA-ZÑñáéíóúÁÉÍÓÚ]+
fit [B|F|W]F
unity [k]|[m]|[K]|[M]|[B]|[b]
type [P]|[E]|[L]|[p]|[e]|[l]
type_del [fast]|[full]

identificador {letra}({letra}|{entero}|"_"|".")*
caracter "\'"("\\")?({letra}|{entero})"\'"
cadena [\"][^\"\n]*[\"\n]
ruta (\/{identificador})+(".dk")
negativo ["-"]?[{entero}]

%x COM
%%

"MKDISK" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return pmkdisk;}
"RMDISK" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return prmdisk;}
"FDISK" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return pfdisk;}
"MOUNT" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return pmount;}
"UNMOUNT" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return punmount;}
"MKFS" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return pmkfs;}

"SIZE" {columna=columna+ anterior;  anterior = strlen(yytext); strcpy(yylval.TEXT, yytext);return psize;}
"F" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return pfit;}
"U" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return punit;}
"PATH" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return ppath;}
"TYPE" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return ptype;}
"DELETE" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return pdelete;}
"NAME" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return pname;}
"ADD" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return padd;}
"ID" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return pid;}
"FS" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return pfs;}

{entero} {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext); return entero;}
{cadena} {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext); return cadena;}
{caracter} {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext); return caracter;}
{fit} {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext); return fit;}
{unity} {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext); return unity;}
{ruta} {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext); return ruta;}
{type} {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext); return type;}
{type_del} {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext); return type_del;}
{identificador} {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext); return identificador;}
{negativo} {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext); return negativo;}
"=" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return igual;}
"-" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return menos;}
'\"' {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return pcomillas;}

"\n" {linea++; columna=0; anterior=0;}
[[:blank:]] { columna++; }

.   {

std::cout<<"Error Lexico: "<<linea<<"\n";
//printf("Error lexico en linea %d",linea);
}
