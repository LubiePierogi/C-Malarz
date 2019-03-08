#ifndef _ZLECENIA_H
#define _ZLECENIA_H
#include"a.h"
#include"wykres.h"
#include"inne.h"



#define ZLEC_R_NIC             (0)
#define ZLEC_R_WYJSCIE         (1)
#define ZLEC_R_NOWY_OBRAZ      (2)
#define ZLEC_R_NASTEPNY        (3)
#define ZLEC_R_WCZESNIEJSZY    (4)
#define ZLEC_R_POCZATEK        (5)
#define ZLEC_R_KONIEC          (6)
#define ZLEC_R_USUN            (7)
#define ZLEC_R_E               (8)
#define ZLEC_R_ZAPISZ          (9)
#define ZLEC_R_FRAK            (10)
#define ZLEC_R_KOPIUJ          (11)
#define ZLEC_R_WYKRES          (12)
#define ZLEC_R_ROZMYCIE        (13)
#define ZLEC_R_WYOSTRZENIE     (14)

typedef struct Zlecenie
{
 uint32_t rodzaj;
 uint32_t szerokosc;
 uint32_t wysokosc;
 char *str;
 uint16_t kolory;
 uint16_t jaki;
 double prawo;
 double gora;
 double lewo;
 double dol;
 uint32_t iloscPowtorzen;
 double complex e;
 tablicaDubli*td;
 uint32_t kreskaUlamkowa;
 double complex czynnik;
}Zlecenie;

typedef struct ListaZlecen
{
 Zlecenie zlecenie;
 struct ListaZlecen*nast;
}ListaZlecen;



extern uint8_t czyZajety;

uint8_t dodajZlecenie(Zlecenie);
uint8_t wezZlecenie(Zlecenie*);


void ustawZlecenia();
void zwolnijZlecenia();


void *funkcjaOdZlecen(void*);

#endif