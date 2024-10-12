#pragma once
#include <SFML/Graphics.hpp>
#include <memory>   
#include <chrono>

#include "Menu.hpp"
#include "BoardDrawable.hpp"
#include "PieceSelector.hpp"
#include "Game.hpp"

using namespace hive;

class GameWindow
{
public:
    GameWindow();

    void run();

private:
    void processEvents();
    void handleMouseMoved();
    void handleMouseButtonPressed(sf::Mouse::Button button);
    void handleMouseButtonReleased(sf::Mouse::Button button);
    void handleWindowClose();
    void update();
    void render();
    void handleMouseClick();
    void handleMouseWheelScroll(float delta);

    void handlePieceSelectorClick(const sf::Vector2f &mousePos);
    void handleBoardClick(sf::Vector2f mousePos);
    void handlePiecePlacement(const Position &boardPos);
    void handlePieceMovement(const Position &boardPos);

    Position convertMouseToBoardPos(sf::Vector2f mousePos);

    sf::RenderWindow window;
    sf::View boardView;
    sf::Font font;
    PieceSelector pieceSelector;
    std::unique_ptr<Game> game;
    BoardDrawable boardDrawable;
    std::chrono::high_resolution_clock::time_point clickStartTime;
    std::string player = "";

    bool dragging = false;
    bool change = true;
    sf::Vector2f dragStartPos;
    sf::Vector2f lastMousePos;
};