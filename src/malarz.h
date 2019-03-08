#ifndef _MALARZ_H
#define _MALARZ_H
//kolory:
// czerowy   0
// zielony   1
// niebieski 2
// alfa      3

#include"a.h"
#include"doubel.h"
#include"wykres.h"
#include"gui.h"
#include"zlecenia.h"
#include"inne.h"


int main(int,char**);
void*funkcjaWatkuOdOkna(void*);

int32_t wczytajFreeType();
void petla(SDL_Window*);
void rysowanie(SDL_Window*);
void rysowanieObrazka();
void rysowanieMenu();
void rysujTekst(char*,uint32_t,uint32_t,Czworka,float*,float*);
Czworka *rozszerz(Czworka*piksele,uint32_t szerokosc,uint32_t wysokosc,uint32_t nowaSzerokosc,uint32_t nowaWysokosc);

uint8_t ustawTeksture();

#endif