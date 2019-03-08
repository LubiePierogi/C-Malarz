#include"gui.h"


gui_cos *gui_poc=NULL,*gui_kon=NULL;
uint32_t gui_iloscRzeczy=0;
gui_cos *gui_wybrane=NULL;
gui_cos *gui_klikniete=NULL; //wskaźnik na klikniętą rzecz w gui
int32_t gui_myszX=0,gui_myszY=0;
uint32_t gui_prawo=0; // nazwy elementów do wybrania po wciśnięciu przycisków
uint32_t gui_gora=0;
uint32_t gui_lewo=0;
uint32_t gui_dol=0;
void(*gui_enter)()=NULL;
void(*gui_escape)()=NULL;



gui_pn gp_frakKolor[]=
{
 {KOLOR_F_PODSTAWOWE,22},
 {KOLOR_F_CZARNOBIALE,23}
};
gui_pn gp_frakRodzaj[]=
{
 {ZBIOR_J,24},
 {ZBIOR_M,25},
 {JAKIS_INNY_ZBIOR,26},
 {PLONOUCY_STATEK,27},
 {PLONOUCY_STATEK_DWA,28},
 {JAKIS_WZOREK,29},
 {ZBIOR_FAJNY,30},
 {ZBIOR_FAJNY2,31},
 {ZBIOR_FAJNY3,32}
};
gui_pn gp_wykresKolor[]=
{
 {KOLOR_W_PODSTAWOWE,36},
 {KOLOR_W_JAKIES_INNE,37}
};


uint8_t gui_pt_pisz(gui_poleTekstowe*pole,char*co)
{
 if(pole==NULL||co==NULL) return B_PUSTY_WSKAZNIK;
 //printf("Dodawanie %s\n",co);
 //printf("Wskaźnik %p\n",pole->t);
 //if(pole->t)
 // printf("To teraz jest %s\n",pole->t);
 uint32_t d=0;
 char*it=co;
 char*e=NULL;
 while(*it)
 {
  ++d;
  ++it;
 }
 //printf("Długość %" PRIu32 "\n",pole->dlugosc);
 //printf("dodanie  %" PRIu32 " ###\n",d);
 pole->dlugosc+=d;
 //printf("Długość %" PRIu32 "\n",pole->dlugosc);
 e=realloc(pole->t,pole->dlugosc+1);
 if(!e)
 {
  pole->dlugosc-=d;
  return B_BRAK_MIEJSCA_PAM;
 }
 pole->t=e;
 e+=pole->dlugosc-d;
 while(1)
 {
  *e=*co;
  if(!*e) break;
  ++e;
  ++co;
 };
 //printf("Wynik %s\n",pole->t);
 //printf("Wskaźnik %p\n",pole->t);
 return B_DOBRZE;
}
uint8_t gui_pt_cofnij(gui_poleTekstowe*pole)
{
 if(pole==NULL) return B_PUSTY_WSKAZNIK;
 char*it=NULL;
 char c=0;
 uint32_t ile=0;
 //printf("Długość %" PRIu32 "\n",pole->dlugosc);
 //printf("Wskaźnik %p\n",pole->t);
 it=(pole->t)+(pole->dlugosc);
 while(it>pole->t)
 {
  --it;
  ++ile;
  c=(*it)&(char)0xc0;
  printf("%" PRIx16 "\n",(uint16_t)(uint8_t)c);
  if(c!=(char)0x80)
  {
   *it=0;
   break;
  }
 }
 //printf("odjęcie  %" PRIu32 " ###\n",ile);
 pole->dlugosc-=ile;
 if(pole->dlugosc>0)
  pole->t=realloc(pole->t,pole->dlugosc+1);
 else
 {
  free(pole->t);
  pole->t=NULL;
 }
 //printf("Długość %" PRIu32 "\n",pole->dlugosc);
 //printf("Wynik %s\n",pole->t);
 //int i=0;
 //char*ptr=pole->t;
 //printf("Wskaźnik %p\n",pole->t);
 return B_DOBRZE;
}
uint8_t gui_pt_czysc(gui_poleTekstowe*pole)
{
 if(pole==NULL) return B_PUSTY_WSKAZNIK;
 free(pole->t);
 pole->dlugosc=0;
 return 0;
}


gui_cos *gui_dodaj()
{
 gui_cos *w=malloc(sizeof(gui_cos));
 if(!w) return NULL;
 char*ptr=(char*)w;
 uint32_t ile=0;
 while(ile<sizeof(gui_cos))
 {
  *ptr='\0';
  ++ile;
  ++ptr;
 }
 if(gui_kon)
 {
  gui_kon->nast=w;
  w->wcze=gui_kon;
 }
 else
 {
  gui_poc=w;
  w->wcze=NULL;
 }
 w->nast=NULL;
 gui_kon=w;
 w->rodzaj=GUI_R_NIC;
 ++gui_iloscRzeczy;
 return w;
}
uint8_t gui_usun(gui_cos*co,uint8_t czyOdKonca)
{
 gui_cos *w=NULL;
 if(!co) return 1;
 w=czyOdKonca?gui_kon:gui_poc;
 while(w)
 {
  if(co==w)
  {
   if(w->nast)
    w->nast->wcze=w->wcze;
   else
    gui_kon=w->wcze;
   if(w->wcze)
    w->wcze->nast=w->nast;
   else
    gui_poc=w->nast;
   if(w->rodzaj==GUI_R_POLE_TEKSTOWE&&w->poleTekstowe.t)
   {
    free(w->poleTekstowe.t);
   }
   free(w);
   --gui_iloscRzeczy;
   return 0;
  }
  w=czyOdKonca?w->wcze:w->nast;
 }
 return 1;
}
uint8_t gui_czysc()
{
 gui_cos *w=NULL,*x=NULL;
 w=gui_poc;
 while(w)
 {
  x=w;
  w=w->nast;
  free(x);
 }
 gui_poc=NULL;
 gui_kon=NULL;
 gui_iloscRzeczy=0;
 return 0;
}
uint8_t gui_usunMniejWazne(uint32_t ile)
{
 gui_cos *w=NULL,*x=NULL;
 w=gui_poc;
 while(w)
 {
  x=w;
  w=w->nast;
  if(x->nieWaznosc>=ile)
  {
   if(x->wcze)
    x->wcze->nast=x->nast;
   else
    gui_poc=x->nast;
   if(x->nast)
    x->nast->wcze=x->wcze;
   else
    gui_kon=x->wcze;
   free(x);
   --gui_iloscRzeczy;
  }
 }
 return 0;
}
gui_cos *gui_daj(uint32_t nazwa,uint8_t czyOdKonca)
{
 gui_cos *w=NULL;
 if(!nazwa) return NULL;
 if(czyOdKonca)
 {
  w=gui_kon;
  while(w)
  {
   if(w->nazwa==nazwa)
   {
    return w;
   }
   w=w->wcze;
  }
  return NULL;
 }
 else
 {
  w=gui_poc;
  while(w)
  {
   if(w->nazwa==nazwa)
   {
    return w;
   }
   w=w->nast;
  }
  return NULL;
 }
}





