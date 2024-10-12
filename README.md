# HiveGame

## Opis projektu

**HiveGame** to gra strategiczna oparta na popularnej grze planszowej **Hive**. Projekt został napisany w języku C++ i wykorzystuje bibliotekę **SFML** do renderowania grafiki oraz obsługi interakcji użytkownika. Narazie została zaimplementowana logika gry (trzeba dodać więcej testów) i możliwość gry dla dwóch graczy.

## Wymagania

Aby uruchomić projekt na systemie Linux, należy zainstalować następujące pakiety:
- `cmake`
- `libsfml-dev`
- `g++`

Instalacja za pomocą menedżera pakietów, np. `apt` lub `dnf`:

```bash
sudo dnf install libsfml-dev cmake g++
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

Po zbudowaniu projektu można uruchomić testy w katalogu `build`. Testy generują pliki `.png` w katalogu build przedstawiające przebieg gry w danym teście.

Aby uruchomić testy:
```bash
./tests/test_GameLogic
```

## Uruchomienie gry dla dwóch graczy

Aby uruchomić grę:
```bash
./main/HiveGameApp
```

--- 

