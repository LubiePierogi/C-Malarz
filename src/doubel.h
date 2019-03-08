#ifndef _DOUBEL_H
#define _DOUBEL_H



#include"a.h"
#define ILOSCI_MIEJSCA_DLA_TD (8)


typedef struct tablicaDubli
{
 size_t dlugosc;
 size_t miejsce;
 double complex*w;
} tablicaDubli;



uint8_t tablicaDubliCtor(tablicaDubli*a);
uint8_t tDDodaj(tablicaDubli*a,double complex x);
uint8_t tDUsunKonca(tablicaDubli*a);
uint8_t tDUsunKtorys(tablicaDubli*a,size_t ktory);
uint8_t tDWez(tablicaDubli*a,size_t ktory,double complex*x);
uint8_t tDCzysc(tablicaDubli*a);
size_t tDDlugosc(tablicaDubli*a);
size_t tDMiejsce(tablicaDubli*a);



#endif // DUBEL_H