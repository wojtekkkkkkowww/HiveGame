#include "AlphaBethaSearch.hpp"
#include "Button.hpp"
#include "GameController.hpp"
#include "ResourceManager.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

namespace
{
enum class LocalMode
{
    Menu,
    HumanVsHuman,
    HumanVsAi
};
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 700), "Hive",
                            sf::Style::Default, sf::ContextSettings(0, 0, 8));
    window.setFramerateLimit(60);

    const auto &font = ResourceManager::getInstance().getFont("arial.ttf");
    Button humanButton({300.0f, 64.0f}, {350.0f, 270.0f}, "Human vs Human", font);
    Button aiButton({300.0f, 64.0f}, {350.0f, 360.0f}, "Human vs AI", font);

    LocalMode mode = LocalMode::Menu;
    std::unique_ptr<GameController> controller;
    std::unique_ptr<hive::AlphaBetaAI> ai;

    while (window.isOpen())
    {
        if (mode == LocalMode::Menu)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                if (event.type == sf::Event::MouseButtonReleased &&
                    event.mouseButton.button == sf::Mouse::Left)
                {
                    const auto mouse =
                        window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                    if (humanButton.contains(mouse))
                    {
                        mode = LocalMode::HumanVsHuman;
                    }
                    else if (aiButton.contains(mouse))
                    {
                        mode = LocalMode::HumanVsAi;
                    }

                    if (mode != LocalMode::Menu)
                    {
                        controller = std::make_unique<GameController>(window);
                        controller->setPlayer('W');
                        controller->setLocalHotseat(mode == LocalMode::HumanVsHuman);
                        if (mode == LocalMode::HumanVsAi)
                        {
                            ai = std::make_unique<hive::AlphaBetaAI>(controller->getGame());
                        }
                    }
                }
            }

            window.clear(sf::Color(245, 241, 230));
            window.draw(humanButton);
            window.draw(aiButton);
            window.display();
            continue;
        }

        controller->handleGameControl();
        auto &game = controller->getGame();
        if (mode == LocalMode::HumanVsAi && !game.isGameOver() && game.getCurrentTurn() == 'B')
        {
            controller->applyLocalAction(ai->getNextMove());
        }
    }
}
