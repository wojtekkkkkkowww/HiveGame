#include "AlphaBethaSearch.hpp"
#include "Button.hpp"
#include "GameController.hpp"
#include "ResourceManager.hpp"
#include <SFML/Graphics.hpp>
#include <array>
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
    sf::RenderWindow window(sf::VideoMode(1000, 700), "Hive", sf::Style::Default,
                            sf::ContextSettings(0, 0, 8));
    window.setFramerateLimit(60);

    const auto &font = ResourceManager::getInstance().getFont("arial.ttf");
    Button humanButton({340.0f, 68.0f}, {330.0f, 365.0f}, "LOCAL: TWO PLAYERS", font);
    humanButton.setFillColor(sf::Color(246, 184, 66));
    humanButton.setTextColor(sf::Color(30, 27, 24));
    Button aiButton({340.0f, 68.0f}, {330.0f, 455.0f}, "PLAY AGAINST AI", font);
    aiButton.setFillColor(sf::Color(52, 194, 178));
    aiButton.setTextColor(sf::Color(18, 31, 31));

    sf::Text title("HIVE", font, 82);
    title.setStyle(sf::Text::Bold);
    title.setFillColor(sf::Color(246, 184, 66));
    title.setOutlineColor(sf::Color(35, 31, 27));
    title.setOutlineThickness(3.0f);
    title.setPosition(390.0f, 110.0f);

    sf::Text subtitle("TACTICAL INSECT STRATEGY", font, 20);
    subtitle.setLetterSpacing(1.8f);
    subtitle.setFillColor(sf::Color(218, 211, 196));
    subtitle.setPosition(343.0f, 220.0f);

    sf::RectangleShape menuPanel({500.0f, 330.0f});
    menuPanel.setPosition(250.0f, 285.0f);
    menuPanel.setFillColor(sf::Color(34, 31, 29, 245));
    menuPanel.setOutlineThickness(2.0f);
    menuPanel.setOutlineColor(sf::Color(94, 83, 67));

    std::array<sf::CircleShape, 8> decoration;
    for (std::size_t index = 0; index < decoration.size(); ++index)
    {
        auto &hex = decoration[index];
        hex.setRadius(42.0f);
        hex.setPointCount(6);
        hex.setOrigin(42.0f, 42.0f);
        hex.setFillColor(sf::Color::Transparent);
        hex.setOutlineThickness(2.0f);
        hex.setOutlineColor(sf::Color(246, 184, 66, 45));
        const float x = index % 2 == 0 ? 100.0f : 900.0f;
        const float y = 95.0f + static_cast<float>(index / 2) * 155.0f;
        hex.setPosition(x, y);
    }

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
                        controller->setPlayer(hive::PlayerColor::White);
                        controller->setLocalHotseat(mode == LocalMode::HumanVsHuman);
                        if (mode == LocalMode::HumanVsAi)
                        {
                            ai = std::make_unique<hive::AlphaBetaAI>(controller->getGame());
                        }
                    }
                }
            }

            window.clear(sf::Color(20, 19, 18));
            for (const auto &hex : decoration)
            {
                window.draw(hex);
            }
            window.draw(menuPanel);
            window.draw(title);
            window.draw(subtitle);
            window.draw(humanButton);
            window.draw(aiButton);
            window.display();
            continue;
        }

        controller->handleGameControl();
        auto &game = controller->getGame();
        if (mode == LocalMode::HumanVsAi && !game.isGameOver() &&
            game.getCurrentTurn() == hive::PlayerColor::Black)
        {
            controller->applyLocalAction(ai->getNextMove());
        }
    }
}
