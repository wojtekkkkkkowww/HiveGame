# HiveGame

## Opis projektu

Implementacja gry planszowej **Hive**. Projekt został napisany w języku C++ na linux i wykorzystuje bibliotekę **SFML**. Narazie została zaimplementowana logika gry (trzeba dodać więcej testów) i gui umożliwiające rozgrywkę dla dwuch graczy.

## PNG SOURCES
https://www.instructables.com/Hive-game-with-box/


## Wymagania

Należy zainstalować pakiety podane przez SFML:
https://github.com/SFML/cmake-sfml-project/blob/master/README.md
Instalacja za pomocą menedżera pakietów, np. `apt` :

```bash
sudo apt update
sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libfreetype-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev
```

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

## Testy jednostkowe

Po zbudowaniu projektu można uruchomić testy w katalogu `build`. Testy generują pliki `.png` w katalogu `build`, które przedstawiają przebieg gry w danym teście.

Aby uruchomić wszystkie testy:
```bash
./tests/test_GameLogic
```

Możliwe jest również uruchamianie pojedynczych testów, podając nazwę testu jako filtr. Na przykład:

```bash
./tests/test_GameLogic --gtest_filter=TileMovementTest.WhiteWins
```

## Uruchomienie gry dla dwóch graczy

Aby uruchomić grę:
```bash
./main/HiveGameApp
```

--- 


