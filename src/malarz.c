#include"malarz.h"

//kolory:
// alfa      0
// czerowy   1
// zielony   2
// niebieski 3




//////////////////////////////////////////////////////////////////////////////




static GLint uniform_mytexture=0,uniform_mvp=0;
static GLint
 atryb_wierzch=0,
 atryb_texcoord=0,
 atryb_mwierzch=0,
 atryb_kolor=0;
static GLuint
 cieniarzv=0, //vertex shader od obrazka
 cieniarzf=0, //fragment shader od obrazka
 program=0, //program od obrazka
 shadev=0, //vertex shader od menu
 shadef=0, //fragment shader od menu
 program1=0; //program od menu
static GLuint texture_id=0;
static GLuint texture_tekst=0;
static const char*vsh=
"#version 120\n"
"attribute vec2 wierzch;"
"attribute vec2 texcoord;"
"uniform mat2 mvp;"
"varying vec2 ftexcoord;"
"void main(void)"
"{"
" gl_Position=vec4(mvp*wierzch,0.0,1.0);"
" ftexcoord=texcoord;"
"}",
*fsh=
"#version 120\n"
"varying vec2 ftexcoord;"
"uniform sampler2D texture;"
"void main(void)"
"{"
" gl_FragColor=texture2D(texture,ftexcoord);"
"}",
*mvsh=
"#version 120\n"
"attribute vec2 mwierzch;"
"attribute vec3 kolor;"
"varying vec3 fkolor;"
"void main(void)"
"{"
" gl_Position=vec4(mwierzch,0.0,1.0);"
" fkolor=kolor;"
"}",
*mfsh=
"#version 120\n"
"varying vec3 fkolor;"
"void main(void)"
"{"
" gl_FragColor=vec4(fkolor,1.0);"
"}",
*nazwav="wierzch",
*nazwamv="mwierzch",
*nazwamf="kolor",
*nazwat="texcoord";
//////////////////////////////////////////////////////////////////////////////



static uint32_t szerokoscOkna=PIERWSZA_SZEROKOSC;
static uint32_t wysokoscOkna=PIERWSZA_WYSOKOSC;


static FT_Library ftlibrary;
static FT_Face ftface;




int main(int argc,char**argv)
{
 int32_t e;
 e=wczytajFreeType();
 if(e)
 {
  printf("Bład: %" PRId32 "\n",e);
  return 1;
 }
 ustawZlecenia();
 pthread_t woutekOkno;
 pthread_create(&woutekOkno,NULL,funkcjaWatkuOdOkna,NULL);
 pthread_t woutekOdZlecen;
 pthread_create(&woutekOdZlecen,NULL,funkcjaOdZlecen,NULL);
 gui_fZacznij();
 pthread_join(woutekOkno,NULL);
 pthread_join(woutekOdZlecen,NULL);
 zwolnijZlecenia();
 return 0;
}



int32_t wczytajFreeType()
{
 
 int error;
 //int glyph_index;
 error=FT_Init_FreeType(&ftlibrary);
 if(error)
 {
  return 1;
 }
 error=FT_New_Face
 (
  ftlibrary,
  "./Ubuntu-R.ttf",
  0,
  &ftface
 );
 if(error)
 {
  return 1;
 }
 error=FT_Set_Pixel_Sizes
 (
  ftface,
  0,
  16
 );
 return 0;
}



//////////////////////////////////////////////////////////////////////////////

