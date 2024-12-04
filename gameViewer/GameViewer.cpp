#include "GameViewer.hpp"
#include "FileOperations.hpp"

namespace hive
{

    GameViewer::GameViewer(sf::RenderWindow &window, const std::string &gameFile)
        : BaseInterface(window)
    {
        moves = loadFromFile(gameFile);
        game->startNewGame();
    }

    std::string trim(const std::string &str)
    {
        size_t start = str.find_first_not_of(" \t\n\r\f\v");
        size_t end = str.find_last_not_of(" \t\n\r\f\v");
        return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
    }

    void GameViewer::processEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                handleWindowClose(); //
                break;

            case sf::Event::MouseWheelScrolled:
                handleMouseWheelScroll(event.mouseWheelScroll.delta); //
                break;

            case sf::Event::MouseButtonPressed:
                handleMouseButtonPressed(event.mouseButton.button);
                break;

            case sf::Event::MouseButtonReleased:
                handleMouseButtonReleased(event.mouseButton.button);
                break;

            case sf::Event::MouseMoved:
                handleMouseMoved(); //
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Right)
                {
                    handleNextMove();
                }
                else if (event.key.code == sf::Keyboard::Left)
                {
                    handlePreviousMove();
                }
                break;

            default:
                break;
            }
        }
    }

    void GameViewer::handleMouseButtonReleased(sf::Mouse::Button button)
    {
        if (button == sf::Mouse::Left)
        {
            dragging = false;
        }
        change = true;
    }

    void GameViewer::handleMouseButtonPressed(sf::Mouse::Button button)
    {
        if (button == sf::Mouse::Left)
        {
            dragging = true;
            window.setView(boardView);

            lastMousePos = window.mapPixelToCoords(-sf::Mouse::getPosition(window));
            clickStartTime = std::chrono::high_resolution_clock::now();
        }
        change = true;
    }

    void GameViewer::render()
    {
        window.clear(sf::Color::White);

        window.setView(boardView);
        window.draw(boardDrawable);
        window.setView(window.getDefaultView());
        window.draw(turnText);
        window.display();
    }

    void GameViewer::handleNextMove()
    {
        if (currentMoveIndex < moves.size())
        {

            if (game->applyAction(moves[currentMoveIndex]))
            {
                std::cerr << "move applyed " << currentMoveIndex << std::endl;
                currentMoveIndex++;
                change = true;
            }
            else
            {
                std::cerr << "Failed to apply next move: " << moves[currentMoveIndex] << std::endl;

                std::cerr << "Failed to apply next move: " << currentMoveIndex << std::endl;
            }
        }
        else
        {
            std::cerr << "No more moves" << std::endl;
        }
    }

    void GameViewer::handlePreviousMove()
    {
        if (currentMoveIndex > 1)
        {
            std::cerr << "Reverting move " << currentMoveIndex - 1 << std::endl;
            game->revertAction();
            currentMoveIndex--;
            change = true;
        }
    }
}