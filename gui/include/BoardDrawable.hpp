#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <list>
#include <memory>

#include "Game.hpp"
#include "HexDrawable.hpp"

class BoardDrawable : public sf::Drawable
{
public:
    BoardDrawable(const hive::Board &board, float hexSize);
    ~BoardDrawable();
    void loadResources();
    void update();
    void updateEmptyTiles();
    void updateBoardTiles();
    std::list<HexDrawable> hexDrawables;
    hive::Position selectedPosition = hive::invalidPosition;


private:
    const hive::Board &board;
    float hexSize;
    sf::Font font;
    std::map<std::string, sf::Texture> textures;
    sf::Text getPositionText(int x, int y) const;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    static constexpr float sqrt_3 = sqrt(3);
    std::pair<float, float>calculateHexPosition(int x, int y);
};
