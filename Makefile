CC=gcc
LD=-lGL -lSDL2 -lGLEW -lm -lpthread -lfreetype
ZXC=-Wall
STD=-std=c99
INCLUDE=-I/usr/include/freetype2
LDFLAGS=
all: ./obj/malarz.o ./obj/wykres.o ./obj/doubel.o ./obj/a.o ./obj/gui.o ./obj/zlecenia.o ./obj/inne.o
	$(CC) ./obj/malarz.o ./obj/wykres.o ./obj/doubel.o ./obj/a.o ./obj/gui.o ./obj/zlecenia.o ./obj/inne.o -o malarz $(INCLUDE) $(LD) $(STD) $(ZXC)
./obj/malarz.o: ./src/malarz.c ./src/malarz.h ./src/wykres.h ./src/a.h ./src/doubel.h ./src/gui.h ./src/inne.h
	$(CC) ./src/malarz.c -o ./obj/malarz.o -c $(INCLUDE) $(STD) $(ZXC)
./obj/wykres.o: ./src/wykres.c ./src/wykres.h ./src/a.h ./src/doubel.h
	$(CC) ./src/wykres.c -o ./obj/wykres.o -c $(INCLUDE) $(STD) $(ZXC)
./obj/doubel.o: ./src/doubel.c ./src/doubel.h ./src/a.h
	$(CC) ./src/doubel.c -o ./obj/doubel.o -c $(INCLUDE) $(STD) $(ZXC)
./obj/a.o: ./src/a.c ./src/a.h
	$(CC) ./src/a.c -o ./obj/a.o -c $(INCLUDE) $(STD) $(ZXC)
./obj/gui.o: ./src/gui.c ./src/gui.h ./src/a.h
	$(CC) ./src/gui.c -o ./obj/gui.o -c $(INCLUDE) $(STD) $(ZXC)
./obj/zlecenia.o: ./src/zlecenia.c ./src/zlecenia.h ./src/a.h
	$(CC) ./src/zlecenia.c -o ./obj/zlecenia.o -c $(INCLUDE) $(STD) $(ZXC)
./obj/inne.o: ./src/inne.c ./src/inne.h ./src/a.h
	$(CC) ./src/inne.c -o ./obj/inne.o -c $(INCLUDE) $(STD) $(ZXC)
