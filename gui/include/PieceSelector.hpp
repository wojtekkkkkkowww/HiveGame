#pragma once
#include "PieceAtlas.hpp"
#include "PieceCatalog.hpp"
#include "Player.hpp"
#include "ResourceManager.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <vector>

/*
 * @brief The PieceSelector class is responsible for displaying pool of pieces that player can
 * choose from.
 */
class PieceSelector : public sf::Drawable
{
  public:
    PieceSelector(float windowWidth, float windowHeight)
    {
        stackRect = sf::RectangleShape(sf::Vector2f(windowWidth, 150));
        stackRect.setFillColor(sf::Color(31, 29, 27, 245));
        stackRect.setOutlineColor(sf::Color(246, 184, 66));
        stackRect.setOutlineThickness(2);
        stackRect.setPosition(0, windowHeight - 150);

        loadPieces();
    }

    void loadPieces()
    {
        ResourceManager &resourceManager = ResourceManager::getInstance();
        const sf::Texture &atlas = resourceManager.getPieceAtlas();
        const sf::Font &font = resourceManager.getFont("arial.ttf");
        const sf::Vector2f cellSize{112.0f, 132.0f};
        constexpr float cellSpacing = 14.0f;
        constexpr float targetHeight = 88.0f;
        float xOffset = 12.0f;

        for (const auto &definition : hive::pieceCatalog)
        {
            PieceEntry entry;
            entry.type = definition.code;
            entry.sprite.setTexture(atlas);
            const auto textureRect = PieceAtlas::textureRect(atlas, entry.type);
            entry.sprite.setTextureRect(textureRect);
            const float uniformScale = targetHeight / static_cast<float>(textureRect.height);
            entry.sprite.setScale(uniformScale, uniformScale);

            entry.background.setSize(cellSize);
            entry.background.setPosition(stackRect.getPosition().x + xOffset,
                                         stackRect.getPosition().y + 8.0f);
            entry.background.setFillColor(idleFillColor);
            entry.background.setOutlineColor(idleOutlineColor);
            entry.background.setOutlineThickness(outlineThickness);

            const auto cellBounds = entry.background.getGlobalBounds();
            const auto spriteBounds = entry.sprite.getLocalBounds();
            entry.sprite.setOrigin(spriteBounds.left + spriteBounds.width / 2.0f,
                                   spriteBounds.top + spriteBounds.height / 2.0f);
            entry.sprite.setPosition(cellBounds.left + cellBounds.width / 2.0f,
                                     cellBounds.top + 51.0f);

            entry.countText.setFont(font);
            entry.countText.setCharacterSize(20);
            entry.countText.setFillColor(sf::Color(238, 230, 211));
            entry.countText.setOutlineColor(sf::Color(20, 19, 18));
            entry.countText.setOutlineThickness(1.0f);

            pieces.push_back(std::move(entry));
            xOffset += cellSize.x + cellSpacing;
        }
    }

    void updateCounts(const hive::Player &player)
    {
        for (auto &piece : pieces)
        {
            piece.availableCount = player.getTileCount(piece.type);
            piece.countText.setString(std::to_string(piece.availableCount));
            const auto textBounds = piece.countText.getLocalBounds();
            const auto cellBounds = piece.background.getGlobalBounds();
            piece.countText.setOrigin(textBounds.left + textBounds.width / 2.0f,
                                      textBounds.top + textBounds.height / 2.0f);
            piece.countText.setPosition(cellBounds.left + cellBounds.width / 2.0f,
                                        cellBounds.top + 116.0f);
            piece.sprite.setColor(piece.availableCount > 0 ? sf::Color::White
                                                           : sf::Color(120, 120, 120, 110));
        }

        if (selectedPiece != '\0' && countFor(selectedPiece) == 0)
        {
            selectedPiece = '\0';
        }
    }

    void selectPiece(sf::Vector2f mousePos)
    {
        for (auto &piece : pieces)
        {
            if (piece.availableCount > 0 && piece.background.getGlobalBounds().contains(mousePos))
            {
                selectedPiece = piece.type;
                return;
            }
        }
    }

    bool contains(sf::Vector2f mousePos) { return stackRect.getGlobalBounds().contains(mousePos); }

    char selectedPiece = '\0';

  private:
    struct PieceEntry
    {
        char type = '\0';
        int availableCount = 0;
        sf::Sprite sprite;
        sf::Text countText;
        sf::RectangleShape background;
    };

    int countFor(char type) const
    {
        for (const auto &piece : pieces)
        {
            if (piece.type == type)
            {
                return piece.availableCount;
            }
        }
        return 0;
    }

    inline static const sf::Color idleFillColor{246, 184, 66, 18};
    inline static const sf::Color idleOutlineColor{142, 110, 54};
    inline static const sf::Color selectedFillColor{54, 235, 207, 36};
    inline static const sf::Color selectedOutlineColor{54, 235, 207};
    static constexpr float outlineThickness = 2.0f;

    sf::RectangleShape stackRect;
    std::vector<PieceEntry> pieces;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(stackRect, states);
        for (const auto &piece : pieces)
        {
            auto background = piece.background;
            const bool selected = piece.type == selectedPiece;
            background.setFillColor(selected ? selectedFillColor : idleFillColor);
            background.setOutlineColor(selected ? selectedOutlineColor : idleOutlineColor);
            target.draw(background, states);
            target.draw(piece.sprite, states);
            target.draw(piece.countText, states);
        }
    }
};
