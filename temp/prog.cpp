#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <set>
#include <string>

// Structure to represent a grid position
struct Position {
    int left, right;

    Position(int l = 0, int r = 0) : left(l), right(r) {}

    // Define the addition of two positions
    Position add(const Position& other) const {
        return Position(left + other.left, right + other.right);
    }

    // Overloading the less-than operator for using Position in a set
    bool operator<(const Position& other) const {
        if (left == other.left)
            return right < other.right;
        return left < other.left;
    }
};

// Define the directions for hexagonal movement
std::set<Position> directions = {
    Position(0, -1),  // N
    Position(0, 1),   // S
    Position(1, -1),  // NE
    Position(1, 0),   // SE
    Position(-1, 1),  // SW
    Position(-1, 0)   // NW
};

// Generate hexagonal grid positions
std::set<Position> generate_hex_grid(int radius) {
    std::set<Position> grid;
    grid.insert(Position(0, 0));  // Start at the center (0,0)

    // Add new positions iteratively based on the radius
    for (int i = 0; i < radius; ++i) {
        std::set<Position> new_positions;
        for (const auto& pos : grid) {
            // Go in all 6 directions and add new positions
            for (const auto& dir : directions) {
                Position new_pos = pos.add(dir);
                new_positions.insert(new_pos);
            }
        }
        grid.insert(new_positions.begin(), new_positions.end());
    }

    return grid;
}

// Calculate hexagon corner points (30-degree rotation included)
sf::Vector2f hex_corner(const sf::Vector2f& center, float size, int i) {
    float angle_deg = 60 * i;
    float angle_rad = M_PI / 180 * angle_deg;
    return sf::Vector2f(center.x + size * cos(angle_rad), center.y + size * sin(angle_rad));
}

// Plot the hexagonal grid using SFML
void plot_hex_grid(const std::set<Position>& grid, float hex_size) {
    // Create a window and render texture for off-screen drawing
    sf::RenderTexture render_texture;
    render_texture.create(800, 800);  // Adjust as needed

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {  // Load font (ensure it's in the working directory)
        std::cerr << "Failed to load font" << std::endl;
        return;
    }

    for (const auto& position : grid) {
        // Calculate position of the hex based on the grid coordinates
        float x = hex_size * (1.5f * position.left);
        float y = hex_size * (std::sqrt(3) * (position.right + position.left / 2.0f));

        // Create hexagon shape
        sf::ConvexShape hexagon;
        hexagon.setPointCount(6);
        for (int i = 0; i < 6; ++i) {
            sf::Vector2f corner = hex_corner(sf::Vector2f(x + 400, y + 400), hex_size, i);  // Shift to center
            hexagon.setPoint(i, corner);
        }
        hexagon.setOutlineThickness(1);
        hexagon.setOutlineColor(sf::Color::Black);
        hexagon.setFillColor(sf::Color::Transparent);

        // Draw the hexagon
        render_texture.draw(hexagon);

        // Add the coordinates in the center of each hexagon
        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(position.left) + "," + std::to_string(position.right));
        text.setCharacterSize(10);
        text.setFillColor(sf::Color::Black);
        text.setPosition(x + 395, y + 395);  // Offset for proper centering
        render_texture.draw(text);
    }

    // Save the rendered texture to a PNG image
    render_texture.display();
    render_texture.getTexture().copyToImage().saveToFile("hex_grid.png");
}

int main() {
    // Set the radius of the hex grid
    int radius = 7;

    // Generate the hexagonal grid
    std::set<Position> grid = generate_hex_grid(radius);

    // Plot and save the hexagonal grid
    plot_hex_grid(grid, 40.0f);  // Adjust hex size if needed

    std::cout << "Hex grid saved as 'hex_grid.png'" << std::endl;
    return 0;
}
