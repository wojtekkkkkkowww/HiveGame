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

class GameController : public BaseInterface
{
public:
    GameController(sf::RenderWindow &window);
    void apllyOpponentAction(const std::string &action);
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