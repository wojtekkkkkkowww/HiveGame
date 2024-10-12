#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "ResourceManager.hpp"

class PieceSelector : public sf::Drawable
{
public:
    PieceSelector(float windowWidth, float windowHeight)
        : windowHeight(windowHeight)
    {
        stackRect = sf::RectangleShape(sf::Vector2f(windowWidth, 150));
        stackRect.setFillColor(sf::Color::White);
        stackRect.setOutlineColor(sf::Color::Black);
        stackRect.setOutlineThickness(2);
        stackRect.setPosition(0, windowHeight - 150);

        loadPieces();
    }

    void loadPieces()
    {
        ResourceManager &resourceManager = ResourceManager::getInstance();
        std::vector<std::string> pieceTypes = {"ANT", "BEETLE", "GRASSHOPPER", "QUEEN", "SPIDER"};
        float xOffset = 10.0f;
        float yOffset = 10.0f;
        float pieceSpacing = 60.0f;

        for (const auto &type : pieceTypes)
        {
            const sf::Texture &texture = resourceManager.getTexture(type);
            addPiece(type, texture, xOffset, (type == "QUEEN") ? yOffset + 20.0f : yOffset);
            xOffset += texture.getSize().x + pieceSpacing;
        }
    }

    void addPiece(const std::string &pieceType, const sf::Texture &texture, float xOffset, float yOffset)
    {
        sf::Sprite sprite;
        sprite.setTexture(texture);
        if (!sprite.getTexture())
        {
            std::cerr << "Texture not set for sprite" << std::endl;
            return;
        }
        sprite.setPosition(stackRect.getPosition().x + xOffset, stackRect.getPosition().y + yOffset);
        pieces.push_back({pieceType, sprite});
        std::cerr << "Added piece: " << pieceType << " at position: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
    }

    void selectPiece(sf::Vector2f mousePos)
    {
        for (auto &piece : pieces)
        {
            if (piece.second.getGlobalBounds().contains(mousePos))
            {
                selectedPiece = piece.first;
                std::cerr << "CLICKED: " << piece.first << std::endl;
            }
        }
    }

    bool contains(sf::Vector2f mousePos)
    {
        return stackRect.getGlobalBounds().contains(mousePos);
    }
    std::string selectedPiece;

private:
    sf::RectangleShape stackRect;
    std::vector<std::pair<std::string, sf::Sprite>> pieces;
    float windowHeight;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(stackRect, states);
        for (const auto &piece : pieces)
        {
            if (piece.first == selectedPiece)
            {
                sf::Sprite highlightedSprite = piece.second;
                highlightedSprite.setColor(sf::Color(255, 100, 100)); // Change color to red
                target.draw(highlightedSprite, states);
                std::cerr << "Highliting selected piece" << std::endl;
            }
            else
            {
                target.draw(piece.second, states);
            }
        }
    }
};