#include "BoardDrawable.hpp"
#include <iostream>

BoardDrawable::BoardDrawable(const hive::Board &board, float hexSize)
    : board(board), hexSize(hexSize)
{
    loadResources();
    std::cerr << "BoardDrawable created" << std::endl;
}

void BoardDrawable::loadResources()
{
    std::vector<std::string> tileNames = {"ANT", "BEETLE", "GRASSHOPPER", "QUEEN", "SPIDER"};
    for (const auto &name : tileNames)
    {
        sf::Texture texture;
  
        if (!texture.loadFromFile(std::string(SOURCE_DIR) + "/res/" + name + ".png"))
        {
            std::cerr << "Error loading texture: " << name << std::endl;
            continue;
        }
        std::cerr << "Loaded texture: " << name << std::endl;
        textures[name] = texture;
    }

    if (!font.loadFromFile(std::string(SOURCE_DIR) + "/res/arial.ttf"))
    {
        std::cerr << "Error loading font: arial.ttf" << std::endl;
    }
    else
    {
        std::cerr << "Loaded font: arial.ttf" << std::endl;
    }

    std::cerr << "Loaded all resources" << std::endl;
}

void BoardDrawable::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Vector2u windowSize = target.getSize();
    float centerX = windowSize.x / 2.0f;
    float centerY = windowSize.y / 2.0f;

    float sqrt_3 = sqrt(3);
    auto calculatePosition = [this, sqrt_3, centerX, centerY](int x, int y)
    {
        float posX = hexSize * (1.5f * x) + centerX;
        float posY = hexSize * (sqrt_3 * (y + 0.5f * x)) + centerY;
        return std::make_pair(posX, posY);
    };

    std::cerr << "Drawing tiles" << std::endl;
    for (const auto &[position, tiles] : board.boardTiles)
    {
        float offset = 0.0f;
        auto [posX, posY] = calculatePosition(position.x, position.y);

        for (const auto &tile : tiles)
        {
            HexDrawable hex(0.95f * hexSize);
            hex.setPosition(posX, posY + offset);
            hex.setTile(tile, textures);
            target.draw(hex, states);
            offset += 5.0f;
        }

        sf::Text text = getPositionText(position.x, position.y);
        text.setPosition(posX - 10, posY - 10);
        target.draw(text, states);
    }
    std::cerr << "Drawing empty tiles" << std::endl;

    for (const auto &emptyPosition : board.emptyTiles)
    {
        HexDrawable hex(0.95f * hexSize);
        auto [posX, posY] = calculatePosition(emptyPosition.x, emptyPosition.y);
        hex.setPosition(posX, posY);
        target.draw(hex, states);
        sf::Text text = getPositionText(emptyPosition.x, emptyPosition.y);
        text.setPosition(posX - 10, posY - 10);
        target.draw(text, states);
    }
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
