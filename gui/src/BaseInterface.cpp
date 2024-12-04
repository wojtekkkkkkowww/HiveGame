#include "BaseInterface.hpp"

BaseInterface::BaseInterface(sf::RenderWindow &window)
    : window(window),
      game(std::make_unique<Game>()),
      boardDrawable(game->board, 32.0f)
{
    ResourceManager &resourceManager = ResourceManager::getInstance();
    boardView = window.getDefaultView();

    turnText.setFont(resourceManager.getFont("arial.ttf"));
    turnText.setCharacterSize(24);
    turnText.setFillColor(sf::Color::Black);
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

void BaseInterface::handleWindowClose()
{
    window.close();
}

void BaseInterface::updateTurnText()
{
    if (game->gameStatus == "WHITE_WINS" || game->gameStatus == "BLACK_WINS" || game->gameStatus == "DRAW")
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
