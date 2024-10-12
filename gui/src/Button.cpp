#include "Button.hpp"

Button::Button(const sf::Vector2f &size, const sf::Vector2f &position, const std::string &text, const sf::Font &font)
{
    buttonShape.setSize(size);
    buttonShape.setPosition(position);
    buttonShape.setOutlineThickness(2);
    buttonShape.setOutlineColor(sf::Color::Black);

    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::Black);
    buttonText.setPosition(
        position.x + (size.x - buttonText.getLocalBounds().width) / 2,
        position.y + (size.y - buttonText.getLocalBounds().height) / 2
    );
}

void Button::setPosition(const sf::Vector2f &position)
{
    buttonShape.setPosition(position);
    buttonText.setPosition(
        position.x + (buttonShape.getSize().x - buttonText.getLocalBounds().width) / 2,
        position.y + (buttonShape.getSize().y - buttonText.getLocalBounds().height) / 2
    );
}

void Button::setText(const std::string &text)
{
    buttonText.setString(text);
    buttonText.setPosition(
        buttonShape.getPosition().x + (buttonShape.getSize().x - buttonText.getLocalBounds().width) / 2,
        buttonShape.getPosition().y + (buttonShape.getSize().y - buttonText.getLocalBounds().height) / 2
    );
}

void Button::setFillColor(const sf::Color &color)
{
    buttonShape.setFillColor(color);
}

void Button::setTextColor(const sf::Color &color)
{
    buttonText.setFillColor(color);
}

bool Button::contains(const sf::Vector2f &point) const
{
    return buttonShape.getGlobalBounds().contains(point);
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(buttonShape, states);
    target.draw(buttonText, states);
}