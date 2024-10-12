#pragma once
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable
{
public:
    Button(const sf::Vector2f &size, const sf::Vector2f &position, const std::string &text, const sf::Font &font);
    Button() = default;
    void setPosition(const sf::Vector2f &position);
    void setText(const std::string &text);
    void setFillColor(const sf::Color &color);
    void setTextColor(const sf::Color &color);

    bool contains(const sf::Vector2f &point) const;

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::RectangleShape buttonShape;
    sf::Text buttonText;
};