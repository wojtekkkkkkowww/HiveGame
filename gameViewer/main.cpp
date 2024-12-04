#include "GameViewer.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
using namespace hive;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: GameViewer <game_file>" << std::endl;
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "Game Viewer");
    GameViewer gameViewer(window, argv[1]);

    const sf::Time frameTime = sf::seconds(1.f / 20.f);
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        if (elapsed < frameTime)
        {
            sf::sleep(frameTime - elapsed);
        }

        gameViewer.handleGameControl();
    }

    return 0;
}