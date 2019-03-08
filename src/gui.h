#ifndef _GUI_H
#define _GUI_H
#include"a.h"
#include"zlecenia.h"
#include"wykres.h"
#include"inne.h"

#define GUI_R_NIC             (0x00)
#define GUI_R_PRZYCISK        (0x03)
#define GUI_R_NAPIS           (0x04)
#define GUI_R_POLE_TEKSTOWE   (0x05)
#define GUI_R_PRZELACZNIK     (0x06)


// nazwy przycisków

#define GUI_WYJSCIE                  (0001)
#define GUI_WCZE                     (0002)
#define GUI_NAST                     (0003)
#define GUI_USUN_OBRAZ               (0004)
#define GUI_KOPIUJ_OBRAZ             (0005)
#define GUI_MENU_NOWY_OBRAZ          (1001)
#define GUI_MENU_WCZYTAJ             (1002)
#define GUI_MENU_ZAPISZ              (1003)
#define GUI_MENU_TWORZ               (1004)
#define GUI_MENU_COSTAM              (1005)
#define GUI_MENU_NAPIS               (1006)
#define GUI_NO_POWROT                (2001)
#define GUI_NO_SZER                  (2002)
#define GUI_NO_WYSO                  (2003)
#define GUI_NO_UTWORZ                (2004)
#define GUI_NO_NAPIS                 (2005)
#define GUI_ZAPISZ_POWROT            (3001)
#define GUI_ZAPISZ_NAZWA             (3002)
#define GUI_ZAPISZ_ZAPISZ            (3003)
#define GUI_ZAPISZ_NAPIS             (3004)
#define GUI_TWORZ_POWROT             (4001)
#define GUI_TWORZ_FRAKTAL            (4002)
#define GUI_TWORZ_WYKRES             (4003)
#define GUI_TWORZ_NAPIS              (4004)
#define GUI_FRAKTAL_POWROT           (5001)
#define GUI_FRAKTAL_RODZAJ           (5002)
#define GUI_FRAKTAL_KOLOR            (5003)
#define GUI_FRAKTAL_RZECZYWISTA      (5004)
#define GUI_FRAKTAL_UROJONA          (5005)
#define GUI_FRAKTAL_PRAWO            (5006)
#define GUI_FRAKTAL_GORA             (5007)
#define GUI_FRAKTAL_LEWO             (5008)
#define GUI_FRAKTAL_DOL              (5009)
#define GUI_FRAKTAL_ILOSC            (5010)
#define GUI_FRAKTAL_RYSUJ            (5011)
#define GUI_FRAKTAL_NAPIS            (5012)
#define GUI_WYKRES_POWROT            (6001)
#define GUI_WYKRES_KOLOR             (6002)
#define GUI_WYKRES_PRAWO             (6003)
#define GUI_WYKRES_GORA              (6004)
#define GUI_WYKRES_LEWO              (6005)
#define GUI_WYKRES_DOL               (6006)
#define GUI_WYKRES_LICZNIK           (6007)
#define GUI_WYKRES_MIANOWNIK         (6008)
#define GUI_WYKRES_RYSUJ             (6009)
#define GUI_WYKRES_CZYNNIKRE         (6010)
#define GUI_WYKRES_CZYNNIKIM         (6011)
#define GUI_WYKRES_NAPIS             (6012)
#define GUI_COSTAM_POWROT            (7001)
#define GUI_COSTAM_ROZMYCIE          (7002)
#define GUI_COSTAM_WYOSTRZENIE       (7003)
#define GUI_COSTAM_NAPIS             (7004)


typedef struct gui_pn
{
 uint32_t wartosc;
 uint32_t ktore;
} gui_pn;

extern gui_pn gp_frakKolor[];
extern gui_pn gp_frakRodzaj[];
extern gui_pn gp_wykresKolor[];

typedef struct gui_przycisk
{
 uint32_t prawo;
 uint32_t gora;
 uint32_t lewo;
 uint32_t dol;
 char *t; // to wskaśnik na gotowy napis, więc nie jest czyszczony przez funkcję free
 void(*funkcja)();
} gui_przycisk;
typedef struct gui_przelacznik
{
 uint32_t prawo;
 uint32_t gora;
 uint32_t lewo;
 uint32_t dol;
 uint32_t ilosc;
 uint32_t wybor;
 gui_pn *w; //tablica z wartpsciami i tekstami
 
} gui_przelacznik;
typedef struct gui_napis
{
 uint32_t prawo;
 uint32_t gora;
 uint32_t lewo;
 uint32_t dol;
 //prostokąt, w którym ma być napis
 char *t; //wskaźnik na gotowy napis
} gui_napis;
typedef struct gui_poleTekstowe
{
 uint32_t prawo;
 uint32_t gora;
 uint32_t lewo;
 uint32_t dol;
 char *t;
 char *tt;
 uint32_t dlugosc;
} gui_poleTekstowe;
typedef struct gui_cos
{
 uint8_t rodzaj;
 struct gui_cos *nast;
 struct gui_cos *wcze;
 uint32_t nazwa;
 uint32_t prawo;
 uint32_t gora;
 uint32_t lewo;
 uint32_t dol;
 uint32_t nieWaznosc;
 union
 {
  gui_przycisk     przycisk;
  gui_napis        napis;
  gui_poleTekstowe poleTekstowe;
  gui_przelacznik  przelacznik;
 };
} gui_cos;


uint8_t gui_pt_pisz(gui_poleTekstowe*,char*);
uint8_t gui_pt_cofnij(gui_poleTekstowe*);
uint8_t gui_pt_czysc(gui_poleTekstowe*);


extern gui_cos *gui_poc,*gui_kon;
extern uint32_t gui_iloscRzeczy;

gui_cos *gui_dodaj();
uint8_t gui_usun(gui_cos*co,uint8_t czyOdKonca);
uint8_t gui_czysc();
uint8_t usunMniejWazne(uint32_t);
gui_cos *gui_daj(uint32_t nazwa,uint8_t czyOdKonca);

extern gui_cos *gui_wybrane;
void gui_ruchMyszy(int32_t,int32_t);
void gui_klik(int32_t x,int32_t y);
void gui_odklik(int32_t x,int32_t y);
void gui_klawiatura(SDL_Scancode);
void gui_myszZaznaczenie();
uint8_t gui_czySpelnia(gui_cos*);
uint8_t gui_przelacz(gui_cos*);
void gui_pisanie(char*);
void gui_backspace();
extern int32_t gui_myszX,gui_myszY;
extern uint32_t gui_prawo; // nazwy elementów do wybrania po wciśnięciu przycisków
extern uint32_t gui_gora;
extern uint32_t gui_lewo;
extern uint32_t gui_dol;
extern void(*gui_enter)();
extern void(*gui_escape)();



void gui_fZacznij();
void gui_fWyjscie();
void gui_fKopiuj();
void gui_fUsun();
void gui_fWcze();
void gui_fNast();
void gui_fMenu();
void gui_fNowyObraz();
void gui_fUtworz();
void gui_fZapisz();
void gui_fZapiszx();
void gui_fTworz();
void gui_fFraktal();
void gui_fWykres();
void gui_fFraktalx();
void gui_fWykresx();
void gui_fCostam();
void gui_fRozmycie();
void gui_fWyostrzenie();

#endif