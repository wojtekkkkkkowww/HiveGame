# Audyt kodu HiveGame

Data audytu: 2026-07-18

## Cel dokumentu

Ten dokument opisuje, jak działa obecna wersja projektu, jakie ma mankamenty i w jakiej
kolejności warto ją modernizować. Audyt nie zakłada wielkiego przepisania od zera. Reguły Hive
są subtelne, dlatego bezpieczniej zachować działające testy i wymieniać kolejne elementy za
stabilnymi interfejsami.

## Stan projektu

- Projekt kompiluje się jako C++20.
- Wszystkie obecne testy przechodzą: 34 testy z 12 zestawów.
- Pełny zestaw testów logiki trwa około 10 ms, więc same testy jednostkowe nie wskazują dziś
  problemu wydajnościowego.
- Testy nie obejmują limitu ruchów, błędnych zapisów ruchów, rozłączeń sieciowych, zakończenia
  pętli klienta ani wielu operacji `apply/revert` używanych przez AI.
- W repo są lokalne, niezatwierdzone zmiany. Audyt ich nie nadpisuje.

## Jak obecnie działa program

### Silnik

`Game` jest fasadą, ale równocześnie publicznie udostępnia prawie cały stan:

- `Board board`,
- `currentTurn`,
- `players`,
- `gameStatus`,
- `avaliableActions`.

`ActionHandler` generuje, waliduje, wykonuje i cofa akcje. `TurnManager` zmienia gracza oraz
sprawdza otoczenie hetmanek. `ActionParser` tłumaczy pomiędzy `Action` i tekstowym zapisem partii.
Wszystkie te obiekty przechowują referencje do pól należących do `Game`, dlatego stan jest
rozproszony i można go zmienić z pominięciem reguł.

Plansza to:

- `map<Position, deque<Tile>> boardTiles` — stos pionków na zajętym polu,
- `set<Position> emptyTiles` — puste pola na granicy ula,
- `map<string, Position> positions` — indeks zapisu pionka,
- osobne pola pozycji obu hetmanek,
- osobny zbiór punktów artykulacji.

Generator ruchów używa innych algorytmów dla poszczególnych figur. Przed generowaniem ruchów
wyznaczane są punkty artykulacji, aby nie rozłączyć ula.

### AI

`AlphaBetaAI` modyfikuje jeden obiekt `Game`, schodzi rekurencyjnie i cofa ruchy. Przed każdą
gałęzią kopiuje listę akcji i `emptyTiles`, ponieważ obecne API cofania nie potrafi samodzielnie
odtworzyć pełnego stanu. To jest najważniejsze źródło zbędnych kopii w wyszukiwaniu.

### GUI i sieć

`GameController` ma własny lokalny obiekt `Game`. `HumanClient` przekazuje tekst ruchu do serwera,
a serwer jedynie przesyła tekst drugiemu klientowi. Serwer nie posiada silnika i nie waliduje
partii.

AI uruchamiane jako osobny klient również ma własny `Game`. Sockety są więc potrzebne wyłącznie
do komunikacji pomiędzy procesami, a nie do samej rozgrywki z AI.

## Mankamenty krytyczne — poprawność i stabilność

### P0. Nieprawidłowa akcja trafia do historii

Obie wersje `Game::applyAction` dodają tekst do `actionStrings` przed sprawdzeniem
`ActionHandler::applyAction`. Po odrzuceniu ruchu `getLastAction()` może zwrócić ruch, którego
nie wykonano. Zapis partii i komunikacja GUI mogą się przez to rozjechać ze stanem silnika.

Naprawa: najpierw zweryfikować i wykonać akcję, a dopiero potem dopisać jej zapis.

### P0. Cofnięcie postawienia hetmanki resetuje niewłaściwy kolor

`ActionHandler::revertPacedQueen` wybiera hetmankę na podstawie `currentTurn`. W momencie cofania
`currentTurn` wskazuje już przeciwnika. Pole pozycji cofanej hetmanki może więc pozostać ustawione,
a wyczyszczone zostanie pole drugiego gracza.

Naprawa: kolor wynika z cofanej figury lub gracza, który wykonał zapisaną akcję. Najlepiej, aby
rekord undo zawierał kompletną zmianę stanu, zamiast rekonstruować ją z bieżącej tury.

### P0. Brak limitu ruchów

