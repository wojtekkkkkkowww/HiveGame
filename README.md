# HiveGame

## Opis projektu

Implementacja gry planszowej **Hive**. Projekt został napisany w języku C++ na linux i wykorzystuje bibliotekę **SFML**.
Do gry wykorzystuje komunikację TCP.

## PNG SOURCES
https://www.instructables.com/Hive-game-with-box/


## Wymagania

Należy zainstalować następujące pakiety: SFML, SFML-devel, gtest, gtest-devel, cmake

## Budowanie projektu

1. Utwórz nowy katalog `build`:
   ```bash
   mkdir build
   cd build
   ```

2. W katalogu `build` uruchom:
   ```bash
   cmake ..
   make
   ```
Po skompilowaniu projektu w katalogu build pojawią się foldery z plikami wykonywalnymi: `server`, `client`, `gameViewer`, `tests`, `ai_tests`.

## Testy jednostkowe

Po zbudowaniu projektu można uruchomić testy w katalogu `build`. Testy generują pliki `.txt` z zapisem ruchów  w katalogu `build`.

Aby uruchomić wszystkie Unit Testy:
```bash
./tests/test_GameLogic
```

Możliwe jest również uruchamianie pojedynczych testów, podając nazwę testu jako filtr. Na przykład:

```bash
./tests/test_GameLogic --gtest_filter=TileMovementTest.WhiteWins
```

## Przeglądanie zapisanych partii
Do przeglądania zapisanej parti można użyć programu `GameViewer`. Program przyjmuje jako argument ścieżkę do pliku z zapisem partii.
Zapisy parti zostają wygenerowane po uruchomieniu testów jednostkowych, lub uruchomieniu pojedynczej gry w AiTester.

```bash
./gameViewer/GameViewer path_to_file
```

## Uruchomienie gry
Należy uruchomić server i dwóch klientów np.
```bash
./server/Server 53000
./client/HiveClient 53000 White
./client/HiveClient 53000 Black
```
Gra przeciwko AI
```bash
./server/Server 53000
./client/HiveClient 53000 White
./client/AiClient 53000 Black
```

## Eksperymenty AI
Uruchomienie pojedynczej gry:
```bash
./ai_tests/AiTester random alphabeta
``` 
Uruchomienie testu dla wielu gier:
```bash
./ai_tests/AiTester test
```
Uruchomienie algorytmu SA:
```bash
./ai_tests/AiTester optimize
```
Uruchomienie testu performance:
```bash
./ai_tests/AiTester perft
```
Uruchomienie testu dla bf:
```bash
./ai_tests/AiTester bf
```
## skrypty
w folderze /scripts 
znajdują się skrypty do tworzenia wykresów: 
skrypty 'bf.py', 'heat.py', 'sa.py'
Oraz skrypt mierzący czas bota: 'calculate.sh'

W katalogu /results są pliki z wynikami testów 'cache.txt', 'bf.txt'
Uruchomienie skryptu dla wykrsu branching factor:
```bash
python3 bf.py
```
--- 


