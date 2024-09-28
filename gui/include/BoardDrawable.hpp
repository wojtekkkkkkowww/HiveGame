#pragma once
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "HexDrawable.hpp"
#include <cmath>

class BoardDrawable : public sf::Drawable
{
public:
    BoardDrawable(const hive::Board &board, float hexSize);
    void loadTextures();

private:
    const hive::Board &board;
    float hexSize;
    std::map<std::string, sf::Texture> textures;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