`TurnManager::updateGameStatus` sprawdza tylko otoczenie hetmanek. Nie istnieje licznik całkowitych
półruchów ani reguła remisu po limicie. AI kontra AI i klient sieciowy mogą działać bez końca.

Naprawa: dodać konfigurowalny `maxPly`, licznik `ply`, `GameResult` oraz testy dla ruchu przed
limitem, ruchu osiągającego limit i cofnięcia ruchu z limitu.

### P0. Serwer w praktyce nie rozpoznaje końca gry

Serwer czeka na wiadomości `WHITE_WINS`, `BLACK_WINS` lub `DRAW`, ale klienci wysyłają zapis
wykonanej akcji. Status gry nie jest wysyłany. Pętla serwera nie kończy się po normalnym końcu
partii.

Naprawa krótkoterminowa: klient wysyła jawny komunikat końca według zdefiniowanego protokołu.
Naprawa docelowa dla multiplayera: serwer jest autorytatywny i sam stosuje akcje do `Game`.

### P0. Parser maskuje błędy wejścia

- Używa `regex_search`, więc może zaakceptować poprawny fragment niepoprawnego tekstu.
- Niepoprawny zapis jest zamieniany na `WAIT`.
- `catch (...)` wypisuje błąd, po czym funkcja kontynuuje pracę z nieprawidłowymi danymi.
- Regexy są konstruowane przy każdym parsowaniu.

Naprawa: `std::expected<Action, ParseError>` (albo zgodny odpowiednik), pełne dopasowanie wejścia,
brak `catch (...)` i parser oparty na prostym skanowaniu znaków. Tekstowy zapis powinien istnieć
na granicy pliku/sieci, nie w logice ruchu.

### P0. `startGameFromState` nie rozpoczyna od czystego stanu

Metoda dokłada pionki do aktualnej planszy, nie resetuje graczy, historii ani wszystkich liczników.
Nie odtwarza również liczby pozostałych figur i liczników tur. Stan utworzony tą metodą może być
wewnętrznie niespójny.

Naprawa: przyjmować kompletny `GameState`, walidować go i atomowo zastępować obecny stan albo
udostępnić osobną fabrykę `Game::fromState`.

### P1. Domyślny `Tile` zawiera niezainicjalizowane `color` i `type`

Domyślny konstruktor nie ustawia pól znakowych. Kod parsera tworzy takie obiekty, a po błędzie
może kontynuować ich użycie. To otwiera drogę do niezdefiniowanego zachowania.

Naprawa: silne typy `PieceType` i `PlayerColor`, poprawne wartości domyślne albo brak konstruktora
domyślnego dla niekompletnej figury.

### P1. Klient ma pętle bez warunku zakończenia

- wynik `startConnection()` jest ignorowany,
- oczekiwanie na `START` może kręcić się aktywnie na nieblokującym sockecie,
- główna pętla to `while (true)`,
- zamknięcie okna nie kończy klienta,
- rozłączenie ani koniec gry nie kończą pętli.

Naprawa: jawny stan sesji (`Connecting`, `Lobby`, `Playing`, `Finished`, `Disconnected`) oraz jedna
pętla aplikacji, która przetwarza GUI, AI i opcjonalną sieć.

### P1. TCP nie ma ramkowania wiadomości

Kod zakłada, że jedno `send` odpowiada jednemu `receive`. TCP tego nie gwarantuje: wiadomość może
zostać podzielona albo kilka wiadomości może zostać sklejonych.

Naprawa: pakiet SFML, prefiks długości lub protokół liniowy z buforem wejściowym.

## Projekt typów i własność

### Surowe wskaźniki graczy

`map<char, Player*>` zawiera dokładnie dwóch graczy, alokowanych przez `new` i usuwanych ręcznie.
To komplikuje konstrukcję `Game`, wymusza destruktor i domyślnie pozwala na niebezpieczne
kopiowanie `Game`.

Najprostszy model:

```cpp
enum class PlayerColor : std::uint8_t { White, Black };

struct Players {
    Player white{PlayerColor::White};
    Player black{PlayerColor::Black};

    Player& operator[](PlayerColor color);
    const Player& operator[](PlayerColor color) const;
};
```

Można też użyć `std::array<Player, 2>`. `unique_ptr` nie jest tu potrzebny, bo obiekty mają stały
czas życia i nie są polimorficzne.

### `map` kontra `array`

