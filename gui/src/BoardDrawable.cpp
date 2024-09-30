#include "BoardDrawable.hpp"
#include <iostream>

BoardDrawable::BoardDrawable(const hive::Board &board, float hexSize)
    : board(board), hexSize(hexSize)
{
    loadTextures();
}

void BoardDrawable::loadTextures()
{
    std::vector<std::string> tileNames = {"ANT", "BEETLE", "GRASSHOPPER", "QUEEN", "SPIDER"};
    for (const auto &name : tileNames)
    {
        sf::Texture texture;
        // wczytywanie z lokalnej ścieżki uwaga
        if (!texture.loadFromFile("/mnt/data/projekty/HiveGame/res/" + name + ".png"))
        {
            std::cerr << "Error loading texture: " << name << std::endl;
            continue;
        }
        std::cerr << "Loaded texture: " << name << std::endl;
        textures[name] = texture;
    }
}


void BoardDrawable::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Vector2u windowSize = target.getSize();
    float centerX = windowSize.x / 2.0f;
    float centerY = windowSize.y / 2.0f;

    float sqrt_3 = sqrt(3);
    auto calculatePosition = [this, sqrt_3, centerX, centerY](int x, int y) {
        float posX = hexSize * (1.5f * x) + centerX;
        float posY = hexSize * (sqrt_3 * (y + 0.5f * x)) + centerY;
        return std::make_pair(posX, posY);
    };

    for (const auto &[position, tiles] : board.boardTiles)
    {
        HexDrawable hex(0.95f*hexSize);
        auto [posX, posY] = calculatePosition(position.x, position.y);
        hex.setPosition(posX, posY);
        hex.setTile(tiles.front(), textures);
        target.draw(hex, states);
    }

    for (const auto &emptyPosition : board.emptyTiles)
    {
        HexDrawable hex(0.95f*hexSize);
        auto [posX, posY] = calculatePosition(emptyPosition.x, emptyPosition.y);
        hex.setPosition(posX, posY);
        target.draw(hex, states);
    }
}
