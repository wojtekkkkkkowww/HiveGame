# Kontrola jakości C++20

Projekt wymaga standardu C++20 bez rozszerzeń kompilatora. Zwykły build ma włączone podstawowe,
rygorystyczne ostrzeżenia:

- Clang/GCC: `-Wall -Wextra -Wpedantic -Werror`,
- MSVC: `/W4 /WX`.

Ostrzeżenie kompilatora przerywa build, dlatego nie jest możliwe przypadkowe pozostawienie nowego
warningu w kodzie.

## clang-tidy

Opcjonalny build z analizą statyczną:

```sh
cmake -S . -B build-tidy \
    -DCMAKE_BUILD_TYPE=Debug \
    -DHIVE_ENABLE_CLANG_TIDY=ON
cmake --build build-tidy -j4
```

Konfiguracja `.clang-tidy` celowo nie narzuca rozbudowanej stylistyki. Sprawdza:

- analizator przepływu Clanga (`clang-analyzer-*`),
- typowe błędy (`bugprone-*`),
- zbędne kopie i inne problemy wydajnościowe (`performance-*`),
- przenośność (`portability-*`),
- podstawowe elementy współczesnego C++ (`override`, `nullptr`).

Analiza jest podpięta do silnika i AI. GUI nadal przechodzi pełny zestaw warningów kompilatora,
ale nie jest przekazywane do clang-tidy 21, ponieważ nagłówki SFML 2.6 są niezgodne z jego
aktualną implementacją `std::char_traits<sf::Uint32>`.

## Formatowanie

Kod można sformatować za pomocą:

```sh
clang-format -i path/to/file.cpp
```

Przed commitem warto również uruchomić:

```sh
git diff --check
```