Pomysł zamiany mapy na tablicę jest dobry dla zamkniętych, małych zbiorów:

- dwóch graczy,
- pięciu bazowych typów figur,
- sześciu kierunków,
- liczników figur,
- tabeli metadanych figur.

Zamiast `pair` lepsze będą nazwane struktury, np.:

```cpp
struct PieceDefinition {
    PieceType type;
    std::uint8_t initialCount;
    AssetId asset;
    MovementKind movement;
};
```

Nie należy indeksować tablicy bezpośrednio współrzędnymi, bo plansza ma współrzędne ujemne.
Nie oznacza to jednak, że plansza wymaga mapy. Bazowa gra ma maksymalnie 22 figury, dlatego
docelowym, głównym źródłem prawdy może być `std::array<PieceState, 22>`. Zajętość pozycji i stos
można znaleźć przez liniowe przejście po aktywnych figurach; przy 22 elementach jest to mały,
przewidywalny koszt bez alokacji. Jeśli benchmark kiedyś wykaże potrzebę indeksu pozycji, może on
być pochodnym cache'em, a nie główną reprezentacją.

Docelowy wybór:

- `std::array<PieceState, 22>` dla wszystkich figur bazowej gry,
- stały `PieceId` będący indeksem w tablicy,
- pole aktywności/położenia i wysokości stosu w `PieceState`,
- liniowe zapytania po maksymalnie 22 elementach,
- opcjonalny, odbudowywalny cache granicy ula.

Najpierw trzeba jednak benchmarkować. Maksymalnie 22 bazowe figury oznaczają, że różnica między
kontenerami może być mniejsza niż koszt obecnych alokacji, kopii i generowania zbiorów.

### Typy domenowe

W kodzie rozpoczęto dodawanie `PieceType` i `PieceColor`, ale reszta systemu nadal używa znaków.
Potrzebne są spójne:

- `PlayerColor`,
- `PieceType`,
- `PieceId`,
- `GameResult`,
- `ActionType`,
- `PlyCount`.

Konwersja na znak lub tekst powinna być tylko w serializerze, logach i GUI.

## Wydajność

### Potwierdzone zbędne kopie

- Alpha-beta kopiuje `avaliableActions` i `emptyTiles` w każdym węźle drzewa.
- `RandomAIAlgorithm` kopiuje te same kolekcje; część kopii jest nieużywana.
- `Board::getTile` zwraca `Tile` przez wartość, razem ze `std::string`.
- `Board::getTiles` przebudowuje i kopiuje wszystkie figury do `vector<list<Tile>>` przy
  odświeżeniu GUI.
- `Board::getPlayerTiles` za każdym razem buduje nowy `set`.
- `Board::getNeighbours` alokuje i buduje `set`, mimo że są zawsze dokładnie 6 sąsiadów.
- GUI kopiuje tekstury z globalnego `ResourceManager` do `BoardDrawable`.
- `PieceSelector::draw` kopiuje `sf::Sprite` dla zaznaczonej figury.

### Kontenery w gorących ścieżkach

Generowanie ruchów, DFS mrówki, BFS pająka i punkty artykulacji intensywnie używają drzewiastych
`map`/`set`. Przy tak małych kolekcjach często lepsze będą:

- `std::array<Position, 6>` dla sąsiadów,
- mały `std::vector` i liniowe sprawdzanie dla najwyżej kilku elementów,
- płaski zbiór lub hash set dla większych obszarów osiągalnych przez mrówkę,
- indeksy wektorowe w algorytmie punktów artykulacji.

### Teksty i regexy

Stringi nie są głównym kosztem reguł gry, ale stają się kosztowne w AI, jeżeli każdy symulowany
ruch jest serializowany. `Game::applyValidAction`, używane przez alpha-beta, obecnie nie zapisuje
tekstu i to jest dobry kierunek. Docelowo:

- search pracuje wyłącznie na `Action` i `UndoRecord`,
- tekst jest tworzony dopiero dla zaakceptowanego ruchu użytkownika, zapisu lub sieci,
- notacja figury nie musi być przechowywana jako osobny `std::string` w każdym stanie search.

### Najważniejsza zmiana dla AI

Wprowadzić:

```cpp
UndoRecord GameState::apply(Action action);
void GameState::undo(const UndoRecord& undo);
```