void gui_ruchMyszy(int32_t x,int32_t y)
{
 gui_myszX=x;
 gui_myszY=y;
 gui_myszZaznaczenie();
}
void gui_klik(int32_t x,int32_t y)
{
 gui_ruchMyszy(x,y);
 if(gui_czySpelnia(gui_wybrane))
 {
  gui_klikniete=gui_wybrane;
 }
}
void gui_odklik(int32_t x,int32_t y)
{
 void(*rzecz)()=NULL;
 if(gui_klikniete!=NULL)
 {
  if(gui_czySpelnia(gui_klikniete))
  {
   switch(gui_klikniete->rodzaj)
   {
    case GUI_R_PRZYCISK:
     rzecz=gui_klikniete->przycisk.funkcja;
     break;
    case GUI_R_PRZELACZNIK:
     gui_przelacz(gui_klikniete);
     break;
   }
  }
  gui_klikniete=NULL;
 }
 if(rzecz) rzecz();
}


//////
uint8_t gui_przelacz(gui_cos*rzecz)
{
 if(!rzecz)
  return 1;
 ++rzecz->przelacznik.wybor;
 if(rzecz->przelacznik.wybor>=rzecz->przelacznik.ilosc)
  rzecz->przelacznik.wybor=0;
 return 0;
}
//////
uint8_t gui_czySpelnia(gui_cos*rzecz)
{
 return
  rzecz!=NULL &&
  (
   (
    rzecz->rodzaj==GUI_R_PRZYCISK &&
    rzecz->przycisk.prawo>=gui_myszX &&
    rzecz->przycisk.gora<=gui_myszY &&
    rzecz->przycisk.lewo<=gui_myszX &&
    rzecz->przycisk.dol>=gui_myszY
   )
   ||
   (
    rzecz->rodzaj==GUI_R_PRZELACZNIK &&
    rzecz->przelacznik.prawo>=gui_myszX &&
    rzecz->przelacznik.gora<=gui_myszY &&
    rzecz->przelacznik.lewo<=gui_myszX &&
    rzecz->przelacznik.dol>=gui_myszY
   )
   ||
   (
    rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
    rzecz->poleTekstowe.prawo>=gui_myszX &&
    rzecz->poleTekstowe.gora<=gui_myszY &&
    rzecz->poleTekstowe.lewo<=gui_myszX &&
    rzecz->poleTekstowe.dol>=gui_myszY
   )
  )
 ;
}
void gui_myszZaznaczenie()
{
 gui_cos *rzecz=gui_poc;
 while(rzecz!=NULL)
 {
  if(gui_czySpelnia(rzecz))
  {
   gui_wybrane=rzecz;
   break;
  }
  rzecz=rzecz->nast;
 }
}
void gui_klawiatura(SDL_Scancode przycisk)
{
 void(*funkcja)()=NULL;
 uint32_t nazwa=0;
 switch(przycisk)
 {
  case SDL_SCANCODE_RETURN:
   if
   (
    gui_wybrane &&
    (
     (
      gui_wybrane->rodzaj==GUI_R_PRZYCISK &&
      (funkcja=gui_wybrane->przycisk.funkcja)!=NULL
     )
     ||
     (
      gui_wybrane->rodzaj==GUI_R_PRZELACZNIK &&
      gui_przelacz(gui_wybrane)
     )
    )
   )
   {
    funkcja();
   }
   else if((funkcja=gui_enter)!=NULL)
    funkcja();
   break;
  case SDL_SCANCODE_RIGHT:
   if
   (
    gui_wybrane &&
    (nazwa=gui_wybrane->prawo)!=0
   )
   {
    gui_wybrane=gui_daj(nazwa,0);
   }
   else if((nazwa=gui_prawo)!=0)
    gui_wybrane=gui_daj(nazwa,0);
   break;
  case SDL_SCANCODE_UP:
   if
   (
    gui_wybrane &&
    (nazwa=gui_wybrane->gora)!=0
   )
   {
    gui_wybrane=gui_daj(nazwa,0);
   }
   else if((nazwa=gui_gora)!=0)
    gui_wybrane=gui_daj(nazwa,0);
   break;
  case SDL_SCANCODE_LEFT:
   if
   (
    gui_wybrane &&
    (nazwa=gui_wybrane->lewo)!=0
   )
   {
    gui_wybrane=gui_daj(nazwa,0);
   }
   else if((nazwa=gui_lewo)!=0)
    gui_wybrane=gui_daj(nazwa,0);
   break;
  case SDL_SCANCODE_DOWN:
   if
   (
    gui_wybrane &&
    (nazwa=gui_wybrane->dol)!=0
   )
   {
    gui_wybrane=gui_daj(nazwa,0);
   }
   else if((nazwa=gui_dol)!=0)
    gui_wybrane=gui_daj(nazwa,0);
   break;
  case SDL_SCANCODE_ESCAPE:
    if(gui_escape) gui_escape();
   break;
  case SDL_SCANCODE_BACKSPACE:
    gui_backspace();
   break;
  default:
   break;
 }
}


