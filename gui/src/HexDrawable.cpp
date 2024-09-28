#include "HexDrawable.hpp"

HexDrawable::HexDrawable(float size)
{
    hex.setRadius(size);
    hex.setPointCount(6);
    hex.setOrigin(size, size);
    hex.setFillColor(sf::Color::White);
    hex.setRotation(30.f);
    hex.setOutlineThickness(1.f);
    hex.setOutlineColor(sf::Color::Red);
}

void HexDrawable::setPosition(float x, float y)
{
    hex.setPosition(x, y);
    sprite.setPosition(x, y);
}

void HexDrawable::setTile(const hive::Tile &tile, const std::map<std::string, sf::Texture> &textures)
{
    if(tile.color == "WHITE")
    {
        hex.setFillColor(sf::Color::White);
    }
    else
    {
        hex.setFillColor(sf::Color::Black);
    }

    sprite.setTexture(textures.at(tile.type));
    sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);
    sprite.setScale(0.3f, 0.3f);
}

void HexDrawable::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(hex, states);
    target.draw(sprite, states);
}
