#pragma once
#include <SFML/Graphics.hpp>
#include <memory>   
#include <chrono>

#include "BoardDrawable.hpp"
#include "PieceSelector.hpp"
#include "Game.hpp"
#include "Button.hpp"

using namespace hive;

class GameController 
{   
public:
    GameController(sf::RenderWindow &window);
    void handleGameControll();
    void apllyOpponentAction(const std::string& action);
    void setPlayer(const std::string& player);
    std::string message = "";


private:
    void processEvents();
    void update();
    void render();
    void updateTurnText();
    bool isMyTurn();
    
    void handleMouseMoved();
    void handleMouseButtonPressed(sf::Mouse::Button button);
    void handleMouseButtonReleased(sf::Mouse::Button button);
    void handleWindowClose();
    void handleMouseClick();
    void handleWaitButtonClick();
    void handleMouseWheelScroll(float delta);
    void handlePieceSelectorClick(const sf::Vector2f &mousePos);
    void handleBoardClick(sf::Vector2f mousePos);
    void handlePiecePlacement(const Position &boardPos);
    void handlePieceMovement(const Position &boardPos);
    Position convertMouseToBoardPos(sf::Vector2f mousePos);

    bool dragging = false;
    bool change = true;
    std::chrono::high_resolution_clock::time_point clickStartTime;
    sf::Vector2f dragStartPos;
    sf::Vector2f lastMousePos;


    sf::RenderWindow &window;
    sf::View boardView;
    PieceSelector pieceSelector;
    std::unique_ptr<Game> game;
    BoardDrawable boardDrawable;
    std::string player = "WHITE";
    sf::Text turnText;
    Button waitButton;
    ActionParser actionParser;


};