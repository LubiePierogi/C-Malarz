#ifndef _A_H
#define _A_H
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<math.h>
#include<complex.h>
#include<pthread.h>
#include<GL/glew.h>
#include<SDL2/SDL.h>

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct Czworka
{
 uint8_t e[4];
} Czworka;

typedef struct Obraz
{
 uint32_t szerokosc;
 uint32_t wysokosc;
 Czworka *piksele;
 pthread_mutex_t muteks;
 pthread_mutexattr_t muteksAttr;
 uint8_t czyZmieniono;
} Obraz;
typedef struct HSV
{
 double e[3];
} HSV;



#define LICZBA_PI             (3.141592653589793238462643383279)

#define B_DOBRZE                 (0x00)
#define B_JAKIS_BLAD             (0x01) //jakiś błąd to zawsze 1
#define B_PUSTY_WSK_OBRAZU       (0x02)
#define B_ZLE_WYMIARY            (0x03)
#define B_NIE_OTWARTO_PLIKU      (0x04)
#define B_STRUKTURA_ZAJETA       (0x05)
#define B_PUSTY_WSK_NAPISU       (0x06)
#define B_BRAK_OBRAZU            (0x07)
#define B_ZLE_KOLOROWANIE        (0x08)
#define B_BRAK_MIEJSCA_PAM       (0x09)
#define B_PUSTY_WSK_TD           (0x0a)
#define B_BRAK_TD                (0x0b)
#define B_NIE_WIEM_JESZCZE       (0x0c)
#define B_POZA_ZAKRESEM          (0x0d)
#define B_ZLY_RODZAJ             (0x0e)
#define B_PUSTY_WSKAZNIK         (0x0f)
#define B_BRAK_ZMIAN             (0x10)
#define B_NIE_GOTOWY             (0x11)
#define B_OBRAZ_ZAJETY_PRZEZ_COS (0x12)
#define B_KONIEC_MIEJSC          (0x13)
#define B_PUSTA_LISTA            (0x14)

#define dlugoscNaglowkaBMP       (14)
#define dlugoscNaglowkaDIB       (108)
#define dlugoscNaglowka          (dlugoscNaglowkaBMP+dlugoscNaglowkaDIB)

#define DPI72                    (2835)


#define SZEROKOSC_MENU           (300)
#define PIERWSZA_WYSOKOSC        (700)
#define PIERWSZA_SZEROKOSC       (SZEROKOSC_MENU+PIERWSZA_WYSOKOSC)

//kolory:
// alfa      0
// czerowy   1
// zielony   2
// niebieski 3

//extern Obraz Chleb;

uint8_t obrazCtor(Obraz*);
uint8_t obrazDtor(Obraz*);
uint8_t ustawObraz(Obraz*,uint32_t,uint32_t);
uint8_t kopiujObraz(Obraz*,Obraz*);
uint8_t zniszczObraz(Obraz*);
Czworka intLittleEnd(uint32_t a);
Czworka intBigEnd(uint32_t a);

extern uint8_t bmp[dlugoscNaglowkaBMP];


extern char *napisy[];
extern const Czworka zera;

uint8_t zapiszPlik(Obraz*obraz,char*nazwa);

//
// tutaj jest kolejka z obrazkami
//

typedef struct ListObraz
{
 Obraz obraz;
 struct ListObraz*wcze;
 struct ListObraz*nast;
} ListObraz;


uint8_t dodajObraz();
uint8_t usunWszystkieObrazy();
uint8_t usunObraz();
void ustawPoczatek();
void ustawKoniec();
uint8_t idzDoPrzodu();
uint8_t idzDoTylu();
void wziecieObrazu();

uint8_t czyInny();
uint8_t czyPoczatek(); //zwraca 1 lub 0
uint8_t czyKoniec();

void pokaz();

Obraz*dajObraz();


extern ListObraz*obrazyPoczatek;
extern ListObraz*obrazyKoniec;
extern ListObraz*obrazyMiejsce;
extern uint8_t czyInnyObraz;

extern uint8_t dzialanieOkna;
extern uint8_t czyWyjscie;


#endif