



#include"inne.h"

uint8_t rozmycie(Obraz*obraz)
{
 if(obraz==NULL) return B_PUSTY_WSKAZNIK;
 pthread_mutex_lock(&(obraz->muteks));
 if(obraz->piksele==NULL)
 {
  pthread_mutex_unlock(&(obraz->muteks));
  return B_BRAK_OBRAZU;
 }
 uint32_t szerokosc=0;
 uint32_t wysokosc=0;
 szerokosc=obraz->szerokosc;
 wysokosc=obraz->wysokosc;
 Czworka *piksele=malloc(sizeof(Czworka)*szerokosc*wysokosc);
 if(piksele==NULL)
 {
  pthread_mutex_unlock(&(obraz->muteks));
  return B_BRAK_MIEJSCA_PAM;
 }
 int32_t x=0,y=0,i,ax,ay;
 int32_t r,g,b,a;
 while(1)
 {
  //printf("x: %" PRIu32 " y: %" PRIu32 "\n",x,y);
  i=0;
  r=0;
  g=0;
  b=0;
  a=0;
  ax=x-1;
  ay=y-1;
  while(1)
  {
   if(ay<0)
   {
    ++ay;
    continue;
   }
   if(ax<0)
   {
    ++ax;
    continue;
   }
   if(ax>x+1||ax>=szerokosc)
   {
    ax=x-1;
    ++ay;
    continue;
   }
   if(ay>y+1||ay>=wysokosc)
    break;
   ++i;
   r+=obraz->piksele[ax+szerokosc*ay].e[0];
   g+=obraz->piksele[ax+szerokosc*ay].e[1];
   b+=obraz->piksele[ax+szerokosc*ay].e[2];
   a+=obraz->piksele[ax+szerokosc*ay].e[3];
   ++ax;
  }
  r/=i;
  g/=i;
  b/=i;
  a/=i;
  piksele[x+szerokosc*y].e[0]=r;
  piksele[x+szerokosc*y].e[1]=g;
  piksele[x+szerokosc*y].e[2]=b;
  piksele[x+szerokosc*y].e[3]=a;
  ++x;
  if(x>=szerokosc)
  {
   x=0;
   ++y;
  }
  if(y>=wysokosc)
  {
   break;
  }
 }
 free(obraz->piksele);
 obraz->piksele=piksele;
 obraz->czyZmieniono=1;
 pthread_mutex_unlock(&(obraz->muteks));
 return B_DOBRZE;
}
uint8_t wyostrzenie(Obraz*obraz)
{
 if(obraz==NULL) return B_PUSTY_WSKAZNIK;
 pthread_mutex_lock(&(obraz->muteks));
 if(obraz->piksele==NULL)
 {
  pthread_mutex_unlock(&(obraz->muteks));
  return B_BRAK_OBRAZU;
 }
 uint32_t szerokosc=0;
 uint32_t wysokosc=0;
 szerokosc=obraz->szerokosc;
 wysokosc=obraz->wysokosc;
 Czworka *piksele=malloc(sizeof(Czworka)*szerokosc*wysokosc);
 if(piksele==NULL)
 {
  pthread_mutex_unlock(&(obraz->muteks));
  return B_BRAK_MIEJSCA_PAM;
 }
 int32_t x=0,y=0,i,ax,ay;
 int32_t r,g,b,a;
 while(1)
 {
  //printf("x: %" PRIu32 " y: %" PRIu32 "\n",x,y);
  i=0;
  r=0;
  g=0;
  b=0;
  a=0;
  ax=x-1;
  ay=y-1;
  while(1)
  {
   if(ay<0)
   {
    ++ay;
    continue;
   }
   if(ax<0)
   {
    ++ax;
    continue;
   }
   if(ax>x+1||ax>=szerokosc)
   {
    ax=x-1;
    ++ay;
    continue;
   }
   if(ay>y+1||ay>=wysokosc)
    break;
   ++i;
   r+=obraz->piksele[ax+szerokosc*ay].e[0];
   g+=obraz->piksele[ax+szerokosc*ay].e[1];
   b+=obraz->piksele[ax+szerokosc*ay].e[2];
   a+=obraz->piksele[ax+szerokosc*ay].e[3];
   ++ax;
  }
  r/=i;
  r-=obraz->piksele[x+szerokosc*y].e[0];
  r=obraz->piksele[x+szerokosc*y].e[0]-r;
  r=r<0?0:(r>255?255:r);
  g/=i;
  g-=obraz->piksele[x+szerokosc*y].e[1];
  g=obraz->piksele[x+szerokosc*y].e[1]-g;
  g=g<0?0:(g>255?255:g);
  b/=i;
  b-=obraz->piksele[x+szerokosc*y].e[2];
  b=obraz->piksele[x+szerokosc*y].e[2]-b;
  b=b<0?0:(b>255?255:b);
  a/=i;
  a-=obraz->piksele[x+szerokosc*y].e[3];
  a=obraz->piksele[x+szerokosc*y].e[3]-a;
  a=a<0?0:(a>255?255:a);
  piksele[x+szerokosc*y].e[0]=r;
  piksele[x+szerokosc*y].e[1]=g;
  piksele[x+szerokosc*y].e[2]=b;
  piksele[x+szerokosc*y].e[3]=a;
  ++x;
  if(x>=szerokosc)
  {
   x=0;
   ++y;
  }
  if(y>=wysokosc)
  {
   break;
  }
 }
 free(obraz->piksele);
 obraz->piksele=piksele;
 obraz->czyZmieniono=1;
 pthread_mutex_unlock(&(obraz->muteks));
 return B_DOBRZE;
}