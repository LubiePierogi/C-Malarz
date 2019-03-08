



#include"zlecenia.h"

uint8_t czyZajety=0;

static ListaZlecen*zleceniaPoczatek=NULL;
static ListaZlecen*zleceniaKoniec=NULL;


static pthread_mutex_t funkcjaOdZlecenMuteks;
static pthread_mutexattr_t funkcjaOdZlecenMuteksAttr;

void ustawZlecenia()
{
 pthread_mutexattr_init(&(funkcjaOdZlecenMuteksAttr));
 pthread_mutex_init(&(funkcjaOdZlecenMuteks),&(funkcjaOdZlecenMuteksAttr));
}



void zwolnijZlecenia()
{
 pthread_mutex_destroy(&(funkcjaOdZlecenMuteks));
}





uint8_t dodajZlecenie(Zlecenie zlecenie)
{
 pthread_mutex_lock(&funkcjaOdZlecenMuteks);
 ListaZlecen *w=malloc(sizeof(ListaZlecen));
 if(w==NULL)
 {
  pthread_mutex_unlock(&funkcjaOdZlecenMuteks);
  return B_BRAK_MIEJSCA_PAM;
 }
 w->nast=NULL;
 w->zlecenie=zlecenie;
 if(zleceniaPoczatek==NULL)
 {
  zleceniaPoczatek=w;
 }
 else
 {
  zleceniaKoniec->nast=w;
 }
 zleceniaKoniec=w;
 czyZajety=1;
 pthread_mutex_unlock(&funkcjaOdZlecenMuteks);
 return B_DOBRZE;
}

uint8_t wezZlecenie(Zlecenie*zlecenie)
{
 pthread_mutex_lock(&funkcjaOdZlecenMuteks);
 if(zlecenie==NULL)
 {
  pthread_mutex_unlock(&funkcjaOdZlecenMuteks);
  return B_PUSTY_WSKAZNIK;
 }
 if(zleceniaPoczatek==NULL)
 {
  czyZajety=0;
  pthread_mutex_unlock(&funkcjaOdZlecenMuteks);
  return B_PUSTA_LISTA;
 }
 ListaZlecen*w=zleceniaPoczatek;
 zleceniaPoczatek=w->nast;
 *zlecenie=w->zlecenie;
 if(zleceniaPoczatek==NULL)
  zleceniaKoniec=NULL;
 free(w);
 pthread_mutex_unlock(&funkcjaOdZlecenMuteks);
 return B_DOBRZE;
}








