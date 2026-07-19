#pragma once
#include <SFML/Graphics.hpp>

#include "BaseInterface.hpp"
#include "Button.hpp"
#include "PieceSelector.hpp"

using namespace hive;

class GameController : public BaseInterface
{
  public:
    GameController(sf::RenderWindow &window);
    void apllyOpponentAction(const std::string &action);
    bool applyLocalAction(const Action &action);
    void setPlayer(hive::PlayerColor player);

    void setLocalHotseat(bool enabled) { localHotseat = enabled; }

    Game &getGame() { return *game; }

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
    PlayerColor player = PlayerColor::White;
    Button waitButton;
    bool localHotseat = false;
};
