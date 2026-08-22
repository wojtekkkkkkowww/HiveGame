#include "AlphaBethaSearch.hpp"
#include "Button.hpp"
#include "GameController.hpp"
#include "ResourceManager.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <memory>
#include <optional>

namespace
{
namespace WindowConfig
{
constexpr unsigned int width = 1000U;
constexpr unsigned int height = 700U;
constexpr unsigned int antialiasingLevel = 8U;
constexpr unsigned int frameLimit = 60U;
}

namespace Theme
{
const sf::Color backgroundColor(20, 19, 18);
const sf::Color titleColor(246, 184, 66);
const sf::Color titleOutlineColor(35, 31, 27);
const sf::Color subtitleColor(218, 211, 196);
const sf::Color panelColor(34, 31, 29, 245);
const sf::Color panelOutlineColor(94, 83, 67);
const sf::Color humanButtonFillColor(246, 184, 66);
const sf::Color humanButtonTextColor(30, 27, 24);
const sf::Color aiButtonFillColor(52, 194, 178);
const sf::Color aiButtonTextColor(18, 31, 31);
const sf::Color decorationOutlineColor(246, 184, 66, 45);
}

namespace MenuLayout
{
constexpr float menuWidth = static_cast<float>(WindowConfig::width);
constexpr float menuHeight = static_cast<float>(WindowConfig::height);
constexpr float titleX = 390.0f;
constexpr float titleY = 110.0f;
constexpr float subtitleX = 343.0f;
constexpr float subtitleY = 220.0f;
constexpr float panelWidth = 500.0f;
constexpr float panelHeight = 330.0f;
constexpr float panelX = 250.0f;
constexpr float panelY = 285.0f;
constexpr float buttonWidth = 340.0f;
constexpr float buttonHeight = 68.0f;
constexpr float buttonX = 330.0f;
constexpr float humanButtonY = 365.0f;
constexpr float aiButtonY = 455.0f;
constexpr float decorationRadius = 42.0f;
constexpr float decorationLeftX = 100.0f;
constexpr float decorationRightX = 900.0f;
constexpr float decorationStartY = 95.0f;
constexpr float decorationRowSpacing = 155.0f;
constexpr float titleOutlineThickness = 3.0f;
constexpr float menuPanelOutlineThickness = 2.0f;
constexpr float decorationOutlineThickness = 2.0f;
constexpr std::size_t decorationCount = 8U;
}

enum class LocalMode
{
    Menu,
    HumanVsHuman,
    HumanVsAi
};

struct MenuScreen
{
    Button humanButton;
    Button aiButton;
    sf::Text title;
    sf::Text subtitle;
    sf::RectangleShape menuPanel;
    std::array<sf::CircleShape, MenuLayout::decorationCount> decoration;
};

Button makeMenuButton(const sf::Vector2f &size, const sf::Vector2f &position, const std::string &text,
                      const sf::Font &font, const sf::Color &fillColor, const sf::Color &textColor)
{
    Button button(size, position, text, font);
    button.setFillColor(fillColor);
    button.setTextColor(textColor);
    return button;
}

sf::Text makeTitle(const sf::Font &font)
{
    sf::Text title("HIVE", font, 82);
    title.setStyle(sf::Text::Bold);
    title.setFillColor(Theme::titleColor);
    title.setOutlineColor(Theme::titleOutlineColor);
    title.setOutlineThickness(MenuLayout::titleOutlineThickness);
    title.setPosition(MenuLayout::titleX, MenuLayout::titleY);
    return title;
}

sf::Text makeSubtitle(const sf::Font &font)
{
    sf::Text subtitle("TACTICAL INSECT STRATEGY", font, 20);
    subtitle.setLetterSpacing(1.8f);
    subtitle.setFillColor(Theme::subtitleColor);
    subtitle.setPosition(MenuLayout::subtitleX, MenuLayout::subtitleY);
    return subtitle;
}

sf::RectangleShape makeMenuPanel()
{
    sf::RectangleShape menuPanel({MenuLayout::panelWidth, MenuLayout::panelHeight});
    menuPanel.setPosition(MenuLayout::panelX, MenuLayout::panelY);
    menuPanel.setFillColor(Theme::panelColor);
    menuPanel.setOutlineThickness(MenuLayout::menuPanelOutlineThickness);
    menuPanel.setOutlineColor(Theme::panelOutlineColor);
    return menuPanel;
}

std::array<sf::CircleShape, MenuLayout::decorationCount> makeDecoration()
{
    std::array<sf::CircleShape, MenuLayout::decorationCount> decoration;
    for (std::size_t index = 0; index < decoration.size(); ++index)
    {
        auto &hex = decoration[index];
        hex.setRadius(MenuLayout::decorationRadius);
        hex.setPointCount(6);
        hex.setOrigin(MenuLayout::decorationRadius, MenuLayout::decorationRadius);
        hex.setFillColor(sf::Color::Transparent);
        hex.setOutlineThickness(MenuLayout::decorationOutlineThickness);
        hex.setOutlineColor(Theme::decorationOutlineColor);
        const float x = index % 2 == 0 ? MenuLayout::decorationLeftX : MenuLayout::decorationRightX;
        const float y = MenuLayout::decorationStartY + static_cast<float>(index / 2) * MenuLayout::decorationRowSpacing;
        hex.setPosition(x, y);
    }
    return decoration;
}

MenuScreen makeMenuScreen(const sf::Font &font)
{
    return MenuScreen{
        .humanButton = makeMenuButton({MenuLayout::buttonWidth, MenuLayout::buttonHeight},
                                     {MenuLayout::buttonX, MenuLayout::humanButtonY},
                                     "LOCAL: TWO PLAYERS", font, Theme::humanButtonFillColor,
                                     Theme::humanButtonTextColor),
        .aiButton = makeMenuButton({MenuLayout::buttonWidth, MenuLayout::buttonHeight},
                                  {MenuLayout::buttonX, MenuLayout::aiButtonY}, "PLAY AGAINST AI",
                                  font, Theme::aiButtonFillColor, Theme::aiButtonTextColor),
        .title = makeTitle(font),
        .subtitle = makeSubtitle(font),
        .menuPanel = makeMenuPanel(),
        .decoration = makeDecoration(),
    };
}

std::optional<LocalMode> selectedMode(const MenuScreen &menu, const sf::Vector2f &mousePosition)
{
    if (menu.humanButton.contains(mousePosition))
    {
        return LocalMode::HumanVsHuman;
    }
    if (menu.aiButton.contains(mousePosition))
    {
        return LocalMode::HumanVsAi;
    }
    return std::nullopt;
}

void setupMatch(LocalMode mode, sf::RenderWindow &window, std::unique_ptr<GameController> &controller,
                std::unique_ptr<hive::AlphaBetaAI> &ai)
{
    controller = std::make_unique<GameController>(window);
    controller->setPlayer(hive::PlayerColor::White);
    controller->setLocalHotseat(mode == LocalMode::HumanVsHuman);
    if (mode == LocalMode::HumanVsAi)
    {
        ai = std::make_unique<hive::AlphaBetaAI>(controller->getGame());
    }
}

void drawMenu(sf::RenderWindow &window, const MenuScreen &menu)
{
    window.clear(Theme::backgroundColor);
    for (const auto &hex : menu.decoration)
    {
        window.draw(hex);
    }
    window.draw(menu.menuPanel);
    window.draw(menu.title);
    window.draw(menu.subtitle);
    window.draw(menu.humanButton);
    window.draw(menu.aiButton);
    window.display();
}
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(MenuLayout::menuWidth, MenuLayout::menuHeight), "Hive",
                            sf::Style::Default,
                            sf::ContextSettings(0, 0, WindowConfig::antialiasingLevel));
    window.setFramerateLimit(WindowConfig::frameLimit);

    const auto &font = ResourceManager::getInstance().getFont("arial.ttf");
    const auto menu = makeMenuScreen(font);

    LocalMode mode = LocalMode::Menu;
    std::unique_ptr<GameController> controller;
    std::unique_ptr<hive::AlphaBetaAI> ai;

    while (window.isOpen())
    {
        if (mode == LocalMode::Menu)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                if (event.type == sf::Event::MouseButtonReleased &&
                    event.mouseButton.button == sf::Mouse::Left)
                {
                    const auto mouse =
                        window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                    if (const auto selected = selectedMode(menu, mouse))
                    {
                        mode = *selected;
                        setupMatch(mode, window, controller, ai);
                    }
                }
            }

            drawMenu(window, menu);
            continue;
        }

        controller->handleGameControl();
        auto &game = controller->getGame();
        if (mode == LocalMode::HumanVsAi && !game.isGameOver() &&
            game.getCurrentTurn() == hive::PlayerColor::Black)
        {
            controller->applyLocalAction(ai->getNextMove());
        }
    }
}