void *funkcjaOdZlecen(void*a)
{
 uint8_t czy=1;
 uint8_t zxc=0;
 Zlecenie e;
 while(czy)
 {
  zxc=wezZlecenie(&e);
  switch(zxc)
  {
   case B_DOBRZE:
    puts("Nowe zlecenie:");
    switch(e.rodzaj)
    {
     case ZLEC_R_NIC:
      puts(" nic");
      break;
     case ZLEC_R_WYJSCIE:
      puts(" wyjście");
      usunWszystkieObrazy();
      czy=0;
      dzialanieOkna=0;
      czyWyjscie=1;
      break;
     case ZLEC_R_NOWY_OBRAZ:
      printf(" nowy obraz o wykmiarach %" PRIu32 " %" PRIu32 "\n %p\n %p\n",e.szerokosc,e.wysokosc,&e.szerokosc,&e.wysokosc);
      dodajObraz();
      ustawObraz(dajObraz(),e.szerokosc,e.wysokosc);
      //printf
      //(
      // " początek: %p\n"
      // " koniec: %p\n"
      // " teraz: %p\n"
      // ,
      // obrazyPoczatek,
      // obrazyKoniec,
      // obrazyMiejsce
      //);
      break;
     case ZLEC_R_KOPIUJ:
      {
       puts(" kopiowanie obrazu");
       Obraz*pomarancza=dajObraz();
       if(pomarancza!=NULL)
       {
        dodajObraz();
        kopiujObraz(pomarancza,dajObraz());
       }
      }
      break;
     case ZLEC_R_NASTEPNY:
      idzDoPrzodu();
      puts(" następny obraz");
      break;
     case ZLEC_R_WCZESNIEJSZY:
      idzDoTylu();
      puts(" wcześniejszy obraz");
      break;
     case ZLEC_R_POCZATEK:
      puts(" pierwszy obraz");
      ustawPoczatek();
      break;
     case ZLEC_R_KONIEC:
      ustawKoniec();
      puts(" ostatni obraz");
      break;
     case ZLEC_R_USUN:
      usunObraz();
      puts(" usunięcie obrazu");
      break;
     case ZLEC_R_E:
      puts(" wzorek");
      {
       size_t i,j;
       if(dajObraz()==NULL||dajObraz()->piksele==NULL) break;
       pthread_mutex_lock(&(dajObraz()->muteks));
       for(i=0;i<dajObraz()->wysokosc;++i)
       {
        for(j=0;j<dajObraz()->szerokosc;++j)
        {
         dajObraz()->piksele[i*dajObraz()->szerokosc+j].e[0]=((float)j)/dajObraz()->szerokosc*255;
         dajObraz()->piksele[i*dajObraz()->szerokosc+j].e[1]=((float)i)/dajObraz()->wysokosc*255;
         dajObraz()->piksele[i*dajObraz()->szerokosc+j].e[2]=255;
         dajObraz()->piksele[i*dajObraz()->szerokosc+j].e[3]=255;
        }
       }
       dajObraz()->czyZmieniono=1;
       pthread_mutex_unlock(&(dajObraz()->muteks));
      }
      break;
     case ZLEC_R_ZAPISZ:
      printf(" zapisanie obrazu %s\n",e.str);
      switch(zapiszPlik(dajObraz(),e.str))
      {
       case B_DOBRZE:
        puts(" Zapisano.");
        break;
       case B_BRAK_OBRAZU:
        puts(" Nie ma obrazu w tym miejscu!");
        break;
       case B_PUSTY_WSK_OBRAZU:
        puts(" Brak obrazów!");
        break;
       default:
        puts(" Wystąpił bardzo poważny błąd.");
      }
      free(e.str);
      break;
     case ZLEC_R_FRAK:
      puts(" fraktal");
      switch
      (
       rysujFrak
       (
        dajObraz(),e.kolory,e.jaki,e.prawo,e.gora,e.lewo,e.dol,e.iloscPowtorzen,e.e
       )
      )
      {
       case B_DOBRZE:
        puts(" Narysowano fraktal jakiś tam.");
        break;
       case B_PUSTY_WSK_OBRAZU:
        puts(" Brak obrazów!");
        break;
       case B_BRAK_OBRAZU:
        puts(" Nie ma obrazu w tym miejscu.");
        break;
       case B_ZLY_RODZAJ:
        puts(" Zły rodzaj.");
        break;
      }
      break;
     case ZLEC_R_WYKRES:
      puts(" wykres");
      switch
      (
       rysowanieWykresu
       (
        dajObraz(),e.kolory,e.prawo,e.gora,e.lewo,e.dol,e.czynnik,e.td,e.kreskaUlamkowa
       )
      )
      {
       case B_DOBRZE:
        puts(" Narysowano wykres.");
        break;
       case B_PUSTY_WSK_OBRAZU:
        puts(" Brak obrazów!");
        break;
       case B_BRAK_OBRAZU:
        puts(" Nie ma obrazu w tym miejscu.");
        break;
       case B_ZLY_RODZAJ:
        puts(" Zły rodzaj.");
        break;
      }
      tDCzysc(e.td);
      free(e.td);
      break;
     case ZLEC_R_ROZMYCIE:
      rozmycie(dajObraz());
      puts(" rozmycie");
      break;
     case ZLEC_R_WYOSTRZENIE:
      wyostrzenie(dajObraz());
      puts(" wyostrzenie");
      break;
    }
    break;
   case B_PUSTA_LISTA:
    break;
   case B_PUSTY_WSKAZNIK:
    puts("Nie mam dokąd zapisać zlecenia");
    break;
   default:
    break;
  }
 }
 return NULL;
}