
#include"wykres.h"



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
)
{
 if(!obraz) return B_PUSTY_WSK_OBRAZU;
 if(!liczby) return B_PUSTY_WSK_TD;
 pthread_mutex_lock(&(obraz->muteks));
 if(!obraz->piksele)
 {
  pthread_mutex_unlock(&(obraz->muteks));
  return B_BRAK_OBRAZU;
 }
 size_t i,j;
 double complex chleb; //zmienna pomocnicza;
 Czworka(*kolor)(double complex);
 switch(kolory) //wybór sposobu kolorowania
 {
  case KOLOR_W_PODSTAWOWE: //takie tęczowe kolorowanie dziedziny, czy jakoś tak
   kolor=lZNaRGBA;
   break;
  case KOLOR_W_JAKIES_INNE:
   kolor=wykresKolorInny;
   break;
  default:
   pthread_mutex_unlock(&(obraz->muteks));
   return B_ZLE_KOLOROWANIE;
 }
 for(i=0;i<obraz->wysokosc;++i)
 {
  for(j=0;j<obraz->szerokosc;++j)
  {
   chleb=lewo+(((double)j)/obraz->szerokosc)*(prawo-lewo)
        +dol*I+(((double)i)/obraz->wysokosc)*(gora-dol)*I;
   obraz->piksele[i*obraz->szerokosc+j]=kolor(wielomian(chleb,czynnik,liczby,kreska));
  }
 }
 obraz->czyZmieniono=1;
 pthread_mutex_unlock(&(obraz->muteks));
 return B_DOBRZE;
}


Czworka lZNaRGBA(double complex pierogi)
{
 if(isnan(creal(pierogi))||isnan(cimag(pierogi))) return zera;
 return HSVNaRGB(lZNaHSV(pierogi));
}


HSV lZNaHSV(double complex pierogi)
{
 HSV trzy={{0.0,0.0,0.0}};
 double chleb;
 chleb=carg(pierogi);
 trzy.e[0]=chleb;
 chleb=cabs(pierogi);
 chleb=log(chleb);
 chleb=chleb>=HUGE_VAL?1.0:chleb/sqrt(chleb*chleb+1);
 if(chleb<0.0)
 {
  trzy.e[1]=1.0;
  trzy.e[2]=1.0+chleb;
 }
 else
 {
  trzy.e[1]=1.0-chleb;
  trzy.e[2]=1.0;
 }
 return trzy;
}

double koutNaH(double kout)
{
 kout=fmod(kout,2*LICZBA_PI);
 if(kout<0.0)kout+=2*LICZBA_PI;
 if(kout<=LICZBA_PI/3.0||kout>=5.0/3.0*LICZBA_PI) return 1.0;
 if(kout>LICZBA_PI/3.0&&kout<2.0/3.0*LICZBA_PI) return 2.0-3.0*kout/LICZBA_PI;
 if(kout>4.0/3.0*LICZBA_PI&&kout<5.0/3.0*LICZBA_PI) return -4.0+3.0*kout/LICZBA_PI;
 return 0.0;
}

Czworka HSVNaRGB(HSV hsv)
{
 Czworka trzy={{0,0,0,255}};
 double
  czerwony=hsv.e[2],
  zielony=hsv.e[2],
  niebieski=hsv.e[2];
  czerwony*=koutNaH(hsv.e[0]);
  zielony*=koutNaH(hsv.e[0]+2.0/3.0*LICZBA_PI);
  niebieski*=koutNaH(hsv.e[0]+4.0/3.0*LICZBA_PI);
  czerwony=czerwony*hsv.e[1]+hsv.e[2]*(1.0-hsv.e[1]);
  zielony=zielony*hsv.e[1]+hsv.e[2]*(1.0-hsv.e[1]);
  niebieski=niebieski*hsv.e[1]+hsv.e[2]*(1.0-hsv.e[1]);
  trzy.e[0]=(uint8_t)(czerwony*255.0);
  trzy.e[1]=(uint8_t)(zielony*255.0);
  trzy.e[2]=(uint8_t)(niebieski*255.0);
  return trzy;
}




