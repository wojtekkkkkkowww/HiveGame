#include "BoardDrawable.hpp"
#include "PieceCatalog.hpp"
#include "ResourceManager.hpp"
#include <numbers>

BoardDrawable::BoardDrawable(hive::Board &board, float hexSize) : board(board), hexSize(hexSize)
{
    loadResources();
    update();
}

BoardDrawable::~BoardDrawable() {}

void BoardDrawable::loadResources()
{
    ResourceManager &resourceManager = ResourceManager::getInstance();
    pieceAtlas = &resourceManager.getPieceAtlas();
    font = resourceManager.getFont("arial.ttf");
}

void BoardDrawable::update()
{
    hexDrawables.clear();
    updateEmptyTiles();
    if (board.getTileCount() != 0)
    {
        updateBoardTiles();
    }
}

void BoardDrawable::updateBoardTiles()
{
    const auto &tiles = board.getTiles();
    hive::Position lastPosition = hive::invalidPosition;
    float offset = 0.0f;

    for (size_t level = 0; level < tiles.size(); level++)
    {
        for (const auto &tile : tiles[level])
        {
            addHex(lastPosition, tile, offset, level);
        }
        offset += 5.0f;
    }
}

void BoardDrawable::addHex(hive::Position &lastPosition, const hive::Tile &tile, float offset,
                           size_t level)
{
    lastPosition = tile.position;
    HexDrawable hex(0.95f * hexSize);
    hex.tilePosition = lastPosition;
    hex.setOffset(offset);
    hex.setTile(tile, *pieceAtlas);

    if (lastPosition == selectedPosition && tile.color == player)
    {
        if (board.getLevel(lastPosition) == static_cast<int>(level) + 1)
        {
            hex.highlight(sf::Color(54, 235, 207));
            hex.setOutlineThickness(5.0f);
        }
    }

    auto [posX, posY] = calculateHexPosition(lastPosition.x, lastPosition.y);
    hex.setPosition(posX, posY + offset);

    hexDrawables.push_back(hex);
}

void BoardDrawable::updateEmptyTiles()
{
    for (const auto &emptyPosition : board.emptyTiles)
    {
        HexDrawable hex(0.95f * hexSize);
        hex.tilePosition = emptyPosition;

        auto [posX, posY] = calculateHexPosition(emptyPosition.x, emptyPosition.y);
        hex.setPosition(posX, posY);
        hex.setFillColor(sf::Color(68, 65, 60));
        hex.highlight(sf::Color(246, 184, 66, 145));
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
        // target.draw(text, states);
    }
}

std::pair<float, float> BoardDrawable::calculateHexPosition(int x, int y)
{
    float centerX = viewportSize.x / 2.0f;
    float centerY = viewportSize.y / 2.0f;
    float height = std::numbers::sqrt3_v<float> * hexSize / 2;

    float posX = centerX + height * (2 * x + y) + x;
    float posY = centerY + hexSize * 1.5f * y + y;

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
