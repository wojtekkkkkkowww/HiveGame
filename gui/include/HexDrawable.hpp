#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Board.hpp"

class HexDrawable : public sf::Drawable
    {
    public:
        HexDrawable(float size);
        void setPosition(float x, float y);
        void setTile(const hive::Tile &tile, const std::map<std::string, sf::Texture> &textures);

    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
        sf::CircleShape hex;
        sf::Sprite sprite;
    };