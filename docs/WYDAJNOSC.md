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

## `array<PieceState, 22>` kontra `StaticMap<Position, PieceStack, 22>`

Pomiar wykonano 2026-07-18 na tej samej maszynie i w buildzie zoptymalizowanym.

| Reprezentacja | Depth 7 | Depth 8 | Węzły depth 8 |
|---|---:|---:|---:|
| `array<PieceState, 22>` | 1007 ms | 4402 ms | 1 665 980 |
| `StaticMap`, pomiar 1 | 776 ms | 3330 ms | 1 665 980 |
| `StaticMap`, pomiar 2 | 775 ms | 3389 ms | 1 665 980 |
| `StaticMap`, pomiar 3 | 779 ms | 3340 ms | 1 665 980 |

Mediana nowej wersji dla depth 8 wynosi 3340 ms. Względem wcześniejszego pomiaru 4402 ms jest to
około 24% krótszy czas. Identyczna liczba węzłów potwierdza, że porównano ten sam przebieg searchu.

Wynik bazowy pochodzi z jednego uruchomienia, więc 24% należy traktować jako wynik wstępny, a nie
precyzyjny benchmark statystyczny. Kolejne pomiary powinny:

- wykonywać rozgrzewkę,
- uruchamiać obie wersje naprzemiennie,
- raportować medianę z co najmniej 5 prób,
- mierzyć alokacje oraz osobno `generate`, `apply` i `undo`.

## Już usunięte koszty

- drzewiasta `std::map` zajętych pól i jej alokacje,
- mapa notacji pionków,
- kopiowanie `emptyTiles` w każdym węźle AI,
- własnościowe alokacje dwóch graczy,
- mapy liczników figur,
- logowanie z konstruktorów `Tile` i parsera.

## Następne podejrzane miejsca

- `std::set` w generatorach mrówki i pająka,
- `std::map` oraz `std::set` w punktach artykulacji,
- kopiowanie `avaliableActions` na każdym poziomie alpha-beta,
- zwracanie `Tile` razem ze stringiem notacji przez wartość,
- przebudowywanie wszystkich obiektów rysowania przy każdym odświeżeniu GUI.
