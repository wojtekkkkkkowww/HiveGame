#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

/*
Pictures of tiles are collected from website:
https://www.instructables.com/Hive-game-with-box/
*/

/**
 * @brief The ResourceManager class is responsible for loading and storing textures and fonts.
 */
class ResourceManager
{
  public:
    static ResourceManager &getInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    const sf::Texture &getTexture(char name)
    {
        if (textures.find(name) == textures.end())
        {
            sf::Texture texture;
            if (!texture.loadFromFile(std::string(SOURCE_DIR) + "/res/" + name + ".png"))
            {
                std::cerr << "Error loading texture: " << name << std::endl;
            }
            else
            {
                textures[name] = texture;
                std::cerr << "Loaded texture: " << name << std::endl;
            }
        }
        return textures[name];
    }

    const sf::Texture &getPieceAtlas()
    {
        if (!pieceAtlas)
        {
            pieceAtlas = std::make_unique<sf::Texture>();
            const auto path =
                std::string(SOURCE_DIR) + "/res/generated/hive-pieces-sheet.png";
            if (!pieceAtlas->loadFromFile(path))
            {
                throw std::runtime_error("Failed to load generated piece atlas: " + path);
            }
        }
        return *pieceAtlas;
    }

    const sf::Font &getFont(const std::string &name)
    {
        if (fonts.find(name) == fonts.end())
        {
            sf::Font font;
            if (!font.loadFromFile(std::string(SOURCE_DIR) + "/res/" + name))
            {
            }
            else
            {
                fonts[name] = font;
            }
        }
        return fonts[name];
    }

  private:
    ResourceManager() = default;
    std::unordered_map<char, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
    std::unique_ptr<sf::Texture> pieceAtlas;
};
