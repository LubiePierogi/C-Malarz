
#include"doubel.h"






uint8_t tablicaDubliCtor(tablicaDubli*a)
{
  //Tej funkcji trzeba użyć po zrobieniu tego strukta,
  //a później już nie, bo mogą być wycieki pamięci!!!
 a->dlugosc=0;
 a->miejsce=0;
 a->w=NULL;
 return B_DOBRZE;
}
uint8_t tDDodaj(tablicaDubli*a,double complex x)
{
 if(!a) return B_PUSTY_WSK_TD;
 if(!(a->w))
 {
  a->w=malloc(ILOSCI_MIEJSCA_DLA_TD*sizeof(*a->w));
  if(!a) return B_BRAK_MIEJSCA_PAM;
  a->miejsce=ILOSCI_MIEJSCA_DLA_TD;
  a->dlugosc=0;
 }
 else if(a->miejsce==a->dlugosc)
 {
  double complex*wsk=NULL;
  wsk=realloc(a->w,(a->miejsce+ILOSCI_MIEJSCA_DLA_TD)*sizeof(*a->w));
  if(!wsk) return B_BRAK_MIEJSCA_PAM;
  a->miejsce=a->miejsce+ILOSCI_MIEJSCA_DLA_TD;
  a->w=wsk;
 }
 a->w[a->dlugosc]=x;
 ++a->dlugosc;
 return B_DOBRZE;
}
uint8_t tDUsunKonca(tablicaDubli*a)
{
 if(!a) return B_PUSTY_WSK_TD;
 if(!(a->w)||!a->dlugosc) return B_BRAK_TD;
 --a->dlugosc;
 return B_DOBRZE;
}
uint8_t tDUsunKtorys(tablicaDubli*a,size_t ktory)
{
 if(!a) return B_PUSTY_WSK_TD;
 if(!(a->w)) return B_BRAK_TD;
 if(ktory>=a->dlugosc) return B_POZA_ZAKRESEM;
 --a->dlugosc;
 while(ktory<a->dlugosc)
 {
  a->w[ktory]=a->w[ktory+1];
  ++ktory;
 }
 return B_DOBRZE;
}
uint8_t tDWez(tablicaDubli*a,size_t ktory,double complex*x)
{
 if(!a) return B_PUSTY_WSK_TD;
 if(!(a->w)) return B_BRAK_TD;
 if(ktory>=a->dlugosc) return B_POZA_ZAKRESEM;
 *x=a->w[ktory];
 return B_DOBRZE;
}
uint8_t tDCzysc(tablicaDubli*a)
{
 if(!a) return B_PUSTY_WSK_TD;
 a->dlugosc=0;
 a->miejsce=0;
 if(a->w)
 {
  free(a->w);
  a->w=NULL;
  return B_DOBRZE;
 }
 return B_BRAK_TD;
}
size_t tDDlugosc(tablicaDubli*a)
{
 return a->dlugosc;
}
size_t tDMiejsce(tablicaDubli*a)
{
 return a->miejsce;
}