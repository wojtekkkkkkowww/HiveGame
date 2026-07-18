#pragma once
#include "BoardDrawable.hpp"
#include "Game.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <memory>

using namespace hive;

class BaseInterface
{
  public:
    BaseInterface(sf::RenderWindow &window);

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
    void handleWindowResized(unsigned int width, unsigned int height);
    void updateTurnText();

    Position convertMouseToBoardPos(sf::Vector2f mousePos);

    sf::RenderWindow &window;
    std::chrono::high_resolution_clock::time_point clickStartTime;
    sf::Text turnText;
    sf::View boardView;
    std::unique_ptr<Game> game;
    BoardDrawable boardDrawable;
    bool dragging = false;
    bool change = true;
    sf::Vector2f lastMousePos;
};