void gui_pisanie(char*co)
{
 if(co&&gui_wybrane&&gui_wybrane->rodzaj==GUI_R_POLE_TEKSTOWE)
  gui_pt_pisz(&(gui_wybrane->poleTekstowe),co);
}
void gui_backspace()
{
 if(gui_wybrane&&gui_wybrane->rodzaj==GUI_R_POLE_TEKSTOWE)
  gui_pt_cofnij(&(gui_wybrane->poleTekstowe));
}

void gui_fZacznij()
{
 gui_cos*e=NULL;



 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_WYJSCIE;
 e->prawo=0;
 e->gora=GUI_USUN_OBRAZ;
 e->lewo=0;
 e->dol=0;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=PIERWSZA_WYSOKOSC-10-35;
 e->przycisk.lewo=10;
 e->przycisk.dol=PIERWSZA_WYSOKOSC-10;
 e->przycisk.t=napisy[1];
 e->przycisk.funkcja=gui_fWyjscie;
 e->nieWaznosc=0;




 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_USUN_OBRAZ;
 e->prawo=0;
 e->gora=GUI_KOPIUJ_OBRAZ;
 e->lewo=0;
 e->dol=GUI_WYJSCIE;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=PIERWSZA_WYSOKOSC-10-35-45;
 e->przycisk.lewo=10;
 e->przycisk.dol=PIERWSZA_WYSOKOSC-10-45;
 e->przycisk.t=napisy[10];
 e->przycisk.funkcja=gui_fUsun;
 e->nieWaznosc=0;


 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_KOPIUJ_OBRAZ;
 e->prawo=0;
 e->gora=GUI_WCZE;
 e->lewo=0;
 e->dol=GUI_USUN_OBRAZ;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=PIERWSZA_WYSOKOSC-10-35-90;
 e->przycisk.lewo=10;
 e->przycisk.dol=PIERWSZA_WYSOKOSC-10-90;
 e->przycisk.t=napisy[9];
 e->przycisk.funkcja=gui_fKopiuj;
 e->nieWaznosc=0;



 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_WCZE;
 e->prawo=GUI_NAST;
 e->gora=GUI_MENU_COSTAM;
 e->lewo=0;
 e->dol=GUI_KOPIUJ_OBRAZ;
 e->przycisk.prawo=SZEROKOSC_MENU/2-5;
 e->przycisk.gora=PIERWSZA_WYSOKOSC-10-35-135;
 e->przycisk.lewo=10;
 e->przycisk.dol=PIERWSZA_WYSOKOSC-10-135;
 e->przycisk.t=napisy[12];
 e->przycisk.funkcja=gui_fWcze;
 e->nieWaznosc=0;


 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_NAST;
 e->prawo=0;
 e->gora=GUI_MENU_COSTAM;
 e->lewo=GUI_WCZE;
 e->dol=GUI_KOPIUJ_OBRAZ;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=PIERWSZA_WYSOKOSC-10-35-135;
 e->przycisk.lewo=SZEROKOSC_MENU/2+5;
 e->przycisk.dol=PIERWSZA_WYSOKOSC-10-135;
 e->przycisk.t=napisy[11];
 e->przycisk.funkcja=gui_fNast;
 e->nieWaznosc=0;


 gui_fMenu();
}
void gui_fWyjscie()
{
 gui_czysc();
 gui_escape=NULL;
 Zlecenie zlecenie;
 zlecenie.rodzaj=ZLEC_R_WYJSCIE;
 dodajZlecenie(zlecenie);
}
void gui_fKopiuj()
{
 Zlecenie zlecenie;
 zlecenie.rodzaj=ZLEC_R_KOPIUJ;
 dodajZlecenie(zlecenie);
}
void gui_fUsun()
{
 Zlecenie zlecenie;
 zlecenie.rodzaj=ZLEC_R_USUN;
 dodajZlecenie(zlecenie);
}
void gui_fWcze()
{
 Zlecenie zlecenie;
 zlecenie.rodzaj=ZLEC_R_WCZESNIEJSZY;
 dodajZlecenie(zlecenie);
}
void gui_fNast()
{
 Zlecenie zlecenie;
 zlecenie.rodzaj=ZLEC_R_NASTEPNY;
 dodajZlecenie(zlecenie);
}
void gui_fMenu()
{
 gui_usunMniejWazne(10);
 gui_cos *e=NULL;
 gui_escape=NULL;



 e=gui_dodaj();
 e->rodzaj=GUI_R_NAPIS;
 e->nazwa=GUI_MENU_NAPIS;
 e->napis.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35;
 e->napis.t=napisy[5];
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_MENU_NOWY_OBRAZ;
 e->prawo=0;
 e->gora=0;
 e->lewo=0;
 e->dol=GUI_MENU_ZAPISZ;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+45;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+45;
 e->przycisk.t=napisy[3];
 e->przycisk.funkcja=gui_fNowyObraz;
 e->nieWaznosc=10;



 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_MENU_ZAPISZ;
 e->prawo=0;
 e->gora=GUI_MENU_NOWY_OBRAZ;
 e->lewo=0;
 e->dol=GUI_MENU_TWORZ;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+90;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+90;
 e->przycisk.t=napisy[6];
 e->przycisk.funkcja=gui_fZapisz;
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_MENU_TWORZ;
 e->prawo=0;
 e->gora=GUI_MENU_ZAPISZ;
 e->lewo=0;
 e->dol=GUI_MENU_COSTAM;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+135;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+135;
 e->przycisk.t=napisy[7];
 e->przycisk.funkcja=gui_fTworz;
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_MENU_COSTAM;
 e->prawo=0;
 e->gora=GUI_MENU_TWORZ;
 e->lewo=0;
 e->dol=GUI_WCZE;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+180;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+180;
 e->przycisk.t=napisy[8];
 e->przycisk.funkcja=gui_fCostam;
 e->nieWaznosc=10;
 e=gui_daj(GUI_WCZE,0);
 if(e)
 {
  e->gora=GUI_MENU_COSTAM;
 }
 e=gui_daj(GUI_NAST,0);
 if(e)
 {
  e->gora=GUI_MENU_COSTAM;
 }
 gui_wybrane=gui_daj(GUI_MENU_NOWY_OBRAZ,0);
}
void gui_fNowyObraz()
{
 gui_usunMniejWazne(10);
 gui_cos *e=NULL;
 gui_escape=gui_fMenu;


 e=gui_dodaj();
 e->rodzaj=GUI_R_NAPIS;
 e->nazwa=GUI_NO_NAPIS;
 e->napis.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35;
 e->napis.t=napisy[3];
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_NO_POWROT;
 e->prawo=0;
 e->gora=0;
 e->lewo=0;
 e->dol=GUI_NO_SZER;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+45;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+45;
 e->przycisk.t=napisy[35];
 e->przycisk.funkcja=gui_fMenu;
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_NO_SZER;
 e->prawo=GUI_NO_WYSO;
 e->gora=GUI_NO_POWROT;
 e->lewo=0;
 e->dol=GUI_NO_UTWORZ;
 e->poleTekstowe.prawo=SZEROKOSC_MENU/2-5;
 e->poleTekstowe.gora=10+135;
 e->poleTekstowe.lewo=10;
 e->poleTekstowe.dol=10+35+135;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[14];
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_NO_WYSO;
 e->prawo=0;
 e->gora=GUI_NO_POWROT;
 e->lewo=GUI_NO_SZER;
 e->dol=GUI_NO_UTWORZ;
 e->poleTekstowe.prawo=SZEROKOSC_MENU-10;
 e->poleTekstowe.gora=10+135;
 e->poleTekstowe.lewo=SZEROKOSC_MENU/2+5;
 e->poleTekstowe.dol=10+35+135;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[15];
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_NO_UTWORZ;
 e->prawo=0;
 e->gora=GUI_NO_SZER;
 e->lewo=0;
 e->dol=GUI_WCZE;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+180;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+180;
 e->przycisk.t=napisy[13];
 e->przycisk.funkcja=gui_fUtworz;
 e->nieWaznosc=10;

 e=gui_daj(GUI_WCZE,0);
 if(e)
 {
  e->gora=GUI_NO_UTWORZ;
 }
 e=gui_daj(GUI_NAST,0);
 if(e)
 {
  e->gora=GUI_NO_UTWORZ;
 }
 gui_wybrane=gui_daj(GUI_NO_POWROT,0);
}
void gui_fUtworz()
{
 Zlecenie zlecenie;
 gui_cos*rzecz=NULL;
 zlecenie.rodzaj=ZLEC_R_NOWY_OBRAZ;
 rzecz=gui_daj(GUI_NO_SZER,0);
 if
 (
  !
  (
   rzecz &&
   rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
   rzecz->poleTekstowe.t &&
   sscanf(rzecz->poleTekstowe.t,"%" PRIu32,&zlecenie.szerokosc)>0
  )
 )
  zlecenie.szerokosc=1000;
 rzecz=gui_daj(GUI_NO_WYSO,0);
 if
 (
  !
  (
   rzecz &&
   rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
   rzecz->poleTekstowe.t &&
   sscanf(rzecz->poleTekstowe.t,"%" PRIu32,&zlecenie.wysokosc)>0
  )
 )
  zlecenie.wysokosc=1000;
 dodajZlecenie(zlecenie);
}
void gui_fZapisz()
{

 gui_usunMniejWazne(10);
 gui_cos *e=NULL;
 gui_escape=gui_fMenu;




 e=gui_dodaj();
 e->rodzaj=GUI_R_NAPIS;
 e->nazwa=GUI_ZAPISZ_NAPIS;
 e->napis.prawo=SZEROKOSC_MENU-10;
 e->napis.gora=10;
 e->napis.lewo=10;
 e->napis.dol=10+35;
 e->napis.t=napisy[6];
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_ZAPISZ_POWROT;
 e->prawo=0;
 e->gora=0;
 e->lewo=0;
 e->dol=GUI_ZAPISZ_NAZWA;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+45;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+45;
 e->przycisk.t=napisy[35];
 e->przycisk.funkcja=gui_fMenu;
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_ZAPISZ_NAZWA;
 e->prawo=0;
 e->gora=GUI_ZAPISZ_POWROT;
 e->lewo=0;
 e->dol=GUI_ZAPISZ_ZAPISZ;
 e->poleTekstowe.prawo=SZEROKOSC_MENU-10;
 e->poleTekstowe.gora=10+135;
 e->poleTekstowe.lewo=10;
 e->poleTekstowe.dol=10+35+135;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[2];
 e->nieWaznosc=10;



 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_ZAPISZ_ZAPISZ;
 e->prawo=0;
 e->gora=GUI_ZAPISZ_NAZWA;
 e->lewo=0;
 e->dol=GUI_WCZE;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+180;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+180;
 e->przycisk.t=napisy[21];
 e->przycisk.funkcja=gui_fZapiszx;
 e->nieWaznosc=10;

 e=gui_daj(GUI_WCZE,0);
 if(e)
 {
  e->gora=GUI_ZAPISZ_ZAPISZ;
 }
 e=gui_daj(GUI_NAST,0);
 if(e)
 {
  e->gora=GUI_ZAPISZ_ZAPISZ;
 }
 gui_wybrane=gui_daj(GUI_ZAPISZ_POWROT,0);
}
void gui_fTworz()
{

 gui_usunMniejWazne(10);
 gui_cos *e=NULL;
 gui_escape=gui_fMenu;

 e=gui_dodaj();
 e->rodzaj=GUI_R_NAPIS;
 e->nazwa=GUI_TWORZ_NAPIS;
 e->napis.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35;
 e->napis.t=napisy[7];
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_TWORZ_POWROT;
 e->prawo=0;
 e->gora=0;
 e->lewo=0;
 e->dol=GUI_TWORZ_FRAKTAL;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+45;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+45;
 e->przycisk.t=napisy[35];
 e->przycisk.funkcja=gui_fMenu;
 e->nieWaznosc=10;




 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_TWORZ_FRAKTAL;
 e->prawo=0;
 e->gora=GUI_TWORZ_POWROT;
 e->lewo=0;
 e->dol=GUI_TWORZ_WYKRES;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+90;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+90;
 e->przycisk.t=napisy[38];
 e->przycisk.funkcja=gui_fFraktal;
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_TWORZ_WYKRES;
 e->prawo=0;
 e->gora=GUI_TWORZ_FRAKTAL;
 e->lewo=0;
 e->dol=GUI_WCZE;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+135;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+135;
 e->przycisk.t=napisy[39];
 e->przycisk.funkcja=gui_fWykres;
 e->nieWaznosc=10;

 e=gui_daj(GUI_WCZE,0);
 if(e)
 {
  e->gora=GUI_TWORZ_WYKRES;
 }
 e=gui_daj(GUI_NAST,0);
 if(e)
 {
  e->gora=GUI_TWORZ_WYKRES;
 }
 gui_wybrane=gui_daj(GUI_TWORZ_POWROT,0);
}
void gui_fZapiszx()
{
 Zlecenie zlecenie;
 gui_cos*rzecz=NULL;
 zlecenie.rodzaj=ZLEC_R_ZAPISZ;
 rzecz=gui_daj(GUI_ZAPISZ_NAZWA,0);
 if
 (
  (
   rzecz &&
   rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
   rzecz->poleTekstowe.t
  )
 )
 {
  zlecenie.str=malloc(strlen(rzecz->poleTekstowe.t));
  strcpy(zlecenie.str,rzecz->poleTekstowe.t);
 }
 else
 {
  zlecenie.str=malloc(13);
  
  zlecenie.str[0]='w';
  zlecenie.str[1]='y';
  zlecenie.str[2]='j';
  zlecenie.str[3]=0xc5;
  zlecenie.str[4]=0x9b;
  zlecenie.str[5]='c';
  zlecenie.str[6]='i';
  zlecenie.str[7]='e';
  zlecenie.str[8]='.';
  zlecenie.str[9]='b';
  zlecenie.str[10]='m';
  zlecenie.str[11]='p';
  zlecenie.str[12]='\0';
 }
 dodajZlecenie(zlecenie);

}
void gui_fFraktal()
{

 gui_usunMniejWazne(10);
 gui_cos *e=NULL;
 gui_escape=gui_fMenu;



 e=gui_dodaj();
 e->rodzaj=GUI_R_NAPIS;
 e->nazwa=GUI_FRAKTAL_NAPIS;
 e->napis.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35;
 e->napis.t=napisy[38];
 e->nieWaznosc=10;



 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_FRAKTAL_POWROT;
 e->prawo=0;
 e->gora=0;
 e->lewo=0;
 e->dol=GUI_FRAKTAL_RODZAJ;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+45;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+45;
 e->przycisk.t=napisy[35];
 e->przycisk.funkcja=gui_fTworz;
 e->nieWaznosc=10;




 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZELACZNIK;
 e->nazwa=GUI_FRAKTAL_RODZAJ;
 e->prawo=0;
 e->gora=GUI_FRAKTAL_POWROT;
 e->lewo=0;
 e->dol=GUI_FRAKTAL_KOLOR;
 e->przelacznik.prawo=SZEROKOSC_MENU-10;
 e->przelacznik.gora=10+90;
 e->przelacznik.lewo=10;
 e->przelacznik.dol=10+35+90;
 e->przelacznik.ilosc=9;
 e->przelacznik.w=gp_frakRodzaj;
 e->przelacznik.wybor=0;
 e->nieWaznosc=10;

 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZELACZNIK;
 e->nazwa=GUI_FRAKTAL_KOLOR;
 e->prawo=0;
 e->gora=GUI_FRAKTAL_RODZAJ;
 e->lewo=0;
 e->dol=GUI_FRAKTAL_RZECZYWISTA;
 e->przelacznik.prawo=SZEROKOSC_MENU-10;
 e->przelacznik.gora=10+135;
 e->przelacznik.lewo=10;
 e->przelacznik.dol=10+35+135;
 e->przelacznik.ilosc=2;
 e->przelacznik.w=gp_frakKolor;
 e->przelacznik.wybor=0;
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_FRAKTAL_RZECZYWISTA;
 e->prawo=GUI_FRAKTAL_UROJONA;
 e->gora=GUI_FRAKTAL_KOLOR;
 e->lewo=0;
 e->dol=GUI_FRAKTAL_GORA;
 e->poleTekstowe.prawo=SZEROKOSC_MENU/2-5;
 e->poleTekstowe.gora=10+180;
 e->poleTekstowe.lewo=10;
 e->poleTekstowe.dol=10+35+180;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[40];
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_FRAKTAL_UROJONA;
 e->prawo=0;
 e->gora=GUI_FRAKTAL_KOLOR;
 e->lewo=GUI_FRAKTAL_RZECZYWISTA;
 e->dol=GUI_FRAKTAL_GORA;
 e->poleTekstowe.prawo=SZEROKOSC_MENU-10;
 e->poleTekstowe.gora=10+180;
 e->poleTekstowe.lewo=SZEROKOSC_MENU/2+5;
 e->poleTekstowe.dol=10+35+180;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[41];
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_FRAKTAL_GORA;
 e->prawo=0;
 e->gora=GUI_FRAKTAL_RZECZYWISTA;
 e->lewo=0;
 e->dol=GUI_FRAKTAL_LEWO;
 e->poleTekstowe.prawo=SZEROKOSC_MENU-10;
 e->poleTekstowe.gora=10+225;
 e->poleTekstowe.lewo=10;
 e->poleTekstowe.dol=10+35+225;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[18];
 e->nieWaznosc=10;



 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_FRAKTAL_LEWO;
 e->prawo=GUI_FRAKTAL_PRAWO;
 e->gora=GUI_FRAKTAL_GORA;
 e->lewo=0;
 e->dol=GUI_FRAKTAL_DOL;
 e->poleTekstowe.prawo=SZEROKOSC_MENU/2-5;
 e->poleTekstowe.gora=10+270;
 e->poleTekstowe.lewo=10;
 e->poleTekstowe.dol=10+35+270;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[17];
 e->nieWaznosc=10;

 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_FRAKTAL_PRAWO;
 e->prawo=0;
 e->gora=GUI_FRAKTAL_GORA;
 e->lewo=GUI_FRAKTAL_LEWO;
 e->dol=GUI_FRAKTAL_DOL;
 e->poleTekstowe.prawo=SZEROKOSC_MENU-10;
 e->poleTekstowe.gora=10+270;
 e->poleTekstowe.lewo=SZEROKOSC_MENU/2+5;
 e->poleTekstowe.dol=10+35+270;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[16];
 e->nieWaznosc=10;

 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_FRAKTAL_DOL;
 e->prawo=0;
 e->gora=GUI_FRAKTAL_LEWO;
 e->lewo=0;
 e->dol=GUI_FRAKTAL_ILOSC;
 e->poleTekstowe.prawo=SZEROKOSC_MENU-10;
 e->poleTekstowe.gora=10+315;
 e->poleTekstowe.lewo=10;
 e->poleTekstowe.dol=10+35+315;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[19];
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_FRAKTAL_ILOSC;
 e->prawo=0;
 e->gora=GUI_FRAKTAL_DOL;
 e->lewo=0;
 e->dol=GUI_FRAKTAL_RYSUJ;
 e->poleTekstowe.prawo=SZEROKOSC_MENU-10;
 e->poleTekstowe.gora=10+360;
 e->poleTekstowe.lewo=10;
 e->poleTekstowe.dol=10+35+360;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[42];
 e->nieWaznosc=10;




 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_FRAKTAL_RYSUJ;
 e->prawo=0;
 e->gora=GUI_FRAKTAL_ILOSC;
 e->lewo=0;
 e->dol=GUI_WCZE;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+405;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+405;
 e->przycisk.t=napisy[43];
 e->przycisk.funkcja=gui_fFraktalx;
 e->nieWaznosc=10;



 e=gui_daj(GUI_WCZE,0);
 if(e)
 {
  e->gora=GUI_FRAKTAL_RYSUJ;
 }
 e=gui_daj(GUI_NAST,0);
 if(e)
 {
  e->gora=GUI_FRAKTAL_RYSUJ;
 }
 gui_wybrane=gui_daj(GUI_FRAKTAL_POWROT,0);
}
void gui_fFraktalx()
{
 double x=0.0;
 Zlecenie zlecenie;
 zlecenie.rodzaj=ZLEC_R_FRAK;
 gui_cos *rzecz=NULL;
 rzecz=gui_daj(GUI_FRAKTAL_RODZAJ,0);
 if(rzecz&&rzecz->rodzaj==GUI_R_PRZELACZNIK)
  zlecenie.jaki=rzecz->przelacznik.w[rzecz->przelacznik.wybor].wartosc;
 else
  zlecenie.jaki=ZBIOR_J;
 rzecz=gui_daj(GUI_FRAKTAL_KOLOR,0);
 if(rzecz&&rzecz->rodzaj==GUI_R_PRZELACZNIK)
  zlecenie.kolory=rzecz->przelacznik.w[rzecz->przelacznik.wybor].wartosc;
 else
  zlecenie.kolory=KOLOR_F_PODSTAWOWE;
 rzecz=gui_daj(GUI_FRAKTAL_PRAWO,0);
 if
 (
  !
  (
   rzecz &&
   rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
   rzecz->poleTekstowe.t &&
   sscanf(rzecz->poleTekstowe.t,"%lf",&zlecenie.prawo)>0
  )
 )
  zlecenie.prawo=2.0;
 rzecz=gui_daj(GUI_FRAKTAL_GORA,0);
 if
 (
  !
  (
   rzecz &&
   rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
   rzecz->poleTekstowe.t &&
   sscanf(rzecz->poleTekstowe.t,"%lf",&zlecenie.gora)>0
  )
 )
  zlecenie.gora=2.0;
 rzecz=gui_daj(GUI_FRAKTAL_LEWO,0);
 if
 (
  !
  (
   rzecz &&
   rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
   rzecz->poleTekstowe.t &&
   sscanf(rzecz->poleTekstowe.t,"%lf",&zlecenie.lewo)>0
  )
 )
  zlecenie.lewo=-2.0;
 rzecz=gui_daj(GUI_FRAKTAL_DOL,0);
 if
 (
  !
  (
   rzecz &&
   rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
   rzecz->poleTekstowe.t &&
   sscanf(rzecz->poleTekstowe.t,"%lf",&zlecenie.dol)>0
  )
 )
  zlecenie.dol=-2.0;
 rzecz=gui_daj(GUI_FRAKTAL_ILOSC,0);
 if
 (
  !
  (
   rzecz &&
   rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
   rzecz->poleTekstowe.t &&
   sscanf(rzecz->poleTekstowe.t,"%" PRIu32 ,&zlecenie.iloscPowtorzen)>0
  )
 )
  zlecenie.iloscPowtorzen=1;
 rzecz=gui_daj(GUI_FRAKTAL_RZECZYWISTA,0);
 if
 (
  !
  (
   rzecz &&
   rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
   rzecz->poleTekstowe.t &&
   sscanf(rzecz->poleTekstowe.t,"%lf",&x)>0
  )
 )
  x=0.0;
 zlecenie.e=x;
 rzecz=gui_daj(GUI_FRAKTAL_UROJONA,0);
 if
 (
  !
  (
   rzecz &&
   rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
   rzecz->poleTekstowe.t &&
   sscanf(rzecz->poleTekstowe.t,"%lf",&x)>0
  )
 )
  x=0.0;
 zlecenie.e+=I*x;


 
 dodajZlecenie(zlecenie);

}
void gui_fWykres()
{

 gui_usunMniejWazne(10);
 gui_cos *e=NULL;
 gui_escape=gui_fMenu;




 e=gui_dodaj();
 e->rodzaj=GUI_R_NAPIS;
 e->nazwa=GUI_WYKRES_NAPIS;
 e->napis.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35;
 e->napis.t=napisy[39];
 e->nieWaznosc=10;




 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_WYKRES_POWROT;
 e->prawo=0;
 e->gora=0;
 e->lewo=0;
 e->dol=GUI_WYKRES_KOLOR;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+45;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+45;
 e->przycisk.t=napisy[35];
 e->przycisk.funkcja=gui_fTworz;
 e->nieWaznosc=10;



 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZELACZNIK;
 e->nazwa=GUI_WYKRES_KOLOR;
 e->prawo=0;
 e->gora=GUI_WYKRES_POWROT;
 e->lewo=0;
 e->dol=GUI_WYKRES_LICZNIK;
 e->przelacznik.prawo=SZEROKOSC_MENU-10;
 e->przelacznik.gora=10+90;
 e->przelacznik.lewo=10;
 e->przelacznik.dol=10+35+90;
 e->przelacznik.ilosc=2;
 e->przelacznik.w=gp_wykresKolor;
 e->przelacznik.wybor=0;
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_WYKRES_LICZNIK;
 e->prawo=0;
 e->gora=GUI_WYKRES_KOLOR;
 e->lewo=0;
 e->dol=GUI_WYKRES_MIANOWNIK;
 e->poleTekstowe.prawo=SZEROKOSC_MENU-10;
 e->poleTekstowe.gora=10+135;
 e->poleTekstowe.lewo=10;
 e->poleTekstowe.dol=10+35+135;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[44];
 e->nieWaznosc=10;

 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_WYKRES_MIANOWNIK;
 e->prawo=0;
 e->gora=GUI_WYKRES_LICZNIK;
 e->lewo=0;
 e->dol=GUI_WYKRES_GORA;
 e->poleTekstowe.prawo=SZEROKOSC_MENU-10;
 e->poleTekstowe.gora=10+180;
 e->poleTekstowe.lewo=10;
 e->poleTekstowe.dol=10+35+180;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[45];
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_WYKRES_GORA;
 e->prawo=0;
 e->gora=GUI_WYKRES_MIANOWNIK;
 e->lewo=0;
 e->dol=GUI_WYKRES_LEWO;
 e->poleTekstowe.prawo=SZEROKOSC_MENU-10;
 e->poleTekstowe.gora=10+225;
 e->poleTekstowe.lewo=10;
 e->poleTekstowe.dol=10+35+225;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[18];
 e->nieWaznosc=10;



 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_WYKRES_LEWO;
 e->prawo=GUI_WYKRES_PRAWO;
 e->gora=GUI_WYKRES_GORA;
 e->lewo=0;
 e->dol=GUI_WYKRES_DOL;
 e->poleTekstowe.prawo=SZEROKOSC_MENU/2-5;
 e->poleTekstowe.gora=10+270;
 e->poleTekstowe.lewo=10;
 e->poleTekstowe.dol=10+35+270;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[17];
 e->nieWaznosc=10;

 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_WYKRES_PRAWO;
 e->prawo=0;
 e->gora=GUI_WYKRES_GORA;
 e->lewo=GUI_WYKRES_LEWO;
 e->dol=GUI_WYKRES_DOL;
 e->poleTekstowe.prawo=SZEROKOSC_MENU-10;
 e->poleTekstowe.gora=10+270;
 e->poleTekstowe.lewo=SZEROKOSC_MENU/2+5;
 e->poleTekstowe.dol=10+35+270;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[16];
 e->nieWaznosc=10;

 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_WYKRES_DOL;
 e->prawo=0;
 e->gora=GUI_WYKRES_LEWO;
 e->lewo=0;
 e->dol=GUI_WYKRES_CZYNNIKRE;
 e->poleTekstowe.prawo=SZEROKOSC_MENU-10;
 e->poleTekstowe.gora=10+315;
 e->poleTekstowe.lewo=10;
 e->poleTekstowe.dol=10+35+315;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[19];
 e->nieWaznosc=10;


 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_WYKRES_CZYNNIKRE;
 e->prawo=GUI_WYKRES_CZYNNIKIM;
 e->gora=GUI_WYKRES_DOL;
 e->lewo=0;
 e->dol=GUI_WYKRES_RYSUJ;
 e->poleTekstowe.prawo=SZEROKOSC_MENU/2-5;
 e->poleTekstowe.gora=10+360;
 e->poleTekstowe.lewo=10;
 e->poleTekstowe.dol=10+35+360;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[46];
 e->nieWaznosc=10;

 e=gui_dodaj();
 e->rodzaj=GUI_R_POLE_TEKSTOWE;
 e->nazwa=GUI_WYKRES_CZYNNIKIM;
 e->prawo=0;
 e->gora=GUI_WYKRES_DOL;
 e->lewo=GUI_WYKRES_CZYNNIKRE;
 e->dol=GUI_WYKRES_RYSUJ;
 e->poleTekstowe.prawo=SZEROKOSC_MENU-10;
 e->poleTekstowe.gora=10+360;
 e->poleTekstowe.lewo=SZEROKOSC_MENU/2+5;
 e->poleTekstowe.dol=10+35+360;
 e->poleTekstowe.t=NULL;
 e->poleTekstowe.tt=napisy[47];
 e->nieWaznosc=10;





 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_WYKRES_RYSUJ;
 e->prawo=0;
 e->gora=GUI_WYKRES_CZYNNIKRE;
 e->lewo=0;
 e->dol=GUI_WCZE;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+405;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+405;
 e->przycisk.t=napisy[43];
 e->przycisk.funkcja=gui_fWykresx;
 e->nieWaznosc=10;



 e=gui_daj(GUI_WCZE,0);
 if(e)
 {
  e->gora=GUI_WYKRES_RYSUJ;
 }
 e=gui_daj(GUI_NAST,0);
 if(e)
 {
  e->gora=GUI_WYKRES_RYSUJ;
 }
 gui_wybrane=gui_daj(GUI_WYKRES_POWROT,0);
 
}
void gui_fWykresx()
{
 double x=0.0,v=0.0;
 double complex y=0.0;
 int czy=0;
 uint32_t kreskaUlamkowa=0;
 Zlecenie zlecenie;
 zlecenie.rodzaj=ZLEC_R_WYKRES;
 tablicaDubli *liczby;
 liczby=malloc(sizeof(tablicaDubli));
 gui_cos *rzecz=NULL,*rzecz1=NULL;
 rzecz=gui_daj(GUI_WYKRES_KOLOR,0);
 tablicaDubliCtor(liczby);
 if(rzecz&&rzecz->rodzaj==GUI_R_PRZELACZNIK)
  zlecenie.kolory=rzecz->przelacznik.w[rzecz->przelacznik.wybor].wartosc;
 else
  zlecenie.kolory=KOLOR_W_PODSTAWOWE;
 rzecz=gui_daj(GUI_WYKRES_PRAWO,0);
 if
 (
  !
  (
   rzecz &&
   rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
   rzecz->poleTekstowe.t &&
   sscanf(rzecz->poleTekstowe.t,"%lf",&zlecenie.prawo)>0
  )
 )
  zlecenie.prawo=2.0;
 rzecz=gui_daj(GUI_WYKRES_GORA,0);
 if
 (
  !
  (
   rzecz &&
   rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
   rzecz->poleTekstowe.t &&
   sscanf(rzecz->poleTekstowe.t,"%lf",&zlecenie.gora)>0
  )
 )
  zlecenie.gora=2.0;
 rzecz=gui_daj(GUI_WYKRES_LEWO,0);
 if
 (
  !
  (
   rzecz &&
   rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
   rzecz->poleTekstowe.t &&
   sscanf(rzecz->poleTekstowe.t,"%lf",&zlecenie.lewo)>0
  )
 )
  zlecenie.lewo=-2.0;
 rzecz=gui_daj(GUI_WYKRES_DOL,0);
 if
 (
  !
  (
   rzecz &&
   rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
   rzecz->poleTekstowe.t &&
   sscanf(rzecz->poleTekstowe.t,"%lf",&zlecenie.dol)>0
  )
 )
  zlecenie.dol=-2.0;
 x=0.0;
 v=0.0;
 if
 (
  (rzecz=gui_daj(GUI_WYKRES_CZYNNIKRE,0)) &&
  rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
  rzecz->poleTekstowe.t &&
  sscanf(rzecz->poleTekstowe.t,"%lf",&x)>0
 )
  czy|=1;
 if
 (
  (rzecz=gui_daj(GUI_WYKRES_CZYNNIKIM,0)) &&
  rzecz->rodzaj==GUI_R_POLE_TEKSTOWE &&
  rzecz->poleTekstowe.t &&
  sscanf(rzecz->poleTekstowe.t,"%lf",&v)>0
 )
 czy|=2;
 if(czy)
  zlecenie.czynnik=x+I*v;
 else
  zlecenie.czynnik=1.0;
 rzecz=gui_daj(GUI_WYKRES_LICZNIK,0);
 rzecz1=gui_daj(GUI_WYKRES_MIANOWNIK,0);
 if(rzecz&&rzecz->poleTekstowe.t)
 {
  char*w=rzecz->poleTekstowe.t;
  int przesuniecie=0;
  int c=0;
  while(1)
  {
   c=sscanf(w,"%lf%n",&x,&przesuniecie);
   if(przesuniecie<=0||c<=0) break;
   w+=przesuniecie;
   y=x;
   c=sscanf(w,"%lf%n",&x,&przesuniecie);
   printf("%d\n",przesuniecie);
   if(przesuniecie>0&&c>0)
   {
    w+=przesuniecie;
    y+=I*x;
   }
   tDDodaj(liczby,y);
   ++kreskaUlamkowa;
  }
 }
 if(rzecz1&&rzecz1->poleTekstowe.t)
 {
  char*w=rzecz1->poleTekstowe.t;
  int przesuniecie=0;
  int c=0;
  while(1)
  {
   c=sscanf(w,"%lf%n",&x,&przesuniecie);
   if(przesuniecie<=0||c<=0) break;
   w+=przesuniecie;
   //while(isspace(*w)) ++w;
   y=x;
   c=sscanf(w,"%lf%n",&x,&przesuniecie);
   if(przesuniecie>0&&c>0)
   {
    w+=przesuniecie;
    y+=I*x;
   }
   tDDodaj(liczby,y);
  }
 }
 zlecenie.td=liczby;
 zlecenie.kreskaUlamkowa=kreskaUlamkowa;
 dodajZlecenie(zlecenie);
}
void gui_fCostam()
{

 gui_usunMniejWazne(10);
 gui_cos *e=NULL;
 gui_escape=gui_fMenu;





 e=gui_dodaj();
 e->rodzaj=GUI_R_NAPIS;
 e->nazwa=GUI_FRAKTAL_NAPIS;
 e->napis.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35;
 e->napis.t=napisy[8];
 e->nieWaznosc=10;




 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_COSTAM_POWROT;
 e->prawo=0;
 e->gora=0;
 e->lewo=0;
 e->dol=GUI_COSTAM_ROZMYCIE;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+45;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+45;
 e->przycisk.t=napisy[35];
 e->przycisk.funkcja=gui_fMenu;
 e->nieWaznosc=10;



 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_COSTAM_ROZMYCIE;
 e->prawo=0;
 e->gora=GUI_COSTAM_POWROT;
 e->lewo=0;
 e->dol=GUI_COSTAM_WYOSTRZENIE;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+90;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+90;
 e->przycisk.t=napisy[33];
 e->przycisk.funkcja=gui_fRozmycie;
 e->nieWaznosc=10;

 e=gui_dodaj();
 e->rodzaj=GUI_R_PRZYCISK;
 e->nazwa=GUI_COSTAM_WYOSTRZENIE;
 e->prawo=0;
 e->gora=GUI_COSTAM_ROZMYCIE;
 e->lewo=0;
 e->dol=GUI_WCZE;
 e->przycisk.prawo=SZEROKOSC_MENU-10;
 e->przycisk.gora=10+135;
 e->przycisk.lewo=10;
 e->przycisk.dol=10+35+135;
 e->przycisk.t=napisy[34];
 e->przycisk.funkcja=gui_fWyostrzenie;
 e->nieWaznosc=10;



 e=gui_daj(GUI_WCZE,0);
 if(e)
 {
  e->gora=GUI_COSTAM_WYOSTRZENIE;
 }
 e=gui_daj(GUI_NAST,0);
 if(e)
 {
  e->gora=GUI_COSTAM_WYOSTRZENIE;
 }
 gui_wybrane=gui_daj(GUI_COSTAM_POWROT,0);
}
void gui_fRozmycie()
{
 Zlecenie zlecenie;
 zlecenie.rodzaj=ZLEC_R_ROZMYCIE;
 dodajZlecenie(zlecenie);
}
void gui_fWyostrzenie()
{
 Zlecenie zlecenie;
 zlecenie.rodzaj=ZLEC_R_WYOSTRZENIE;
 dodajZlecenie(zlecenie);
}