#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Board.hpp"

class HexDrawable : public sf::Drawable
    {
    public:
        HexDrawable(float size);
        void setPosition(float x, float y);
        sf::Vector2f getPosition() const { return hex.getPosition(); }
        void setTile(const hive::Tile &tile, const std::map<char, sf::Texture> &textures);
        bool contains(const sf::Vector2f &point) const;
        hive::Position tilePosition;
        void setOffset(float offset) { this->offset = offset; }
        float getOffset() const { return offset; }
        void highlight(sf::Color color) { hex.setOutlineColor(color); }

    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
        sf::CircleShape hex;
        sf::Sprite sprite;
        float size;
        float offset = 0.0f;
    };