void*funkcjaWatkuOdOkna(void*argument)
{
 SDL_Init(SDL_INIT_VIDEO);
 SDL_Window*okno=SDL_CreateWindow
 (
  napisy[0],
  SDL_WINDOWPOS_CENTERED,
  SDL_WINDOWPOS_CENTERED,
  szerokoscOkna,
  wysokoscOkna,
  SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE
 );
 SDL_GL_CreateContext(okno);
 GLenum c=glewInit();
 if(c!=GLEW_OK)
 {
  printf("Oj, wystąpił jakiś błąd: %s\n",glewGetErrorString(c));
  SDL_DestroyWindow(okno);
  return NULL;
 }
 GLint czy=GL_FALSE;
 cieniarzv=glCreateShader(GL_VERTEX_SHADER);
 printf("%d\n",cieniarzv);
 glShaderSource(cieniarzv,1,&vsh,0);
 glCompileShader(cieniarzv);
 glGetShaderiv(cieniarzv,GL_COMPILE_STATUS,&czy);
 if(czy)
  puts("Dobrze");
 else
 {
  GLint d=0;
  glGetShaderiv(cieniarzv,GL_INFO_LOG_LENGTH,&d);
  char *q=NULL;
  q=malloc(d);
  glGetShaderInfoLog(cieniarzv,d,&d,q);
  puts(q);
  free(q);
  puts("Źle!");
  SDL_DestroyWindow(okno);
  return NULL;
 }
 cieniarzf=glCreateShader(GL_FRAGMENT_SHADER);
 printf("%d\n",cieniarzf);
 glShaderSource(cieniarzf,1,&fsh,0);
 glCompileShader(cieniarzf);
 glGetShaderiv(cieniarzf,GL_COMPILE_STATUS,&czy);
 if(czy)
  puts("Dobrze");
 else
 {
  GLint d=0;
  glGetShaderiv(cieniarzf,GL_INFO_LOG_LENGTH,&d);
  char *q=NULL;
  q=malloc(d);
  glGetShaderInfoLog(cieniarzf,d,&d,q);
  puts(q);
  free(q);
  puts("Źle!");
  SDL_DestroyWindow(okno);
  return NULL;
 }
 program=glCreateProgram();
 printf("%d\n",program);
 glAttachShader(program,cieniarzv);
 glAttachShader(program,cieniarzf);
 glLinkProgram(program);
 glGetProgramiv(program,GL_LINK_STATUS,&czy);
 if(czy)
  puts("Dobrze");
 else
 {
  GLint d=0;
  glGetProgramiv(program,GL_INFO_LOG_LENGTH,&d);
  char *q=NULL;
  q=malloc(d);
  glGetProgramInfoLog(program,d,&d,q);
  puts(q);
  free(q);
  puts("Źle!");
  SDL_DestroyWindow(okno);
  return NULL;
 }
 atryb_wierzch=glGetAttribLocation(program,nazwav);
 if(atryb_wierzch==-1)
 {
  printf("Nie działa %s\n",nazwav);
  SDL_DestroyWindow(okno);
  return NULL;
 }
 atryb_texcoord=glGetAttribLocation(program,nazwat);
 if(atryb_texcoord==-1)
 {
  printf("Nie działa %s\n",nazwat);
  SDL_DestroyWindow(okno);
  return NULL;
 }
 shadev=glCreateShader(GL_VERTEX_SHADER);
 printf("%d\n",shadev);
 glShaderSource(shadev,1,&mvsh,0);
 glCompileShader(shadev);
 glGetShaderiv(shadev,GL_COMPILE_STATUS,&czy);
 if(czy)
  puts("Dobrze");
 else
 {
  GLint d=0;
  glGetShaderiv(shadev,GL_INFO_LOG_LENGTH,&d);
  char *q=NULL;
  q=malloc(d);
  glGetShaderInfoLog(shadev,d,&d,q);
  puts(q);
  free(q);
  puts("Źle!");
  SDL_DestroyWindow(okno);
  return NULL;
 }
 shadef=glCreateShader(GL_FRAGMENT_SHADER);
 printf("%d\n",shadef);
 glShaderSource(shadef,1,&mfsh,0);
 glCompileShader(shadef);
 glGetShaderiv(shadef,GL_COMPILE_STATUS,&czy);
 if(czy)
  puts("Dobrze");
 else
 {
  GLint d=0;
  glGetShaderiv(shadef,GL_INFO_LOG_LENGTH,&d);
  char *q=NULL;
  q=malloc(d);
  glGetShaderInfoLog(shadef,d,&d,q);
  puts(q);
  free(q);
  puts("Źle!");
  SDL_DestroyWindow(okno);
  return NULL;
 }
 program1=glCreateProgram();
 printf("%d\n",program1);
 glAttachShader(program1,shadev);
 glAttachShader(program1,shadef);
 glLinkProgram(program1);
 glGetProgramiv(program1,GL_LINK_STATUS,&czy);
 if(czy)
  puts("Dobrze");
 else
 {
  GLint d=0;
  glGetProgramiv(program1,GL_INFO_LOG_LENGTH,&d);
  char *q=NULL;
  q=malloc(d);
  glGetProgramInfoLog(program1,d,&d,q);
  puts(q);
  free(q);
  puts("Źle!");
  SDL_DestroyWindow(okno);
  return NULL;
 }
 atryb_mwierzch=glGetAttribLocation(program1,nazwamv);
 if(atryb_mwierzch==-1)
 {
  printf("Nie działa %s\n",nazwamv);
  SDL_DestroyWindow(okno);
  return NULL;
 }
 atryb_kolor=glGetAttribLocation(program1,nazwamf);
 if(atryb_kolor==-1)
 {
  printf("Nie działa %s\n",nazwamf);
  SDL_DestroyWindow(okno);
  return NULL;
 }




 SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,1);
 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 petla(okno);
 glDeleteProgram(program);
 glDeleteProgram(program1);
 //glDeleteTextures(1,&texture_litera);
 return NULL;
}


