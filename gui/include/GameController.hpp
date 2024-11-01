#pragma once
#include <SFML/Graphics.hpp>
#include <memory>   
#include <chrono>

#include "BoardDrawable.hpp"
#include "PieceSelector.hpp"
#include "Game.hpp"
#include "Button.hpp"

using namespace hive;

/*
class GameController is responsible for handling the user input and updating the GUI.
*/
class GameController 
{   
public:
    GameController(sf::RenderWindow &window);
    virtual void handleGameControl();
    void apllyOpponentAction(const std::string& action);
    void setPlayer(char player);
    std::string message = "";


protected:
    virtual void processEvents();
    virtual void update();
    virtual void render();
    virtual void handleMouseMoved();
    virtual void handleMouseButtonPressed(sf::Mouse::Button button);
    virtual void handleMouseButtonReleased(sf::Mouse::Button button);
    virtual void handleMouseWheelScroll(float delta);
    void handleMouseClick();

    void updateTurnText();
    bool isMyTurn();
    void handleWindowClose();
    void handleWaitButtonClick();
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
    char player = 'W';
    sf::Text turnText;
    Button waitButton;
};