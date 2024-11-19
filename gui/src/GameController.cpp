#include "GameController.hpp"
#include "Game.hpp"

GameController::GameController(sf::RenderWindow &window)
    : window(window),
      pieceSelector(800, 600),
      game(std::make_unique<Game>()),
      boardDrawable(game->board, 32.0f)
{
    ResourceManager &resourceManager = ResourceManager::getInstance();
    boardView = window.getDefaultView();

    waitButton = Button(sf::Vector2f(100, 50), sf::Vector2f(700, 10), "Wait", resourceManager.getFont("arial.ttf"));

    turnText.setFont(resourceManager.getFont("arial.ttf"));
    turnText.setCharacterSize(24);
    turnText.setFillColor(sf::Color::Black);
    turnText.setPosition(10, 10);
}

void GameController::handleGameControl()
{
    processEvents();
    if (change)
    {
        update();
        render();
        change = false;
    }
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

        default:
            break;
        }
    }
}

void GameController::handleMouseMoved()
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

void GameController::handleMouseButtonReleased(sf::Mouse::Button button)
{
    if (button == sf::Mouse::Left)
    {
        dragging = false;
        auto clickEndTime = std::chrono::high_resolution_clock::now();
        auto clickDurtion = std::chrono::duration_cast<std::chrono::milliseconds>(clickEndTime - clickStartTime).count();

        if (clickDurtion < 200.0f && isMyTurn())
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

void GameController::handleWindowClose()
{
    window.close();
}

void GameController::update()
{
    window.setView(boardView);
    boardDrawable.update();
    updateTurnText();
}

void GameController::render()
{
    window.clear(sf::Color::White);

    window.setView(boardView);
    window.draw(boardDrawable);

    window.setView(window.getDefaultView());

    window.draw(turnText);
    window.draw(pieceSelector);

    if (game->avaliableActions.size() == 1 && game->avaliableActions.begin()->type == "WAIT")
    {
        window.draw(waitButton);
    }

    window.display();
    std ::cerr << "Game window rendered" << std::endl;
}

void GameController::updateTurnText()
{
    if (game->gameStatus == "WHITE_WINS" || game->gameStatus == "BLACK_WINS" || game->gameStatus == "DRAW")
    {
        turnText.setString("Game over! " + game->gameStatus);
        return;
    }
    turnText.setString("Current turn: " + std::string(1,game->currentTurn));
}

void GameController::apllyOpponentAction(const std::string &action)
{
    if(!game->applyAction(action)){
        std::cerr << "Failed to apply action: " << action << std::endl;
    }
    change = true;
}

void GameController::setPlayer(char player)
{
    this->player = player;
    boardDrawable.setPlayer(player);
}

bool GameController::isMyTurn()
{
    return game->currentTurn == player;
}

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
    if (game->avaliableActions.size() == 1 && game->avaliableActions.begin()->type == "WAIT")
    {
        WaitAction action;
        game->applyAction(action);
        message = game->getLastAction();
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

    if (!pieceSelector.selectedPiece == '\0')
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
        // to jest w miarę łatwe do zmienieania
        PlaceAction action(boardPos, pieceSelector.selectedPiece);
        if (game->applyAction(action))
        {
            message = game->getLastAction();
        }
    }
    pieceSelector.selectedPiece = '\0';
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
            MoveAction action(boardDrawable.selectedPosition, boardPos);
            if (game->applyAction(action))
            {
                message = game->getLastAction();
            }
            boardDrawable.selectedPosition = invalidPosition;
        }
    }
}

void GameController::handleMouseWheelScroll(float delta)
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

Position GameController::convertMouseToBoardPos(sf::Vector2f mousePos)
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