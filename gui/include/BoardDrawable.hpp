#pragma once
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "HexDrawable.hpp"
#include <cmath>

class BoardDrawable : public sf::Drawable
{
public:
    BoardDrawable(const hive::Board &board, float hexSize);
    void loadResources();

private:
    const hive::Board &board;
    float hexSize;
    sf::Font font;
    std::map<std::string, sf::Texture> textures;
    sf::Text getPositionText(int x, int y) const;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
