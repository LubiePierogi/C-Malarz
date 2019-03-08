



#include"a.h"


//kolory:
// alfa      0
// czerowy   1
// zielony   2
// niebieski 3



char *napisy[]=
{
 "Malarz", //0
 "Wyjście", //1
 "Nazwa", //2
 "Nowy obraz", //3
 "MALARZ", //4
 "Malarz",// 5
 "Zapisz obraz",// 6
 "Narysuj wzór",// 7
 "Efekty",// 8
 "Kopiuj obraz",// 9
 "Usuń obraz",// 10
 "Następny",// 11
 "Wcześniejszy",// 12
 "Utwórz",// 13
 "Szerokość",// 14
 "Wysokość",// 15
 "Prawy brzeg",// 16
 "Lewy brzeg",// 17
 "Górny brzeg",// 18
 "Dolny brzeg",// 19
 "Wymiary",// 20
 "Zapisz",// 21
 "Zielone",// 22
 "Czarno białe",// 23
 "Zbiór Julii",// 24
 "Zbiór Mandelbrota",// 25
 "Jakiś tam zbiór",// 26
 "Płonący statek",// 27
 "Płonący statek, tylko inny",// 28
 "Jakiś wzorek", //29
 "Fajny zbiór 1",// 30
 "Fajny zbiór 2",// 31
 "Fajny zbiór 3",// 32
 "Rozmycie",//33
 "Wyostrzenie",//34
 "Powrót",//35
 "Kolorowe",//36
 "Inne 1",//37
 "Fraktal",//38,
 "Wykres funkcji wymiernej",//39
 "Część rzeczywista",//40
 "Część urojona",//41
 "Ilość powtórzeń",//42
 "Rysuj",//43
 "Pierwiastki wielomianu w liczniku",//44
 "Pierwiastki wielomianu w mianowniku",//45
 "Czynnik (re)",//46
 "Czynnik (im)"//47
};


//////////////////////////////////////////////////////////////////////////////





uint8_t dzialanieOkna=0;
uint8_t czyWyjscie=0;





//////////////////////////////////////////////////////////////////////////////



uint8_t bmp[dlugoscNaglowkaBMP]=
{
 //nagłówek
 0x42,
 0x4D,
 //rozmiar pliku
 0x00,
 0x00,
 0x00,
 0x00,
 //nie wiem
 0x00,
 0x00,
 //też nie wiem
 0x00,
 0x00,
 //początek obrazka, jeszcze nie wiem
 0x00,
 0x00,
 0x00,
 0x00
};


uint8_t zapiszPlik(Obraz*obraz,char*nazwa)
{
 FILE*plik=NULL;
 Czworka cztery={{0,0,0,0}};
 size_t i=0;
 uint32_t rozmiar=0;
 if(!obraz) return B_PUSTY_WSK_OBRAZU;
 if(!nazwa) return B_PUSTY_WSK_NAPISU;//////////////////
 if(!obraz->piksele) return B_BRAK_OBRAZU;
 rozmiar=obraz->szerokosc*obraz->wysokosc;
 plik=fopen(nazwa,"wb");
 if(!plik) return B_NIE_OTWARTO_PLIKU;
 fwrite(bmp,1,2,plik);

 cztery=intLittleEnd(4*rozmiar+dlugoscNaglowkaBMP+dlugoscNaglowkaDIB);
 fwrite(&cztery,4,1,plik);

 fwrite(bmp+6,1,4,plik);

 cztery=intLittleEnd(dlugoscNaglowka);
 fwrite(&cztery,4,1,plik);

 cztery=intLittleEnd(dlugoscNaglowkaDIB);
 fwrite(&cztery,4,1,plik);

 cztery=intLittleEnd(obraz->szerokosc);
 fwrite(&cztery,4,1,plik);

 cztery=intLittleEnd(obraz->wysokosc);
 fwrite(&cztery,4,1,plik);

 cztery.e[0]=0x01;
 cztery.e[1]=0x00;
 cztery.e[2]=0x20;
 cztery.e[3]=0x00;
 fwrite(&cztery,4,1,plik);

 cztery.e[0]=0x03;
 //cztery.e[1]=0x00; //niepotzrebne
 cztery.e[2]=0x00;
 //cztery.e[3]=0x00; //niepotrzebne
 fwrite(&cztery,4,1,plik);

 cztery=intLittleEnd(4*rozmiar);
 fwrite(&cztery,4,1,plik);

 cztery=intLittleEnd(DPI72);
 fwrite(&cztery,4,1,plik);
 fwrite(&cztery,4,1,plik);

 cztery.e[0]=0x00;
 cztery.e[1]=0x00;
 cztery.e[2]=0x00;
 cztery.e[3]=0x00;
 fwrite(&cztery,4,1,plik);
 fwrite(&cztery,4,1,plik);

 cztery.e[0]=0xFF;
 //cztery.e[1]=0x00; //niepotzrebne
 //cztery.e[2]=0x00; //niepotzrebne
 //cztery.e[3]=0x00; //niepotzrebne
 fwrite(&cztery,4,1,plik);

 cztery.e[0]=0x00;
 cztery.e[1]=0xFF;
 //cztery.e[2]=0x00; //niepotzrebne
 //cztery.e[3]=0x00; //niepotzrebne
 fwrite(&cztery,4,1,plik);

 //cztery.e[0]=0x00; //niepotzrebne
 cztery.e[1]=0x00;
 cztery.e[2]=0xFF;
 //cztery.e[3]=0x00; //niepotzrebne
 fwrite(&cztery,4,1,plik);

 //cztery.e[0]=0x00; //niepotzrebne
 //cztery.e[1]=0x00; //niepotzrebne
 cztery.e[2]=0x00;
 cztery.e[3]=0xFF;
 fwrite(&cztery,4,1,plik);

 cztery.e[0]=' ';
 cztery.e[1]='n';
 cztery.e[2]='i';
 cztery.e[3]='W';
 fwrite(&cztery,4,1,plik);

 cztery.e[0]=0x00;
 cztery.e[1]=0x00;
 cztery.e[2]=0x00;
 cztery.e[3]=0x00;
 fwrite(&cztery,4,1,plik);
 for(i=0;i<(11);++i) fwrite(&cztery,4,1,plik);
 for(i=0;i<rozmiar;++i)
 {
  cztery=obraz->piksele[i];
  fwrite(&cztery,4,1,plik);
  //printf("Zapisuję %lu piksel z %u\n",i,rozmiar);
 }
 fclose(plik);
 plik=NULL;
 return B_DOBRZE;
}




