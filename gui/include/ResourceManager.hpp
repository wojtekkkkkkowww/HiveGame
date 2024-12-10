#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

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
                textures[name] = std::move(texture);
                std::cerr << "Loaded texture: " << name << std::endl;
            }
        }
        return textures[name];
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
                fonts[name] = std::move(font);
            }
        }
        return fonts[name];
    }

private:
    ResourceManager() = default;
    std::unordered_map<char, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
};