`UndoRecord` powinien zawierać wyłącznie zmienione pola: przesuniętą figurę, wcześniejsze pozycje,
liczniki, turę i wynik. Dzięki temu AI nie musi kopiować `availableActions` ani `emptyTiles`.
Kolejny etap to stabilny hash stanu i transposition table.

### Brak benchmarku

Szybkie testy jednostkowe nie mierzą drzewa wyszukiwania. Przed zmianą reprezentacji potrzebne są
powtarzalne benchmarki:

- liczba wygenerowanych stanów na sekundę,
- czas `generateLegalActions`,
- czas `apply/undo`,
- alokacje na węzeł,
- perft dla ustalonych pozycji i głębokości.

## Reguły ruchu wymagające dodatkowych testów

Implementacja pająka używa jednego globalnego zbioru `visited` dla całego BFS. Legalność dojścia
do pola po dokładnie trzech krokach zależy od konkretnej ścieżki, więc odrzucenie pola odwiedzonego
wcześniej na innej głębokości lub inną drogą może pominąć legalny ruch. Potrzebne są testy
ścieżek zbieżnych i zakazu cofania w obrębie pojedynczej ścieżki.

Poza tym należy dopisać testy:

- cofnięcia postawienia białej i czarnej hetmanki,
- odrzuconej akcji i niezmienionej historii,
- stanu z chrząszczem na stosie,
- `WAIT` i jego cofnięcia,
- obustronnego otoczenia hetmanek,
- limitu półruchów,
- pełnego resetu i ładowania stanu,
- niepoprawnej notacji.

## GUI: skalowanie i tryb lokalny z AI

SFML potrafi skalować widok. Problem wynika z obecnej implementacji, nie z fundamentalnego braku
tej możliwości w bibliotece:

- centrum planszy jest wpisane na sztywno jako `800 x 600`,
- `PieceSelector` i przycisk mają stałe piksele,
- brak obsługi `sf::Event::Resized`,
- layout nie jest liczony z aktualnego rozmiaru okna,
- skala sprite'a jest stała (`0.3f`),
- obszar planszy i interfejs używają widoków, ale bez spójnego systemu layoutu.

Rozwiązanie:

1. Oddzielić `boardView` od `uiView`.
2. Przy `Resized` ustawić logiczny viewport i przeliczyć layout paneli.
3. Pozycję heksów liczyć względem `(0, 0)` świata, nie środka okna.
4. Kamerę planszy centrować niezależnie i opcjonalnie dopasować do bounding box ula.
5. Skalować sprite do rozmiaru heksa na podstawie wymiaru tekstury.
6. Dla ostrego obrazu używać grafik w odpowiedniej rozdzielczości; UI może używać 9-slice lub
   prostych kształtów SFML.

### Człowiek kontra AI bez socketów

Tryb lokalny powinien być adapterem kontrolerów graczy:

```cpp
class PlayerController {
public:
    virtual ~PlayerController() = default;
    virtual void update(GameSession&) = 0;
};

class HumanController final : public PlayerController {};
class AiController final : public PlayerController {};
class RemoteController final : public PlayerController {};
```

Menu wybiera:

- człowiek kontra człowiek lokalnie,
- człowiek kontra AI,
- AI kontra AI,
- opcjonalnie grę sieciową.

Każdy tryb używa tego samego `GameSession`. Socket pozostaje wyłącznie implementacją
`RemoteController`, a nie warunkiem użycia AI.

## Łatwe dodawanie nowych figur

Obecnie nowy typ trzeba dopisać ręcznie w wielu miejscach: liczniki gracza, listy GUI, ładowanie
tekstur, parser, generator i heurystyki. To sprzyja pominięciom.

Docelowo jedna tabela `PieceDefinition` powinna opisywać:

- typ i nazwę,
- liczbę początkową,
- identyfikator grafiki,
- sposób generowania ruchów,
- opcjonalne dane dla notacji i heurystyki.

Nie wszystko powinno być całkowicie „data-driven”: unikalne reguły ruchu nadal wymagają kodu.
Można jednak scentralizować rejestrację i użyć jednego z dwóch modeli:

- `MovementKind` plus `switch` w jednym `MoveGenerator` — prostszy i zwykle najlepszy,
- strategia/funkcja generująca przypisana w definicji — elastyczniejsza dla rozszerzeń.

