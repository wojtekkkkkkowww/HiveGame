# Pomiary wydajności

## Metoda

Build:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
```

Benchmark:

```sh
build/ai_tests/AiTester perft 8
```

Perft ogranicza każdą pozycję do pierwszych sześciu akcji, dlatego wyniki służą do porównywania
wersji tego repo, a nie do porównania z innymi silnikami Hive.

## Wynik końcowy: `main`, `std::map` i `StaticMap`

Pomiar wykonano 2026-07-18 na tej samej maszynie, w buildzie Release. Każdy wariant uruchomiono
pięć razy. Warianty obecnego kodu były uruchamiane naprzemiennie, żeby ograniczyć wpływ chwilowego
obciążenia komputera.

| Wersja | Próby depth 8 [ms] | Mediana | Węzły |
|---|---|---:|---:|
| czysty branch `main` | 1275, 1265, 1265, 1273, 1264 | 1265 ms | 1 665 980 |
| refaktor z `std::map` (2026-07-18) | 804, 807, 802, 807, 805 | 805 ms | 1 665 980 |
| refaktor z `StaticMap` (2026-07-18) | 770, 771, 775, 775, 777 | 775 ms | 1 665 980 |
| obecny `std::map` (2026-07-19) | 762, 763, 758, 780, 764 | 763 ms | 1 665 980 |
| obecny `StaticMap` (2026-07-19) | 734, 740, 740, 738, 742 | 740 ms | 1 665 980 |

Wnioski:

- `StaticMap` jest obecnie około 3,0% szybszy od `std::map` w tym benchmarku.
- Obecny refaktor ze `StaticMap` jest około 41,5% szybszy od czystego `main`.
- Identyczna liczba węzłów potwierdza ten sam przebieg perft.
- Największy zysk nie pochodzi z kontenera. Daje go usunięcie ponownej generacji ruchów po każdym
  `undo`, zbędnych zbiorów, kopii `emptyTiles` i logowania w hot pathach.
- Bufory akcji alpha-beta są przechowywane osobno dla każdej głębokości. Elementy nie są kopiowane,
  a pojemność wektorów jest ponownie wykorzystywana. Naiwne przenoszenie jednego wektora
  pogarszało wynik do mediany 761 ms przez dodatkowe alokacje.

### Przełączanie backendu planszy

```sh
# StaticMap
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DHIVE_USE_STATIC_MAP=ON

# std::map (tymczasowo domyślnie podczas diagnozy exception)
cmake -S . -B build-stdmap -DCMAKE_BUILD_TYPE=Release -DHIVE_USE_STATIC_MAP=OFF
```

Oba backendy korzystają z tego samego API `Board`. Limit 22 figur jest egzekwowany przez `Board`,
a nie przez przypadkową pojemność kontenera.

## Znaleziona i usunięta regresja

Jedna wersja pośrednia używała `array<Tile, 22>` jako wartości każdego z 22 wpisów `StaticMap`.
Powodowało to utworzenie miejsca na 484 obiekty `Tile` oraz kosztowne czyszczenie/przenoszenie
całych stosów. Jeszcze większym kosztem było ponowne generowanie legalnych ruchów po każdym
cofnięciu searchu. Wersja pośrednia osiągała około 2800 ms i była ponad dwa razy wolniejsza od
`main`.

Aktualnie wartością wpisu jest lekki `deque<Tile>`, a search wykonuje zaufaną akcję z wcześniej
wygenerowanej listy i cofa wyłącznie zmieniony stan. Legalne ruchy są generowane raz na węzeł.

## Już usunięte koszty

- opcjonalnie drzewiasta `std::map` zajętych pól i jej alokacje,
- mapa notacji pionków,
- kopiowanie `emptyTiles` w każdym węźle AI,
- własnościowe alokacje dwóch graczy,
- mapy liczników figur,
- logowanie z konstruktorów `Tile` i parsera.

## Następne podejrzane miejsca

- `std::set` w generatorach mrówki i pająka,
- `std::map` oraz `std::set` w punktach artykulacji,
- zwracanie `Tile` razem ze stringiem notacji przez wartość,
- przebudowywanie wszystkich obiektów rysowania przy każdym odświeżeniu GUI.