void petla(SDL_Window*okno)
{
 dzialanieOkna=1;
 SDL_Event ev;
 SDL_StartTextInput();
// uint8_t czyZmieniono=1;
 while(dzialanieOkna)
 {
  while(SDL_PollEvent(&ev))
  {
   if(ev.type==SDL_QUIT)
   {
    gui_fWyjscie();
   }
   else if(ev.type==SDL_WINDOWEVENT&&ev.window.event==SDL_WINDOWEVENT_SIZE_CHANGED)
   {
    szerokoscOkna=ev.window.data1;
    wysokoscOkna=ev.window.data2;
    //zmianaOkna=0x03;
   }
   else if(ev.type==SDL_MOUSEMOTION)
   {
    gui_ruchMyszy(ev.motion.x,ev.motion.y);
   }
   else if(ev.type==SDL_MOUSEBUTTONDOWN&&ev.button.button==SDL_BUTTON_LEFT)
   {
    gui_klik(ev.button.x,ev.button.y);
   }
   else if(ev.type==SDL_MOUSEBUTTONUP&&ev.button.button==SDL_BUTTON_LEFT)
   {
    gui_odklik(ev.button.x,ev.button.y);
   }
   else if(ev.type==SDL_KEYDOWN)
   {
    gui_klawiatura(ev.key.keysym.scancode);
   }
   else if(ev.type==SDL_TEXTINPUT)
   {
    gui_pisanie(ev.text.text);
   }
  }
  rysowanie(okno);
 }
 SDL_StopTextInput();
}
void rysowanieObrazka()
{
 const static float bufw[]=
 {
  -1.0,-1.0,
  1.0,-1.0,
  1.0,1.0,
  -1.0,1.0
 };
 const static float buft[]=
 {
  0.0,0.0,
  1.0,0.0,
  1.0,1.0,
  0.0,1.0
 };
 static float mvp[4]={1.0,0.0,0.0,1.0};
 static float stos;
 static float obrazek;
 float liczba;

 stos=(float)(szerokoscOkna-SZEROKOSC_MENU)/wysokoscOkna;
 ustawTeksture(&obrazek);
 liczba=obrazek/stos;
 if(liczba<1.0)
 {
  mvp[0]=liczba;
  mvp[3]=1.0;
 }
 else
 {
  mvp[0]=1.0;
  mvp[3]=1.0/liczba;
 }
 glViewport(SZEROKOSC_MENU,0,szerokoscOkna-SZEROKOSC_MENU,wysokoscOkna);
 glUseProgram(program);


 glActiveTexture(GL_TEXTURE0);
 glUniform1i(uniform_mytexture, GL_TEXTURE0);
 glUniformMatrix2fv(uniform_mvp,1,GL_FALSE,mvp);
 glBindTexture(GL_TEXTURE_2D, texture_id);
 glEnableVertexAttribArray(atryb_wierzch);
 glEnableVertexAttribArray(atryb_texcoord);
 glVertexAttribPointer
 (
  atryb_wierzch,
  2,
  GL_FLOAT,
  GL_FALSE,
  0,
  bufw
 );
 glVertexAttribPointer
 (
  atryb_texcoord,
  2,
  GL_FLOAT,
  GL_FALSE,
  0,
  buft
 );
 glDrawArrays(GL_TRIANGLE_FAN,0,4);
 glDisableVertexAttribArray(atryb_wierzch);
 glDisableVertexAttribArray(atryb_texcoord);
}
void rysowanieMenu()
{
 static float bufw[30*2];
 static float bufk[30*3];
 gui_cos*rzecz=NULL;
 glUseProgram(program1);
 glViewport(0,0,SZEROKOSC_MENU,wysokoscOkna);
 glEnableVertexAttribArray(atryb_mwierzch);
 glEnableVertexAttribArray(atryb_kolor);
 glVertexAttribPointer
 (
  atryb_mwierzch,
  2,
  GL_FLOAT,
  GL_FALSE,
  0,
  bufw
 );
 glVertexAttribPointer
 (
  atryb_kolor,
  3,
  GL_FLOAT,
  GL_FALSE,
  0,
  bufk
 );
 bufw[0*2+0]=-1.0;
 bufw[0*2+1]=-1.0;
 bufw[1*2+0]=1.0;
 bufw[1*2+1]=-1.0;
 bufw[2*2+0]=1.0;
 bufw[2*2+1]=1.0;
 bufw[3*2+0]=-1.0;
 bufw[3*2+1]=1.0;
 float kolor[3];
 if(czyZajety)
 {
  kolor[0]=0.18;
  kolor[1]=0.1;
  kolor[2]=0.0;
 }
 else
 {
  kolor[0]=0.0;
  kolor[1]=0.18;
  kolor[2]=0.0;
 }
 bufk[0*3+0]=kolor[0];
 bufk[0*3+1]=kolor[1];
 bufk[0*3+2]=kolor[2];
 bufk[1*3+0]=kolor[0];
 bufk[1*3+1]=kolor[1];
 bufk[1*3+2]=kolor[2];
 bufk[2*3+0]=kolor[0];
 bufk[2*3+1]=kolor[1];
 bufk[2*3+2]=kolor[2];
 bufk[3*3+0]=kolor[0];
 bufk[3*3+1]=kolor[1];
 bufk[3*3+2]=kolor[2];
 glDrawArrays(GL_TRIANGLE_FAN,0,4);
 rzecz=gui_poc;
 while(rzecz)
 {
  float prawo,gora,lewo,dol;
  switch(rzecz->rodzaj)
  {
   case GUI_R_PRZYCISK:
    prawo=rzecz->przycisk.prawo/(SZEROKOSC_MENU*0.5)-1.0;
    gora=1.0-(2.0*rzecz->przycisk.gora)/wysokoscOkna;
    lewo=rzecz->przycisk.lewo/(SZEROKOSC_MENU*0.5)-1.0;
    dol=1.0-(2.0*rzecz->przycisk.dol)/wysokoscOkna;
    bufw[0*2+0]=lewo;
    bufw[0*2+1]=dol;
    bufw[1*2+0]=prawo;
    bufw[1*2+1]=dol;
    bufw[2*2+0]=prawo;
    bufw[2*2+1]=gora;
    bufw[3*2+0]=lewo;
    bufw[3*2+1]=gora;
    
    if(rzecz==gui_wybrane)
    {
     kolor[0]=0.0;
     kolor[1]=0.5;
     kolor[2]=0.0;
    }
    else
    {
     kolor[0]=0.0;
     kolor[1]=0.36;
     kolor[2]=0.0;
    }

    bufk[0*3+0]=kolor[0];
    bufk[0*3+1]=kolor[1];
    bufk[0*3+2]=kolor[2];
    bufk[1*3+0]=kolor[0];
    bufk[1*3+1]=kolor[1];
    bufk[1*3+2]=kolor[2];
    bufk[2*3+0]=kolor[0];
    bufk[2*3+1]=kolor[1];
    bufk[2*3+2]=kolor[2];
    bufk[3*3+0]=kolor[0];
    bufk[3*3+1]=kolor[1];
    bufk[3*3+2]=kolor[2];

    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    break;
   case GUI_R_POLE_TEKSTOWE:
    prawo=rzecz->przycisk.prawo/(SZEROKOSC_MENU*0.5)-1.0;
    gora=1.0-(2.0*rzecz->przycisk.gora)/wysokoscOkna;
    lewo=rzecz->przycisk.lewo/(SZEROKOSC_MENU*0.5)-1.0;
    dol=1.0-(2.0*rzecz->przycisk.dol)/wysokoscOkna;
    bufw[0*2+0]=lewo;
    bufw[0*2+1]=dol;
    bufw[1*2+0]=prawo;
    bufw[1*2+1]=dol;
    bufw[2*2+0]=prawo;
    bufw[2*2+1]=gora;
    bufw[3*2+0]=lewo;
    bufw[3*2+1]=gora;
    
    if(rzecz==gui_wybrane)
    {
     kolor[0]=0.1;
     kolor[1]=0.32;
     kolor[2]=0.0;
    }
    else
    {
     kolor[0]=0.08;
     kolor[1]=0.25;
     kolor[2]=0.0;
    }

    bufk[0*3+0]=kolor[0];
    bufk[0*3+1]=kolor[1];
    bufk[0*3+2]=kolor[2];
    bufk[1*3+0]=kolor[0];
    bufk[1*3+1]=kolor[1];
    bufk[1*3+2]=kolor[2];
    bufk[2*3+0]=kolor[0];
    bufk[2*3+1]=kolor[1];
    bufk[2*3+2]=kolor[2];
    bufk[3*3+0]=kolor[0];
    bufk[3*3+1]=kolor[1];
    bufk[3*3+2]=kolor[2];

    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    break;
   case GUI_R_PRZELACZNIK:
    prawo=rzecz->przycisk.prawo/(SZEROKOSC_MENU*0.5)-1.0;
    gora=1.0-(2.0*rzecz->przycisk.gora)/wysokoscOkna;
    lewo=rzecz->przycisk.lewo/(SZEROKOSC_MENU*0.5)-1.0;
    dol=1.0-(2.0*rzecz->przycisk.dol)/wysokoscOkna;
    bufw[0*2+0]=lewo;
    bufw[0*2+1]=dol;
    bufw[1*2+0]=prawo;
    bufw[1*2+1]=dol;
    bufw[2*2+0]=prawo;
    bufw[2*2+1]=gora;
    bufw[3*2+0]=lewo;
    bufw[3*2+1]=gora;
    
    if(rzecz==gui_wybrane)
    {
     kolor[0]=0.16;
     kolor[1]=0.4;
     kolor[2]=0.0;
    }
    else
    {
     kolor[0]=0.12;
     kolor[1]=0.3;
     kolor[2]=0.0;
    }

    bufk[0*3+0]=kolor[0];
    bufk[0*3+1]=kolor[1];
    bufk[0*3+2]=kolor[2];
    bufk[1*3+0]=kolor[0];
    bufk[1*3+1]=kolor[1];
    bufk[1*3+2]=kolor[2];
    bufk[2*3+0]=kolor[0];
    bufk[2*3+1]=kolor[1];
    bufk[2*3+2]=kolor[2];
    bufk[3*3+0]=kolor[0];
    bufk[3*3+1]=kolor[1];
    bufk[3*3+2]=kolor[2];

    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    break;
  }
  rzecz=rzecz->nast;
 }
 glDisableVertexAttribArray(atryb_mwierzch);
 glDisableVertexAttribArray(atryb_kolor);















 static float mvp[4]={1.0,0.0,0.0,1.0};
 rzecz=gui_poc;
 glUseProgram(program);

 bufk[0]=0.0;
 bufk[1]=0.0;
 bufk[2]=1.0;
 bufk[3]=0.0;
 bufk[4]=1.0;
 bufk[5]=1.0;
 bufk[6]=0.0;
 bufk[7]=1.0;

 glUniformMatrix2fv(uniform_mvp,1,GL_FALSE,mvp);
 glEnableVertexAttribArray(atryb_wierzch);
 glEnableVertexAttribArray(atryb_texcoord);
 glVertexAttribPointer
 (
  atryb_wierzch,
  2,
  GL_FLOAT,
  GL_FALSE,
  0,
  bufw
 );
 glVertexAttribPointer
 (
  atryb_texcoord,
  2,
  GL_FLOAT,
  GL_FALSE,
  0,
  bufk
 );

 while(rzecz)
 {
  //float prawo,gora,lewo,dol;
  switch(rzecz->rodzaj)
  {
   case GUI_R_PRZYCISK:
    if(rzecz->przycisk.t)
    {
     Czworka kolor={{0,255,0,255}};
     rysujTekst(rzecz->przycisk.t,rzecz->przycisk.lewo,rzecz->przycisk.gora,kolor,bufw,bufk);
    }
    break;
   case GUI_R_NAPIS:
    if(rzecz->przycisk.t)
    {
     Czworka kolor={{0,255,0,255}};
     rysujTekst(rzecz->napis.t,rzecz->napis.lewo,rzecz->napis.gora,kolor,bufw,bufk);
    }
    break;
   case GUI_R_PRZELACZNIK:
    if(rzecz->przelacznik.w)
    {
     Czworka kolor={{0,255,0,255}};
     rysujTekst(napisy[rzecz->przelacznik.w[rzecz->przelacznik.wybor].ktore],rzecz->przelacznik.lewo,rzecz->przelacznik.gora,kolor,bufw,bufk);
    }
    break;
   case GUI_R_POLE_TEKSTOWE:
    if(rzecz->poleTekstowe.t)
    {
     Czworka kolor={{0,255,0,255}};
     rysujTekst(rzecz->poleTekstowe.t,rzecz->poleTekstowe.lewo,rzecz->poleTekstowe.gora,kolor,bufw,bufk);
    }
    else if(rzecz->poleTekstowe.tt)
    {
     Czworka kolor={{0,127,0,255}};
     rysujTekst(rzecz->poleTekstowe.tt,rzecz->poleTekstowe.lewo,rzecz->poleTekstowe.gora,kolor,bufw,bufk);
    }
    break;
  }
  rzecz=rzecz->nast;
 }
 glDisableVertexAttribArray(atryb_wierzch);
 glDisableVertexAttribArray(atryb_texcoord);



}
void rysowanie(SDL_Window*okno)
{
 glClearColor(0.0,0.0,0.0,1.0);
 glClear(GL_COLOR_BUFFER_BIT);
 rysowanieObrazka();
 rysowanieMenu();
 SDL_GL_SwapWindow(okno);
}
uint8_t ustawTeksture(float*stos)
{
 static uint32_t czyJest=0;
 Obraz*e=dajObraz();
 if(e==NULL)
 {
  if(czyJest)
  {
   glDeleteTextures(1,&texture_id);
  }
  czyJest=0;
  return B_PUSTY_WSKAZNIK;
 }
 if(pthread_mutex_trylock(&(e->muteks)))
 {
  //puts(" obraz jest zajęty przez inny wątek");
  return B_OBRAZ_ZAJETY_PRZEZ_COS;
 }
 if(e->piksele==NULL)
 {
 // puts(" obraz jest pusty");
  pthread_mutex_unlock(&(e->muteks));
  return B_BRAK_OBRAZU;
 }
 if(!(e->czyZmieniono||czyInny()))
 {
  pthread_mutex_unlock(&(e->muteks));
  return B_BRAK_ZMIAN;
 }
 if(czyJest)
 {
  glDeleteTextures(1,&texture_id);
 }
 glGenTextures(1,&texture_id);
 glBindTexture(GL_TEXTURE_2D,texture_id);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
 glTexImage2D
 (
  GL_TEXTURE_2D, // target
  0,  // level, 0 = base, no minimap,
  GL_RGBA, // internalformat
  e->szerokosc,  // width
  e->wysokosc,  // height
  0,  // border, always 0 in OpenGL ES
  GL_RGBA,  // format
  GL_UNSIGNED_BYTE, // type
  e->piksele
 );
 czyJest=1;
 *stos=(float)e->szerokosc/e->wysokosc;
 e->czyZmieniono=0;
 wziecieObrazu();
 pthread_mutex_unlock(&(e->muteks));
 return B_DOBRZE;
}

