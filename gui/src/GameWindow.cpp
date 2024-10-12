#include "GameWindow.hpp"
#include "Game.hpp"

GameWindow::GameWindow()
    : window(sf::VideoMode(800, 600), "Hive Game", sf::Style::Default, sf::ContextSettings(0, 0, 8)),
      pieceSelector(800, 600),
      game(std::make_unique<Game>()),
      boardDrawable(game->board, 32.0f)
{
    ResourceManager &resourceManager = ResourceManager::getInstance();
    font = resourceManager.getFont("arial.ttf");

    boardView = window.getDefaultView();
}

void GameWindow::run()
{
    sf::Clock clock;
    const sf::Time frameTime = sf::seconds(1.f / 20.f);

    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        processEvents();
        if (change)
        {
            update();
            render();
            change = false;
        }

        if (elapsed < frameTime)
        {
            sf::sleep(frameTime - elapsed);
        }
    }
}

void GameWindow::processEvents()
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

void GameWindow::handleMouseMoved()
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

void GameWindow::handleMouseButtonReleased(sf::Mouse::Button button)
{
    if (button == sf::Mouse::Left)
    {
        dragging = false;
        auto clickEndTime = std::chrono::high_resolution_clock::now();
        auto clickDurtion = std::chrono::duration_cast<std::chrono::milliseconds>(clickEndTime - clickStartTime).count();

        if (clickDurtion < 200.0f)
        {
            handleMouseClick();
        }
    }
    change = true;
}

void GameWindow::handleMouseButtonPressed(sf::Mouse::Button button)
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

void GameWindow::handleWindowClose()
{
    window.close();
}

void GameWindow::update()
{
    window.setView(boardView);
    boardDrawable.update();
    player = game->currentTurn;
    std::cerr << "Board updated" << std::endl;
}

void GameWindow::render()
{
    window.clear(sf::Color::White);

    sf::Text turnText;
    turnText.setFont(font);
    turnText.setCharacterSize(24);
    turnText.setFillColor(sf::Color::Black);
    turnText.setPosition(10, 10);
    turnText.setString("Current turn: " + player);

    window.setView(boardView);
    window.draw(boardDrawable);

    window.setView(window.getDefaultView());

    window.draw(turnText);
    window.draw(pieceSelector);

    window.display();
}

void GameWindow::handleMouseClick()
{
    window.setView(window.getDefaultView());
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (pieceSelector.contains(mousePos))
    {
        handlePieceSelectorClick(mousePos);
    }
    else
    {
        handleBoardClick(mousePos);
    }
}

void GameWindow::handlePieceSelectorClick(const sf::Vector2f &mousePos)
{
    pieceSelector.selectPiece(mousePos);
}

void GameWindow::handleBoardClick(sf::Vector2f mousePos)
{
    window.setView(boardView);
    mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    Position boardPos = convertMouseToBoardPos(mousePos);
    std::cerr << "Mouse clicked board view at " << mousePos.x << " " << mousePos.y << std::endl;

    if (!pieceSelector.selectedPiece.empty())
    {
        handlePiecePlacement(boardPos);
    }
    else
    {
        handlePieceMovement(boardPos);
    }
}

void GameWindow::handlePiecePlacement(const Position &boardPos)
{
    if (boardPos != invalidPosition)
    {
        PlaceAction action(boardPos, pieceSelector.selectedPiece);
        game->applyAction(action);
    }
    pieceSelector.selectedPiece = "";
}

void GameWindow::handlePieceMovement(const Position &boardPos)
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
            game->applyAction(action);
            boardDrawable.selectedPosition = invalidPosition;
        }
    }
}
void GameWindow::handleMouseWheelScroll(float delta)
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

Position GameWindow::convertMouseToBoardPos(sf::Vector2f mousePos)
{
    for (const auto &hex : boardDrawable.hexDrawables)
    {
        std::cerr << "Checking hex at " << hex.tilePosition.x << " " << hex.tilePosition.y << std::endl;
        if (hex.contains(mousePos))
        {
            return hex.tilePosition;
        }
    }
    return invalidPosition;
}