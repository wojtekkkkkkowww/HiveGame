#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "ResourceManager.hpp"

/*
 * @brief The PieceSelector class is responsible for displaying pool of pieces that player can choose from.
 */
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
        std::vector<char> pieceTypes = {'A', 'B', 'G', 'Q', 'S'};
        float xOffset = 10.0f;
        float yOffset = 10.0f;
        float pieceSpacing = 60.0f;

        for (const auto &type : pieceTypes)
        {
            const sf::Texture &texture = resourceManager.getTexture(type);
            addPiece(type, texture, xOffset, (type == 'Q') ? yOffset + 20.0f : yOffset);
            xOffset += texture.getSize().x + pieceSpacing;
        }
    }

    void addPiece(char pieceType, const sf::Texture &texture, float xOffset, float yOffset)
    {
        sf::Sprite sprite;
        sprite.setTexture(texture);
        if (!sprite.getTexture())
        {
            return;
        }
        sprite.setPosition(stackRect.getPosition().x + xOffset, stackRect.getPosition().y + yOffset);
        pieces.push_back({pieceType, sprite});
    }

    void selectPiece(sf::Vector2f mousePos)
    {
        for (auto &piece : pieces)
        {
            if (piece.second.getGlobalBounds().contains(mousePos))
            {
                selectedPiece = piece.first;
            }
        }
    }

    bool contains(sf::Vector2f mousePos)
    {
        return stackRect.getGlobalBounds().contains(mousePos);
    }

    char selectedPiece;

private:
    sf::RectangleShape stackRect;
    std::vector<std::pair<char, sf::Sprite>> pieces;
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
            }
            else
            {
                target.draw(piece.second, states);
            }
        }
    }
};