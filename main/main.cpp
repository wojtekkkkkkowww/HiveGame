#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "GameWindow.hpp"

int main()
{
    // sf::RenderWindow menuWindow(sf::VideoMode(800, 600), "Main Menu");
    

    // Menu menu(font);

    // while (menuWindow.isOpen())
    // {
    //     sf::Event event;
    //     while (menuWindow.pollEvent(event))
    //     {
    //         if (event.type == sf::Event::Closed)
    //             menuWindow.close();

    //         if (event.type == sf::Event::MouseButtonPressed)
    //         {
    //             if (menu.isPvPClicked(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
    //             {
    // menuWindow.close();
    GameWindow gameWindow;
    gameWindow.run();
    //             }
    //         }
    //     }

    //     menuWindow.clear();
    //     menu.draw(menuWindow);
    //     menuWindow.display();
    // }

    return 0;
}