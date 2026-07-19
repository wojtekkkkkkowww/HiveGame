#pragma once
#include <SFML/Graphics.hpp>
#include <list>

#include "HexDrawable.hpp"
#include "Position.hpp"

/*
 * @brief The BoardDrawable class is responsible for drawing the game board.
 * Class implements the sf::Drawable interface and is used by the GameController class.
 */
class BoardDrawable : public sf::Drawable
{
  public:
    BoardDrawable(hive::Board &board, float hexSize);
    ~BoardDrawable();
    void loadResources();
    void update();
    void updateEmptyTiles();
    void updateBoardTiles();
    void addHex(hive::Position &lastPosition, const hive::Tile &tile, float offset, size_t level);

    void setViewportSize(sf::Vector2f size)
    {
        viewportSize = size;
        update();
    }

    void setPlayer(hive::PlayerColor player) { this->player = player; }

    std::list<HexDrawable> hexDrawables;
    hive::Position selectedPosition = hive::invalidPosition;

  private:
    hive::Board &board;
    float hexSize;
    sf::Vector2f viewportSize{800.0f, 600.0f};
    sf::Font font;
    const sf::Texture *pieceAtlas = nullptr;
    sf::Text getPositionText(int x, int y) const;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    static constexpr float sqrt_3 = 1.73205080757f;
    std::pair<float, float> calculateHexPosition(int x, int y);
    hive::PlayerColor player = hive::PlayerColor::White;
};
