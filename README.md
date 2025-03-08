# GIPF - Projekt Gry z Solverem

## Opis Projektu
Projekt ma na celu stworzenie gry **GIPF** wraz z prostym solverem. Gra GIPF to strategiczna gra planszowa, w której gracze rywalizują, umieszczając i przesuwając pionki na planszy w kształcie heksagonu. Zasady gry można znaleźć na stronie: [GIPF Rulebook](https://files.rebel.pl/files/instrukcje/Instrukcja_Gipf_PL.pdf).

Projekt jest dedykowany na platformę **STOS Politechniki Gdańskiej**, która narzuca określone wymagania. W związku z tym, przy uruchamianiu projektu na własnym sprzęcie mogą pojawić się nieoczekiwane błędy.

## Wymagania Systemowe
Aby uruchomić projekt, potrzebujesz:
- **Visual Studio 2019** lub nowszej wersji.
- Środowisko do kompilacji i uruchamiania aplikacji C++.

## Instrukcja Uruchomienia
1. Sklonuj repozytorium na swój komputer.
2. Otwórz plik `*.sln` w Visual Studio.
3. Skompiluj i uruchom projekt.

## Uwaga
Ze względu na wymagania zadania, program nie zawiera wbudowanych instrukcji dotyczących dozwolonych komend. Niewłaściwe działania mogą celowo spowodować błędy i przerwać działanie programu.

## Dostępne Komendy
Po uruchomieniu programu można korzystać z następujących komend:

- **p** - Drukuje aktualny stan planszy.
- **LOAD_GAME_BOARD** - Ładuje nową grę. Po tej komendzie należy podać:
  - Wielkość planszy.
  - Liczbę pionków, które można zebrać naraz.
  - Rezerwę białych pionków.
  - Rezerwę czarnych pionków.
  Każdą wartość należy potwierdzić osobno, naciskając Enter.
- **d** - Wykonuje ruch gracza, który ma aktualnie turę. Po komendzie należy podać polecenie w formacie `skąd-dokąd`, np. `a4-b5`. Szczegóły poniżej w sekcji **Objaśnienie Planszy** oraz **Przykład Ruchu**.
- **PRINT_GAME_STATE** - Drukuje aktualny stan gry.
- **IS_GAME_OVER** - Sprawdza, czy gra się zakończyła.
- **GEN_ALL_POS_MOV** - Generuje i wyświetla wszystkie możliwe ruchy.
- **GEN_ALL_POS_MOV_NUM** - Wyświetla liczbę unikatowych ruchów.
- **GEN_ALL_POS_MOV_EXT** - Generuje wszystkie możliwe ruchy i wyświetla ruch wygrywający (jeśli istnieje). W przeciwnym razie wyświetla wszystkie ruchy.
- **GEN_ALL_POS_MOV_EXT_NUM** - Wyświetla liczbę unikatowych ruchów. Jeśli istnieje ruch wygrywający, wyświetla informację o jednym unikalnym ruchu.
- **SOLVE_GAME_STATE** - Sprawdza, czy możliwe jest wygranie gry w określonej liczbie ruchów. Po komendzie należy podać liczbę ruchów (maksymalnie 4).

## Objaśnienie Planszy
Plansza w grze GIPF ma kształt heksagonu. Dla planszy o wielkości 4, prawdziwy układ pól wygląda następująco:

```
a5 b6 c7 d8 e9
a4 b5 c6 d7 e8 f8
a3 b4 c5 d6 e7 f7 g7
a2 b3 c4 d5 e6 f6 g6 h6
a1 b2 c3 d4 e5 f5 g5 h5 i5
   b1 c2 d3 e4 f4 g4 h4 i4
      c1 d2 e3 f3 g3 h3 i3
         d1 e2 f2 g2 h2 i2
            e1 f1 g1 h1 i1
```

Natomaist w programie wyświetlane są następująco:

```
    _ _ _ _ 
   _ _ _ _ _ 
  _ _ _ _ _ _ 
 _ _ _ _ _ _ _ 
  _ _ _ _ _ _ 
   _ _ _ _ _ 
    _ _ _ _ 
```

gdzie najbardziej skrajny na lewo punkt to b2, lewy wierzchołek w górnym wierszu to b5, a prawy wierzchołek w dolnym wierszu to h2, itd. 

Pola na obwodzie planszy (np. a4, b5, g1, d1, b1, e6) są niewidoczne i służą jedynie do określenia, skąd można wstawić pionek.

### Przykład Ruchu
- Komenda `d a4-b4` postawi pionka na polu `b4`.
- Jeśli następnie użyjesz komendy `d a3-b4`, wcześniejszy pionek zostanie przesunięty na pole `c5`, a nowy pionek pojawi się na `b4`.
