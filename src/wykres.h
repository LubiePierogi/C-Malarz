#ifndef _WYKRES_H
#define _WYKRES_H

#define KOLOR_W_PODSTAWOWE     (0x0000)
#define KOLOR_W_JAKIES_INNE    (0x0001)

#define KOLOR_F_PODSTAWOWE     (0x0000)
#define KOLOR_F_CZARNOBIALE    (0x0001)

#define ZBIOR_J                (0x0000)
#define ZBIOR_M                (0x0001)
#define JAKIS_INNY_ZBIOR       (0x0002)
#define PLONOUCY_STATEK        (0x0003)
#define PLONOUCY_STATEK_DWA    (0x0004)
#define JAKIS_WZOREK           (0x0005)
#define ZBIOR_FAJNY            (0x0006)
#define ZBIOR_FAJNY2           (0x0007)
#define ZBIOR_FAJNY3           (0x0008)

#include"a.h"
#include"doubel.h"

uint8_t rysowanieWykresu
(
 Obraz*obraz,
 uint16_t kolory,
 double prawo,
 double gora,
 double lewo,
 double dol,
 double complex czynnik,
 tablicaDubli*liczby,
 uint32_t kreska
);


uint8_t rysujFrak
(
 Obraz*obraz,
 uint16_t kolory,
 uint16_t jaki,
 double prawo,
 double gora,
 double lewo,
 double dol,
 uint32_t iloscPowtorzen,
 double complex e
);

Czworka lZNaRGBA(double complex);
HSV lZNaHSV(double complex);
double koutNaH(double);
Czworka HSVNaRGB(HSV);
Czworka wykresKolorInny(double complex);

double complex wielomian(double complex x,double complex czynnik,tablicaDubli*liczby,uint32_t kreska);

Czworka zwykleKolorowanie(uint32_t chlebek);
Czworka kolorCB(uint32_t chlebek);

uint32_t zbiorM(double complex x,double complex y,uint32_t powt);
uint32_t zbiorJ(double complex x,double complex y,uint32_t powt);
uint32_t zbiorInny(double complex x,double complex y,uint32_t powt);
uint32_t zbiorStatek(double complex x,double complex y,uint32_t powt);
uint32_t zbiorStatekJ(double complex x,double complex y,uint32_t powt);
uint32_t zbiorHehe(double complex x,double complex y,uint32_t powt);
uint32_t zbiorFajny(double complex x,double complex y,uint32_t powt);
uint32_t zbiorFajny2(double complex x,double complex y,uint32_t powt);
uint32_t zbiorFajny3(double complex x,double complex y,uint32_t powt);


#endif