Warto od razu przewidzieć oficjalne dodatki Mosquito, Ladybug i Pillbug, nawet jeśli nie zostaną
zaimplementowane w pierwszym etapie.

## CMake i higiena kodu

- `global_compile_options` jest tworzone po `add_subdirectory` i nie jest podłączone do targetów,
  więc deklarowane ostrzeżenia i `-Werror` najpewniej nie obowiązują projektu.
- Minimalna wersja CMake 3.5 jest zbyt stara jak na nowoczesny projekt C++20 i targetowe podejście.
- Linker zgłasza duplikaty bibliotek podczas budowania części targetów.
- Publiczne nagłówki zawierają `using namespace hive`.
- W kodzie produkcyjnym jest bardzo dużo `cout`/`cerr`, również w konstruktorach `Tile`,
  renderowaniu i parserze.
- Występują literówki w API (`avaliableActions`, `apllyOpponentAction`, `fredomToMove`,
  `AlphaBetha`, `QueenSafty`, `Oponent`).
- Część pól, które powinny być prywatne, jest publiczna.
- Puste ręcznie napisane destruktory i zbędne include'y zwiększają szum.

Cel: targety `hive_engine`, `hive_ai`, `hive_gui`, `hive_app`, ostrzeżenia ustawione targetowo,
Clang-Tidy/format jako opcjonalne narzędzia i brak zależności silnika od SFML.

## Proponowana architektura docelowa

```text
app/menu
  |
  v
GameSession -------- HumanController
  |                  AiController
  |                  RemoteController
  v
Game (reguły i przebieg)
  |
  +-- GameState
  +-- MoveGenerator
  +-- GameRules
  +-- apply(Action) -> UndoRecord
  +-- undo(UndoRecord)

Warstwy brzegowe:
  SFML renderer <--- read-only snapshot/state
  notation codec <-> Action
  network codec  <-> Action/session events
  replay codec   <-> Action list
```

Silnik nie zna okna, tekstur, socketów ani tekstowej notacji. GUI nie modyfikuje publicznych pól
planszy. AI używa dokładnie tego samego API akcji co człowiek.

## Plan migracji

### Etap 1 — zabezpieczenie zachowania

- usunąć logowanie z hot pathów,
- dodać wymienione testy regresyjne,
- naprawić historię nieprawidłowej akcji i undo hetmanki,
- dodać `GameResult` oraz konfigurowalny limit półruchów,
- uruchomić ostrzeżenia kompilatora i sanitizery w profilu developerskim.

### Etap 2 — typy i własność

- zastąpić graczy obiektami lub `array`,
- dokończyć migrację `char` na enumy,
- dodać `PieceId` i nazwane struktury zamiast par,
- ukryć stan za const API.

### Etap 3 — szybkie `apply/undo`

- wprowadzić `UndoRecord`,
- przestać kopiować akcje i `emptyTiles`,
- rozdzielić serializer od wykonania ruchu,
- dodać perft i benchmark.

### Etap 4 — plansza i generator

- zmienić sąsiadów na `array<Position, 6>`,
- naprawić/zweryfikować generator pająka,
- ocenić `unordered_map`/flat containers na podstawie benchmarku,
- wydzielić `MoveGenerator` i `GameRules`.

### Etap 5 — aplikacja i GUI

- jedna pętla `GameSession`,
- menu wyboru lokalnej gry z AI,
- kontrolery Human/AI/Remote,
- responsywne dwa widoki SFML,
- nowy system assetów.

### Etap 6 — rozszerzalność i AI

- centralny katalog `PieceDefinition`,
- nowe figury,
- move ordering, hash stanu i transposition table,
- dopiero wtedy dalsze mikrooptymalizacje.

## Definicja sukcesu

Projekt jest w dobrym stanie, gdy:

- żadna warstwa poza silnikiem nie może przypadkiem zmienić stanu gry,
- każdy zaakceptowany ruch można dokładnie cofnąć bez kopii całego stanu,
- limit partii i wszystkie wyniki są reprezentowane typem, nie stringiem,
- lokalna gra z AI uruchamia się z menu bez serwera,
- resize okna nie psuje layoutu,
- dodanie figury wymaga jednej definicji oraz implementacji jej unikalnej reguły,
- perft wykrywa regresje poprawności, a benchmark pokazuje koszt zmian,
- kod kompiluje się bez ostrzeżeń i nie posiada ręcznego `new/delete` w modelu gry.