const Czworka zera={{0,0,0,0}};

Czworka intLittleEnd(uint32_t a)
{
 Czworka wynik={{0,0,0,0}};
 wynik.e[0]=a;
 a>>=8;
 wynik.e[1]=a;
 a>>=8;
 wynik.e[2]=a;
 a>>=8;
 wynik.e[3]=a;
 return wynik;
}
Czworka intBigEnd(uint32_t a)
{
 Czworka wynik={{0,0,0,0}};
 wynik.e[3]=a;
 a>>=8;
 wynik.e[2]=a;
 a>>=8;
 wynik.e[1]=a;
 a>>=8;
 wynik.e[0]=a;
 return wynik;
}



//Obraz Chleb;


uint8_t obrazCtor(Obraz*e)
{
 if(e==NULL)
  return B_PUSTY_WSKAZNIK;
 e->szerokosc=0;
 e->wysokosc=0;
 e->piksele=NULL;
 e->czyZmieniono=1;
 pthread_mutexattr_init(&(e->muteksAttr));
 pthread_mutex_init(&(e->muteks),&(e->muteksAttr));
 return B_DOBRZE;
}
uint8_t obrazDtor(Obraz*e)
{
 if(e==NULL)
  return B_PUSTY_WSKAZNIK;
 if(e->piksele!=NULL)
  free(e->piksele);
 pthread_mutex_destroy(&(e->muteks));
 return B_DOBRZE;
}
uint8_t ustawObraz(Obraz*e,uint32_t s,uint32_t w)
{
 if(!e) return B_PUSTY_WSK_OBRAZU;
 pthread_mutex_lock(&(e->muteks));
 if(!s||!w)
 {
  pthread_mutex_unlock(&(e->muteks));
  return B_ZLE_WYMIARY;
 }
 if(e->piksele)
 {
  pthread_mutex_unlock(&(e->muteks));
  return B_STRUKTURA_ZAJETA;
 }
 e->szerokosc=s;
 e->wysokosc=w;
 e->piksele=malloc(sizeof(*(e->piksele))*s*w);
 uint64_t chlebek=0,ciastko=s*w;
 while(chlebek<ciastko)
 {
  *(e->piksele+chlebek)=zera;
  ++chlebek;
 }
 e->czyZmieniono=1;
 pthread_mutex_unlock(&(e->muteks));
 return B_DOBRZE;
}
uint8_t kopiujObraz(Obraz*we,Obraz*wy)
{
 if(!we||!wy) return B_PUSTY_WSK_OBRAZU;
 pthread_mutex_lock(&(we->muteks));
 pthread_mutex_lock(&(wy->muteks));
 if(wy->piksele)
 {
  pthread_mutex_unlock(&(we->muteks));
  pthread_mutex_unlock(&(wy->muteks));
  return B_STRUKTURA_ZAJETA;
 }
 wy->szerokosc=we->szerokosc;
 wy->wysokosc=we->wysokosc;
 wy->piksele=malloc(sizeof(*(wy->piksele))*wy->szerokosc*wy->wysokosc);
 uint64_t chlebek=0,ciastko=wy->szerokosc*wy->wysokosc;
 while(chlebek<ciastko)
 {
  *(wy->piksele+chlebek)=*(we->piksele+chlebek);
  ++chlebek;
 }
 wy->czyZmieniono=1;
 pthread_mutex_unlock(&(we->muteks));
 pthread_mutex_unlock(&(wy->muteks));
 return B_DOBRZE;
}
uint8_t zniszczObraz(Obraz*e)
{
 if(!e) return B_PUSTY_WSK_OBRAZU;
 pthread_mutex_lock(&(e->muteks));
 e->szerokosc=0;
 e->wysokosc=0;
 e->czyZmieniono=1;
 if(!e->piksele)
 {
  pthread_mutex_unlock(&(e->muteks));
  return B_BRAK_OBRAZU;
 }
 free(e->piksele);
 e->piksele=NULL;
 pthread_mutex_unlock(&(e->muteks));
 return B_DOBRZE;
}