double complex wielomian(double complex x,double complex czynnik,tablicaDubli*liczby,uint32_t kreska)
{
 double complex t=0.0;
 size_t i=0;
 while(i<liczby->dlugosc)
 {
  if(tDWez(liczby,i,&t)) return NAN;
  t=(x-t);
  if(kreska>i) czynnik*=t;
  else czynnik/=t;
  ++i;
 }
 return czynnik;
}

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
)
{
 if(!obraz) return B_PUSTY_WSK_OBRAZU;
 pthread_mutex_lock(&(obraz->muteks));
 if(!obraz->piksele)
 {
  pthread_mutex_unlock(&(obraz->muteks));
  return B_BRAK_OBRAZU;
 }
 size_t i,j;
 double complex chleb; //zmienna pomocnicza;
 Czworka(*kolorek)(uint32_t);
 uint32_t(*funkcyja)(double complex,double complex,uint32_t);
 switch(jaki) //wybór rodzaju fraktala
 {
  case ZBIOR_J:
   funkcyja=zbiorJ;
   break;
  case ZBIOR_M:
   funkcyja=zbiorM;
   break;
  case JAKIS_INNY_ZBIOR:
   funkcyja=zbiorInny;
   break;
  case PLONOUCY_STATEK:
   funkcyja=zbiorStatek;
   break;
  case PLONOUCY_STATEK_DWA:
   funkcyja=zbiorStatekJ;
   break;
  case JAKIS_WZOREK:
   funkcyja=zbiorHehe;
   break;
  case ZBIOR_FAJNY:
   funkcyja=zbiorFajny;
   break;
  case ZBIOR_FAJNY2:
   funkcyja=zbiorFajny2;
   break;
  case ZBIOR_FAJNY3:
   funkcyja=zbiorFajny3;
   break;
  default:
   pthread_mutex_unlock(&(obraz->muteks));
   return B_ZLY_RODZAJ;
 }
 switch(kolory) //wybór sposobu kolorowania
 {
  case KOLOR_F_PODSTAWOWE: //takie tęczowe kolorowanie dziedziny, czy jakoś tak
   kolorek=zwykleKolorowanie;
   break;
  case KOLOR_F_CZARNOBIALE:
   kolorek=kolorCB;
   break;
  default:
   pthread_mutex_unlock(&(obraz->muteks));
   return B_ZLE_KOLOROWANIE;
 }
 for(i=0;i<obraz->wysokosc;++i)
 {
  for(j=0;j<obraz->szerokosc;++j)
  {
   chleb=lewo+(((double)j)/obraz->szerokosc)*(prawo-lewo)
        +dol*I+(((double)i)/obraz->wysokosc)*(gora-dol)*I;
    //przeliczenie współrzędnych z obrazu na płaszczyznę zespoloną
   obraz->piksele[i*obraz->szerokosc+j]=kolorek(funkcyja(chleb,e,iloscPowtorzen));
  }
 }
 obraz->czyZmieniono=1;
 pthread_mutex_unlock(&(obraz->muteks));
 return B_DOBRZE;
}
Czworka zwykleKolorowanie(uint32_t chlebek)
{
 Czworka wynik;
 float
  a=0.0f,
  c=0.63f,
  z=0.92f,
  n=0.1f;
 while(chlebek)
 {
  a+=(1.0f-a)*0.1f;
  c*=0.96f;
  z*=0.997f;
  n*=0.36f;
  --chlebek;
 }
 wynik.e[0]=255.0*c;
 wynik.e[1]=255.0*z;
 wynik.e[2]=255.0*n;
 wynik.e[3]=255.0*a;
 return wynik;
}
Czworka kolorCB(uint32_t chlebek)
{
 Czworka wynik;
 wynik.e[0]=chlebek&1?255:0;
 wynik.e[1]=chlebek&1?255:0;
 wynik.e[2]=chlebek&1?255:0;
 wynik.e[3]=255;
 return wynik;
}
uint32_t zbiorM(double complex x,double complex y,uint32_t powt)
{
 double complex w=y;
 uint32_t ile=0;
 while(ile<powt&&cabs(w)<2.0)
 {
  w=w*w+x;
  ++ile;
 }
 return ile; 
}
uint32_t zbiorJ(double complex x,double complex y,uint32_t powt)
{
 uint32_t ile=0;
 while(ile<powt&&cabs(x)<2.0)
 {
  x=x*x+y;
  ++ile;
 }
 return ile; 
}
uint32_t zbiorInny(double complex x,double complex y,uint32_t powt)
{
 uint32_t ile=0;
 while(ile<powt&&cabs(x)<HUGE_VAL)
 {
  x=(csinh(x*x))+y;
  ++ile;
 }
 return ile; 
}
uint32_t zbiorStatek(double complex x,double complex y,uint32_t powt)
{
 double complex w=y;
 uint32_t ile=0;
 while(ile<powt&&cabs(w)<2.0)
 {
  w=cabs(creal(w))+I*cabs(cimag(w));
  w=w*w+x;
  ++ile;
 }
 return ile; 
}
uint32_t zbiorStatekJ(double complex x,double complex y,uint32_t powt)
{
 uint32_t ile=0;
 while(ile<powt&&cabs(x)<2.0)
 {
  x=cabs(creal(x))+I*cabs(cimag(x));
  x=x*x+y;
  ++ile;
 }
 return ile; 
}
uint32_t zbiorHehe(double complex x,double complex y,uint32_t powt)
{
 double complex w=y;
 uint32_t ile=0;
 while(ile<powt&&cabs(w)<HUGE_VAL)
 {
  w=csinh(w)+x;
  ++ile;
 }
 return ile; 
}
uint32_t zbiorFajny(double complex x,double complex y,uint32_t powt)
{
 double complex w=y;
 uint32_t ile=0;
 while(ile<powt&&cabs(w)<2.0)
 {
  w=w*carg(w)+x;
  ++ile;
 }
 return ile; 
}
uint32_t zbiorFajny2(double complex x,double complex y,uint32_t powt)
{
 uint32_t ile=0;
 while(ile<powt&&cabs(x)<2.0)
 {
  x=x*x*x+y;
  ++ile;
 }
 return ile; 
}
uint32_t zbiorFajny3(double complex x,double complex y,uint32_t powt)
{
 //double complex w=y;
 uint32_t ile=0;
 while(ile<powt&&cabs(x)<HUGE_VAL)
 {
  x=csqrt(x)*csin(x)*carg(x)+y;
  ++ile;
 }
 return ile; 
}

Czworka wykresKolorInny(double complex liczba)
{
 if(isnan(creal(liczba))||isnan(cimag(liczba))) return zera;
 double r=0.0,g=0.0,b=0.0,a=0.0;
 double x=creal(liczba);
 x=atan(x);
 x*=2.0/LICZBA_PI;
 if(x>=0.0)
 {
  b=x;
 }
 else
 {
  x=-x;
  b=0.7*x;
  r=0.3*x;
 }
 x=cimag(liczba);
 x=atan(x);
 x*=2.0/LICZBA_PI;
 if(x>=0.0)
 {
  g=x;
 }
 else
 {
  x=-x;
  r+=(1.0-r)*2.0/3.0*x;
  g=1.0/3.0*x;
 }
 a=atan(cabs(liczba))*2.0/LICZBA_PI;
 Czworka wynik;
 wynik.e[0]=(uint8_t)(r*255.0);
 wynik.e[1]=(uint8_t)(g*255.0);
 wynik.e[2]=(uint8_t)(b*255.0);
 wynik.e[3]=(uint8_t)(a*255.0);
 return wynik;
}