#pragma once
#include <SFML/Graphics.hpp>
#include <memory>   
#include <chrono>

#include "BoardDrawable.hpp"
#include "PieceSelector.hpp"
#include "Game.hpp"
#include "Button.hpp"
#include "BaseInterface.hpp"

using namespace hive;

/*
class GameController is responsible for handling the user input and updating the GUI.


    virtual ~BaseInterface() = default;

    virtual void processEvents() = 0;
    virtual void render() = 0;
    virtual void handleMouseButtonPressed(sf::Mouse::Button button) = 0;
    virtual void handleMouseButtonReleased(sf::Mouse::Button button) = 0;
    void handleGameControl();
    void update();

protected:
    void handleMouseMoved();
    void handleMouseWheelScroll(float delta);
    void handleWindowClose();
    void updateTurnText();

    Position convertMouseToBoardPos(sf::Vector2f mousePos);

    std::chrono::high_resolution_clock::time_point clickStartTime;
    sf::Text turnText;
    sf::RenderWindow &window;
    sf::View boardView;
    std::unique_ptr<Game> game;
    BoardDrawable boardDrawable;
    bool dragging = false;
    bool change = true;
    sf::Vector2f lastMousePos;
};
*/
class GameController : public BaseInterface
{   
public:
    GameController(sf::RenderWindow &window);
    void apllyOpponentAction(const std::string& action);
    void setPlayer(char player);
    std::string message = "";

    void processEvents() override;
    void render() override;
    void handleMouseButtonPressed(sf::Mouse::Button button) override;
    void handleMouseButtonReleased(sf::Mouse::Button button) override;

protected:

    
    void handleMouseClick();
    bool isMyTurn();
    void handleWaitButtonClick();
    void handlePieceSelectorClick(const sf::Vector2f &mousePos);
    void handleBoardClick(sf::Vector2f mousePos);
    void handlePiecePlacement(const Position &boardPos);
    void handlePieceMovement(const Position &boardPos);

    PieceSelector pieceSelector;
    sf::Vector2f dragStartPos;
    char player = 'W';
    Button waitButton;

};