#include "BaseInterface.hpp"
#include "ResourceManager.hpp"

BaseInterface::BaseInterface(sf::RenderWindow &window)
    : window(window), boardView(window.getDefaultView()), game(std::make_unique<Game>()),
      boardDrawable(game->board, 32.0f)
{
    ResourceManager &resourceManager = ResourceManager::getInstance();

    turnText.setFont(resourceManager.getFont("arial.ttf"));
    turnText.setCharacterSize(24);
    turnText.setFillColor(sf::Color(238, 230, 211));
    turnText.setOutlineColor(sf::Color(20, 19, 18));
    turnText.setOutlineThickness(1.0f);
    turnText.setPosition(10, 10);
}

void BaseInterface::handleGameControl()
{
    processEvents();
    if (change)
    {
        update();
        render();
        change = false;
    }
}

void BaseInterface::update()
{
    window.setView(boardView);
    boardDrawable.update();
    updateTurnText();
}

void BaseInterface::handleMouseMoved()
{
    if (dragging)
    {
        window.setView(boardView);
        sf::Vector2f currentMousePos = window.mapPixelToCoords(-sf::Mouse::getPosition(window));
        sf::Vector2f delta = (currentMousePos - lastMousePos);
        boardView.move(delta * 0.5f);
        lastMousePos = currentMousePos;
        change = true;
    }
}

void BaseInterface::handleMouseWheelScroll(float delta)
{
    const float MIN_ZOOM_LEVEL = 0.5f;
    const float MAX_ZOOM_LEVEL = 2.0f;
    const float ZOOM_IN_FACTOR = 0.9f;
    const float ZOOM_OUT_FACTOR = 1.1f;

    float currentZoom = boardView.getSize().x / window.getDefaultView().getSize().x;

    if (delta > 0 && currentZoom > MIN_ZOOM_LEVEL)
    {
        boardView.zoom(ZOOM_IN_FACTOR);
    }
    else if (delta < 0 && currentZoom < MAX_ZOOM_LEVEL)
    {
        boardView.zoom(ZOOM_OUT_FACTOR);
    }
    change = true;
}

void BaseInterface::handleWindowClose() { window.close(); }

void BaseInterface::handleWindowResized(unsigned int width, unsigned int height)
{
    boardView.setSize(static_cast<float>(width), static_cast<float>(height));
    boardView.setCenter(static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f);
    boardDrawable.setViewportSize({static_cast<float>(width), static_cast<float>(height)});
    change = true;
}

void BaseInterface::updateTurnText()
{
    if (game->gameStatus == "WHITE_WINS" || game->gameStatus == "BLACK_WINS" ||
        game->gameStatus == "DRAW")
    {
        turnText.setString("Game over! " + game->gameStatus);
        return;
    }
    turnText.setString("Current turn: " + std::string(1, game->currentTurn));
}

Position BaseInterface::convertMouseToBoardPos(sf::Vector2f mousePos)
{
    for (const auto &hex : boardDrawable.hexDrawables)
    {
        if (hex.contains(mousePos))
        {
            return hex.tilePosition;
        }
    }
    return invalidPosition;
}
