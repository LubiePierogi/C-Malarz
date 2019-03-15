# C-Malarz
Program do robienia obrazkóœ zrobiony w C.

Ten program był zrobiony w styczniu 2017 na projekt z PRI.

## Wymagania:

* Unix Makefile
* gcc
* OpenGL 2
* GLEW
* SDL
* Freetype 2

## Kompilowanko:
`$ make`

## xd

W programie jest font Ubuntu. [Stronka](https://design.ubuntu.com/font/)

## Sposób użycia

Po lewej stronie okna programu jest menu, a po prawej widok obrazu. W menu są do
o wyboru następujące opcje:

* Nowy obraz
* Zapisz obraz
* Narysuj wzór
* Efekty
* Wcześniejszy
* Następny
* Kopiuj obraz
* Wyjście

Wybór opcji „Nowy obraz” pozwala na wpisanie wymiarów nowego obrazu
i umieszczenie go na liście obrazów w programie. Jeśli pola przeznaczone
do wpisania wymiarów pozostaną puste lub jeśli będzie w nich nieprawidłowy
napis, ich wartości zostaną zastąpione wartością 1000. Po wybraniu opcji
„Zapisz obraz” trzeba wpisać nazwę pliku i wybrać opcję zapisz. W przypadku
pozostawienia pustego pola na nazwę plik zostanie nazwany „wyjście.bmp”.

Opcja „Narysuj wzór” pozwala na narysowanie fraktala lub wykresu funkcji
wymiernej w zbiorze liczb zespolonych.

Opcja „Efekty” pozwala na rozmycie lub wyostrzenie obrazu.

Opcja „Wcześniejszy” wybiera poprzedni obraz na liście, jeśli istnieje.

Opcja „Następny” przechodzi do następnego istniejącego obrazu.

Opcja „Kopiuj obraz” kopiuje wybrany obraz i wstawia go do listy.

Opcja „Usuń obraz” usuwa z pamięci wybrany obraz i przechodzi do następnego,
jeśli istnieje.

Opcja „Wyjście” usuwa z pamięci wszystkie obrazy i kończy program.

Ekran rysowania fraktala zawiera następujące pola:

* Wybór zbioru do narysowania.
* Sposób kolorowania obrazu.
* Część rzeczywista liczby, która jest przekazywana do funkcji, domyślna wartość
  tego pola to liczba 0.
* Część urojona tejże liczby, również domyślnie 0.
* Miejsce, w którym jest górny brzeg obrazu, domyślnie 2.
* Lewy brzeg obrazu, domyślnie -2.
* Prawy brzeg, domyślnie 2.
* Dolny brzeg, domyślnie -2.
* Największa ilość powtórzeń funkcji.

Ekran rysowania wykresu funkcji wymiernej ma następujące pola:
* Sposób kolorowania wykresu.
* Zbiór pierwiastków wielomianu w liczniku. Są to pary liczb rzeczywistych,
  z których tworzone są liczby urojone. Liczby należy oddzielać spacjami.
* Zbiór pierwiastków wielomianu w mianowniku. Trzeba je wypełniać tak jak pole
  powyżej.
* Górny brzeg wykresu, domyślnie 2.
* Lewy brzeg wykresu, domyślnie -2.
* Prawy brzeg wykresu, domyślnie 2.
* Dolny brzeg wykresu, domyślnie -2.
* Pola na część rzeczywistą i urojoną liczby, przez którą jest mnożony cały
  wykres, jeśli jedno pole jest puste lub zawiera nieprawidłową liczbę jest
  zastępowane przez 0. Jeśli oba są puste, czynnik jest równy 1.
* Jeśli program
  jest w trakcie pracy, tło menu jest brązowe, w przeciwnym wypadku jest zielone.

## Działanko

TL;DR Nic ciekawego xD

Obrazy są przechowywane w liście dwukierunkowej.

Wszystkie działania na obrazach są wykonywane przez oddzielny wątek, a wątek od
okna przekazuje do niego zlecenia przez kolejkę.

Funkcja rysująca fraktal przelicza współrzędne każdego piksela na obrazku na
liczbę zespoloną, a następnie oblicza dla niej określoną funkcję, dopóki spełnia
ona pewien warunek i nie przekroczono ograniczenia ilości powtórzeń. Następnie
ilość powtórzeń, która została zrobiona, jest zamieniana na kolor.

Funkcja rysująca wykres przelicza współrzędne pikseli na liczby urojone, oblicza
dla nich wartości funkcji i na podstawie tych wartości oblicza kolor dla danego
piksela.

Funkcja rozmywająca obraz tworzy w pamięci miejsce na nowy obraz i dla każdemu
pikselowi przyporządkowuje kolor będący średnim kolorem odpowiedniego piksela
w pierwszym obrazku i sąsiadujących z nim i istniejących pikseli.

Funkcja wyostrzająca robi to samo, co funkcja rozmywająca, tylko zamiast
przyporządkowywać pikselom ich rozmycie, oblicza różnicę między drugim obrazem
a pierwszym i odejmuje tę różnicę od drugiego. W przypadku, gdy jakaś część
składowa koloru przekroczy możliwy zakres z góry, otrzymuje największą możliwą
wartość, a gdy z dołu, jest zerowana.

Obrazy są zapisywane w formacie BMP.

# xdxdxd
