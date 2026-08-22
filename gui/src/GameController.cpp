#include "GameController.hpp"
#include "Game.hpp"

GameController::GameController(sf::RenderWindow &window)
    : BaseInterface(window),
      pieceSelector(800, 600) // Initialize pieceSelector with window dimensions
{
    ResourceManager &resourceManager = ResourceManager::getInstance();
    waitButton = Button(sf::Vector2f(100, 50), sf::Vector2f(700, 10), "Wait",
                        resourceManager.getFont("arial.ttf"));
    waitButton.setFillColor(sf::Color(246, 184, 66));
    waitButton.setTextColor(sf::Color(28, 27, 26));
}

void GameController::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            handleWindowClose();
            break;

        case sf::Event::MouseWheelScrolled:
            handleMouseWheelScroll(event.mouseWheelScroll.delta);
            break;

        case sf::Event::MouseButtonPressed:
            handleMouseButtonPressed(event.mouseButton.button);
            break;

        case sf::Event::MouseButtonReleased:
            handleMouseButtonReleased(event.mouseButton.button);
            break;

        case sf::Event::MouseMoved:
            handleMouseMoved();
            break;

        case sf::Event::Resized:
            handleWindowResized(event.size.width, event.size.height);
            break;

        default:
            break;
        }
    }
}

void GameController::handleMouseButtonReleased(sf::Mouse::Button button)
{
    if (button == sf::Mouse::Left)
    {
        dragging = false;
        auto clickEndTime = std::chrono::high_resolution_clock::now();
        auto clickDuration =
            std::chrono::duration_cast<std::chrono::milliseconds>(clickEndTime - clickStartTime)
                .count();

        if (clickDuration < 200.0f && isMyTurn())
        {
            handleMouseClick();
        }
    }
    change = true;
}

void GameController::handleMouseButtonPressed(sf::Mouse::Button button)
{
    if (button == sf::Mouse::Left)
    {
        window.setView(window.getDefaultView());
        if (!pieceSelector.contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
        {
            dragging = true;
            window.setView(boardView);
        }

        lastMousePos = window.mapPixelToCoords(-sf::Mouse::getPosition(window));
        clickStartTime = std::chrono::high_resolution_clock::now();
    }
    change = true;
}

void GameController::render()
{
    pieceSelector.updateCounts(game->getPlayers()[game->getCurrentTurn()]);

    window.clear(sf::Color(82, 78, 72));

    window.setView(boardView);
    window.draw(boardDrawable);

    window.setView(window.getDefaultView());

    window.draw(turnText);
    window.draw(pieceSelector);

    const auto &availableActions = game->getAvailableActions();
    if (availableActions.size() == 1 && availableActions.begin()->type == ActionType::WAIT)
    {
        window.draw(waitButton);
    }

    window.display();
}

void GameController::applyOpponentAction(const std::string &action)
{
    if (const auto result = game->applyActionExpected(action); !result)
    {
        std::cerr << "Failed to apply action: " << action << " reason: " << toString(result.error())
                  << std::endl;
    }
    change = true;
}

bool GameController::applyLocalAction(const Action &action)
{
    const bool applied = game->applyAction(action);
    if (applied)
    {
        change = true;
    }
    return applied;
}

void GameController::setPlayer(PlayerColor player)
{
    this->player = player;
    boardDrawable.setPlayer(player);
}

bool GameController::isMyTurn() { return localHotseat || game->getCurrentTurn() == player; }

void GameController::handleMouseClick()
{
    window.setView(window.getDefaultView());
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (pieceSelector.contains(mousePos))
    {
        handlePieceSelectorClick(mousePos);
    }
    else if (waitButton.contains(mousePos))
    {
        handleWaitButtonClick();
    }
    else
    {
        handleBoardClick(mousePos);
    }
}

void GameController::handleWaitButtonClick()
{
    const auto &availableActions = game->getAvailableActions();
    if (availableActions.size() == 1 && availableActions.begin()->type == ActionType::WAIT)
    {
        if (game->applyAction(makeWaitAction()))
        {
            message = game->getLastAction();
        }
    }
}

void GameController::handlePieceSelectorClick(const sf::Vector2f &mousePos)
{
    pieceSelector.selectPiece(mousePos);
}

void GameController::handleBoardClick(sf::Vector2f mousePos)
{
    /*
    Trzeba zmienić na boardView przed sprawdzeniem pozycji
    */
    window.setView(boardView);
    mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    Position boardPos = convertMouseToBoardPos(mousePos);

    if (pieceSelector.selectedPiece)
    {
        handlePiecePlacement(boardPos);
    }
    else
    {
        handlePieceMovement(boardPos);
    }
}

void GameController::handlePiecePlacement(const Position &boardPos)
{
    if (boardPos != invalidPosition)
    {
        if (game->applyAction(makePlaceAction(boardPos, *pieceSelector.selectedPiece)))
        {
            message = game->getLastAction();
        }
    }
    pieceSelector.selectedPiece.reset();
}

void GameController::handlePieceMovement(const Position &boardPos)
{
    if (boardDrawable.selectedPosition == invalidPosition)
    {
        if (boardPos != invalidPosition)
        {
            boardDrawable.selectedPosition = boardPos;
        }
    }
    else
    {
        if (boardPos != invalidPosition)
        {
            auto action = makeMoveAction(boardDrawable.selectedPosition, boardPos);
            if (game->applyAction(action))
            {
                message = game->getLastAction();
            }
            boardDrawable.selectedPosition = invalidPosition;
        }
    }
}