void rysujTekst(char*tekst,uint32_t x,uint32_t y,Czworka kolor,float*bufw,float*buft)
{
 if(tekst==NULL) return;
 int32_t
  szerokosc=1,
  wysokosc=1,
  szerokosc1=1,
  wysokosc1=1,
  s=0,
  w=0,
  miejscex=0,
  miejscey=0,
  przesuniecie=0;
 Czworka*e=NULL;
 e=malloc(sizeof(Czworka));
 *e=zera;
 while(*tekst)
 {
  int32_t i,j,ile=0;
  uint32_t ch=0;
  unsigned char c=0b10000000;
  if(((*tekst)&(unsigned char)0b11000000)==(unsigned char)0b11000000)
  {
   while(ile<6&&(c&(*tekst)))
   {
    ++ile;
    c>>=1;
   }
   ch=((unsigned char)(*tekst))%((unsigned char)(0b01000000)>>ile);

   while(1)
   {
    ++tekst;
    if((*tekst)=='\0'||ile<=1) break;
    ch<<=6;
    ch|=(*tekst&((char)(0b00111111)));
    --ile;
   }
  }
  else
  {
   ch=*tekst;
   ++tekst;
  }
  FT_Load_Char(ftface,ch,FT_LOAD_RENDER);
  s=ftface->glyph->bitmap.width;
  //printf("s: %d\n",s);
  w=ftface->glyph->bitmap.rows;
  //printf("w: %d\n",w);
  miejscex=przesuniecie+ftface->glyph->bitmap_left;
  //printf("miejscex: %d\n",miejscex);
  miejscey=20-ftface->glyph->bitmap_top;
  if(miejscey<0) miejscey=0;
  //printf("miejscey: %d\n",miejscey);
  //printf("szerokosc: %d\n",szerokosc);
  //printf("wysokosc: %d\n",wysokosc);
  szerokosc1=miejscex+s;
  //printf("szerokosc1: %d\n",szerokosc1);
  wysokosc1=miejscey+w;
  //printf("wysokosc1: %d\n===============\n",wysokosc1);
  if(szerokosc>szerokosc1) szerokosc1=szerokosc;
  if(wysokosc>wysokosc1) wysokosc1=wysokosc;
  Czworka *ee=rozszerz(e,szerokosc,wysokosc,szerokosc1,wysokosc1);
  if(ee==NULL)
  {
   free(e);
   return;
  }
  e=ee;
  szerokosc=szerokosc1;
  wysokosc=wysokosc1;
  i=0;
  while(i<w)
  {
   j=0;
   while(j<s)
   {
    Czworka*piksel=e+j+miejscex+szerokosc*(i+miejscey);
    if(piksel>=e+szerokosc*wysokosc||piksel<e)
    {
     //puts("O nie!");
     ++j;
     continue;
    }
    piksel->e[0]=kolor.e[0];
    piksel->e[1]=kolor.e[1];
    piksel->e[2]=kolor.e[2];
    piksel->e[3]=((uint16_t)kolor.e[3]*ftface->glyph->bitmap.buffer[i*ftface->glyph->bitmap.pitch+j]>>8);    
    ++j;
   }
   ++i;
  }
  przesuniecie+=ftface->glyph->advance.x>>6;
 }
 if(szerokosc>0)
 {
  glUseProgram(program);
  glGenTextures(1,&texture_tekst);
  glBindTexture(GL_TEXTURE_2D,texture_tekst);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexImage2D
  (
   GL_TEXTURE_2D,
   0,
   GL_RGBA,
   szerokosc,
   wysokosc,
   0,
   GL_RGBA,
   GL_UNSIGNED_BYTE,
   e
  );
  bufw[0]=-1.0+2.0*x/SZEROKOSC_MENU;
  bufw[1]=1.0-2.0*y/wysokoscOkna;
  bufw[2]=bufw[0]+szerokosc*2.0/SZEROKOSC_MENU;
  bufw[3]=bufw[1];
  bufw[4]=bufw[2];
  bufw[5]=bufw[1]-2.0*wysokosc/wysokoscOkna;
  bufw[6]=bufw[0];
  bufw[7]=bufw[5];
  glDrawArrays(GL_TRIANGLE_FAN,0,4);
  glDeleteTextures(1,&texture_tekst);
 }
 free(e);
}


Czworka *rozszerz(Czworka*piksele,uint32_t szerokosc,uint32_t wysokosc,uint32_t nowaSzerokosc,uint32_t nowaWysokosc)
{
 //printf("Wcześniejsze wymiary: %d %d\nNowe wymiary:         %d %d\n",szerokosc,wysokosc,nowaSzerokosc,nowaWysokosc);
 if(piksele==NULL) return NULL;
 Czworka *w=malloc(sizeof(Czworka)*nowaSzerokosc*nowaWysokosc);
 if(w==NULL)
 {
  printf("Funkcja rozszerz nie może przydzielić pamięci, ale nie wiem dlaczego :/\n");
  return NULL;
 }
 uint32_t x=0,y=0;
 while(y<nowaWysokosc)
 {
  x=0;
  while(x<nowaSzerokosc)
  {
   if(x<szerokosc&&y<wysokosc)
   {
    *(w+x+y*nowaSzerokosc)=*(piksele+x+y*szerokosc);
   }
   else
   {
    *(w+x+y*nowaSzerokosc)=zera;
   }
   ++x;
  }
  ++y;
 }
 free(piksele);
 return w;
}































//////////////////////////////////////////////////////////////////////////////
