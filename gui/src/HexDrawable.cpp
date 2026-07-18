#include "HexDrawable.hpp"
#include "PieceCatalog.hpp"

HexDrawable::HexDrawable(float size) : size(size)
{
    hex.setRadius(size);
    hex.setPointCount(6);
    hex.setOrigin(size, size);
    hex.setFillColor(sf::Color::White);
    hex.setOutlineThickness(1.f);
    hex.setOutlineColor(sf::Color::Red);
}

void HexDrawable::setPosition(float x, float y)
{
    hex.setPosition(x, y);
    sprite.setPosition(x, y);
}

void HexDrawable::setTile(const hive::Tile &tile, const sf::Texture &atlas)
{
    if (tile.color == 'W')
    {
        hex.setFillColor(sf::Color::White);
    }
    else
    {
        hex.setFillColor(sf::Color::Black);
    }

    const auto &definition = hive::pieceCatalog[hive::pieceIndex(tile.type)];
    const auto cellWidth = static_cast<int>(atlas.getSize().x / hive::pieceCatalog.size());
    const auto cellHeight = static_cast<int>(atlas.getSize().y);
    sprite.setTexture(atlas);
    sprite.setTextureRect(
        {static_cast<int>(definition.atlasColumn) * cellWidth, 0, cellWidth, cellHeight});
    sprite.setOrigin(static_cast<float>(cellWidth) / 2.0f,
                     static_cast<float>(cellHeight) / 2.0f);
    const float targetSize = size * 1.75f;
    sprite.setScale(targetSize / static_cast<float>(cellWidth),
                    targetSize / static_cast<float>(cellHeight));
}

void HexDrawable::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(hex, states);
    target.draw(sprite, states);
}

bool HexDrawable::contains(const sf::Vector2f &point) const
{
    sf::Vector2f hexPos = hex.getPosition();
    sf::Vector2f localPoint = point - hexPos;

    float distanceSquared = localPoint.x * localPoint.x + localPoint.y * localPoint.y;
    float radiusSquared = size * size * 0.6f;
    return distanceSquared <= radiusSquared;
}