// kolejka z obrazkami



ListObraz*obrazyPoczatek=NULL;
ListObraz*obrazyKoniec=NULL;
ListObraz*obrazyMiejsce=NULL;
uint8_t czyInnyObraz=1;

uint8_t dodajObraz()
{
 ListObraz*w=malloc(sizeof(ListObraz));
 if(w==NULL)
 {
  return B_BRAK_MIEJSCA_PAM;
 }
 obrazCtor(&(w->obraz));
 if(obrazyPoczatek==NULL)
 {
  w->wcze=NULL;
  w->nast=NULL;
  obrazyPoczatek=w;
  obrazyKoniec=w;
  obrazyMiejsce=w;
 }
 else
 {
  w->wcze=obrazyMiejsce;
  w->nast=obrazyMiejsce->nast;
  obrazyMiejsce->nast=w;
  if(w->nast==NULL)
  {
   obrazyKoniec=w;
  }
  else
  {
   w->nast->wcze=w;
  }
  obrazyMiejsce=w;
 }
 czyInnyObraz=1;
 return B_DOBRZE;
}


uint8_t usunWszystkieObrazy()
{
 ListObraz*w,*x;
 w=obrazyPoczatek;
 while(w!=NULL)
 {
  x=w;
  w=w->nast;
  obrazDtor(&(x->obraz));
  free(x);
 }
 obrazyPoczatek=NULL;
 obrazyKoniec=NULL;
 obrazyMiejsce=NULL;
 return B_DOBRZE;
}
uint8_t usunObraz()
{
 ListObraz*w;
 w=obrazyMiejsce;
 if(w==NULL)
 {
  return B_PUSTA_LISTA;
 }
 obrazyMiejsce=obrazyMiejsce->nast;
 if(w->nast!=NULL)
 {
  w->nast->wcze=w->wcze;
 }
 else
 {
  obrazyKoniec=w->wcze;
 }
 if(w->wcze!=NULL)
 {
  w->wcze->nast=w->nast;
 }
 else
 {
  obrazyPoczatek=w->nast;
 }
 obrazDtor(&(w->obraz));
 free(w);
 if(obrazyMiejsce==NULL)
 {
  obrazyMiejsce=obrazyPoczatek;
 }
 czyInnyObraz=1;
 return B_DOBRZE;
}
void ustawPoczatek()
{
 obrazyMiejsce=obrazyPoczatek;
 czyInnyObraz=1;
}
void ustawKoniec()
{
 obrazyMiejsce=obrazyKoniec;
 czyInnyObraz=1;
}
uint8_t idzDoPrzodu()
{
 if(obrazyMiejsce==NULL)
  return B_PUSTA_LISTA;
 obrazyMiejsce=obrazyMiejsce->nast;
 if(obrazyMiejsce==NULL)
  obrazyMiejsce=obrazyPoczatek;
 czyInnyObraz=1;
 return B_DOBRZE;
}
uint8_t idzDoTylu()
{
 if(obrazyMiejsce==NULL)
  return B_PUSTA_LISTA;
 obrazyMiejsce=obrazyMiejsce->wcze;
 if(obrazyMiejsce==NULL)
  obrazyMiejsce=obrazyKoniec;
 czyInnyObraz=1;
 return B_DOBRZE;
}

uint8_t czyPoczatek()
{
 return obrazyMiejsce==obrazyPoczatek;
}
uint8_t czyKoniec()
{
 return obrazyMiejsce==obrazyKoniec;
}

uint8_t czyInny()
{
 return czyInnyObraz;
}

void wziecieObrazu()
{
 czyInnyObraz=0;
}

Obraz*dajObraz()
{
 if(obrazyMiejsce!=NULL)
 {
  return &obrazyMiejsce->obraz;
 }
 return NULL;
}

void pokaz()
{
 printf("%p\n%p\n%p\n%p\n",obrazyPoczatek,obrazyKoniec,obrazyMiejsce,dajObraz());
}

//////////////////////////////////////////////////////////////////////////////
