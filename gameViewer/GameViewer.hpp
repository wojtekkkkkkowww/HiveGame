#pragma once
#include "BaseInterface.hpp"
#include <vector>
#include <string>

namespace hive {

class GameViewer : public BaseInterface {
public:
    GameViewer(sf::RenderWindow& window, const std::string& gameFile);

    void processEvents() override;
    void render() override;
    void handleMouseButtonPressed(sf::Mouse::Button button) override;
    void handleMouseButtonReleased(sf::Mouse::Button button) override;

private:
    void handleNextMove();
    void handlePreviousMove();

    std::vector<std::string> moves;
    size_t currentMoveIndex = 0;
};

}
