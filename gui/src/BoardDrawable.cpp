#include "BoardDrawable.hpp"
#include "ResourceManager.hpp"
#include <iostream>

BoardDrawable::BoardDrawable(const hive::Board &board, float hexSize)
    : board(board), hexSize(hexSize)
{
    loadResources();
    update();
}

BoardDrawable::~BoardDrawable() {}

void BoardDrawable::loadResources()
{
    ResourceManager &resourceManager = ResourceManager::getInstance();
    std::vector<char> tileNames = {'A', 'B', 'G', 'Q', 'S'};
    for (const auto &name : tileNames)
    {
        textures[name] = resourceManager.getTexture(name);
    }
    font = resourceManager.getFont("arial.ttf");
}

void BoardDrawable::update()
{
    hexDrawables.clear();

    if (!board.boardTiles.empty()) // segfaults
    {
        updateBoardTiles();
    }

    updateEmptyTiles();
}

void BoardDrawable::updateBoardTiles()
{
    for (const auto &[position, tiles] : board.boardTiles)
    {
        float offset = 0.0f;
        for (auto it = tiles.begin(); it != tiles.end(); ++it)
        {
            const auto &tile = *it;
            HexDrawable hex(0.95f * hexSize);
            hex.tilePosition = position;
            hex.setOffset(offset);
            hex.setTile(tile, textures);

            if (position == selectedPosition && tile.color == player)
            {
                // Check if the next tile has the same position
                auto nextIt = std::next(it);
                if (nextIt == tiles.end() || nextIt->position != position)
                {
                    hex.highlight(sf::Color::Cyan);
                }
            }

            auto [posX, posY] = calculateHexPosition(position.x, position.y);
            hex.setPosition(posX, posY + offset);

            hexDrawables.push_back(hex);
            offset += 5.0f;
        }
    }
}

void BoardDrawable::updateEmptyTiles()
{
    for (const auto &emptyPosition : board.emptyTiles)
    {
        HexDrawable hex(0.95f * hexSize);
        hex.tilePosition = emptyPosition;

        auto [posX, posY] = calculateHexPosition(emptyPosition.x, emptyPosition.y);
        hex.setPosition(posX, posY);
        hex.highlight(sf::Color(192, 192, 192));
        hexDrawables.push_back(hex);
    }
}

void BoardDrawable::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (const auto &hex : hexDrawables)
    {
        target.draw(hex, states);

        sf::Text text = getPositionText(hex.tilePosition.x, hex.tilePosition.y);
        sf::Vector2f pos = hex.getPosition();
        text.setPosition(pos.x - 10, pos.y - 10);
        target.draw(text, states);
    }
}

std::pair<float, float> BoardDrawable::calculateHexPosition(int x, int y)
{
    float sqrt_3 = std::sqrt(3.0f);
    float centerX = 800.f / 2.0f;
    float centerY = 600.f / 2.0f;

    float posX = hexSize * (1.5f * x) + centerX;
    float posY = hexSize * (sqrt_3 * (y + 0.5f * x)) + centerY;

    return std::make_pair(posX, posY);
}

sf::Text BoardDrawable::getPositionText(int x, int y) const
{
    sf::Text text;
    text.setFont(font);
    text.setString(std::to_string(x) + "," + std::to_string(y));
    text.setCharacterSize(10);
    text.setFillColor(sf::Color::Red);
    return text;
}
