#pragma once
#include "GameController.hpp"

class GameViewer : public GameController
{
public:
    GameViewer(sf::RenderWindow &window, const std::string &gameFile);
private:
    void handleMouseButtonPressed(sf::Mouse::Button button) override;
    void handleMouseButtonReleased(sf::Mouse::Button button) override;
    void processEvents() override;
    void render () override;
    void handleNextMove();
    void handlePreviousMove();

    std::vector<std::string> moves;
    size_t currentMoveIndex = 0;
};