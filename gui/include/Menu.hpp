#pragma once
#include <SFML/Graphics.hpp>

class Menu
{
public:
    Menu(sf::Font &font)
    {

        pvpButton.setFont(font);
        pvpButton.setString("Player vs Player");
        pvpButton.setCharacterSize(30);

        sf::FloatRect textBounds = pvpButton.getLocalBounds();

        pvpButtonBackground.setSize(sf::Vector2f(textBounds.width + 20, textBounds.height + 20));
        pvpButtonBackground.setFillColor(sf::Color::Blue);
        pvpButtonBackground.setPosition(300, 250);

        pvpButton.setPosition(
            pvpButtonBackground.getPosition().x + 10,
            pvpButtonBackground.getPosition().y + 10);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(pvpButtonBackground);
        window.draw(pvpButton);
    }

    bool isPvPClicked(sf::Vector2f mousePos)
    {
        return pvpButtonBackground.getGlobalBounds().contains(mousePos);
    }

private:
    sf::Text pvpButton;
    sf::RectangleShape pvpButtonBackground;
};
