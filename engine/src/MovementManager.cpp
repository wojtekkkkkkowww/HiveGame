#include "MovementManager.hpp"

namespace hive
{

    std::set<Position> MovementManager::getQueenBeeMoves(Position position) const
    {
        std::set<Position> neighbours = getNeighbours(position);
        std::set<Position> moves;

        for (auto &neighbour : neighbours)
        {
            if (isEmpty(neighbour))
            {
                moves.insert(neighbour);
            }
        }
        return moves;
    }

    /*
    BEATTLE:
    jeden ruch do przodu gdziekolwiek poprostu może skakać na figury
    z małym wyjąkiem którym jest to że miejsce w które chce skoczyć musi być
    albo na tym samym poziomie albo o jeden wyżej.
    */
    std::set<Position> MovementManager::getBeetleMoves(Position position) const
    {
        std::set<Position> moves;
        std::vector<Position> directions = {N, NE, SE, S, SW, NW};
        auto beetle_level = getLevel(position);

        for (auto &direction : directions)
        {
            Position newPos = {position.x + direction.x, position.y + direction.y};
            std::cerr << "dif " << abs(beetle_level - getLevel(newPos)) << std::endl;
            std::cerr << "newPos " << newPos.x << " " << newPos.y << std::endl;
            if ( abs(beetle_level - getLevel(newPos)) <= 1)
            {   
                std::cerr << "hej newPos " << newPos.x << " " << newPos.y << std::endl;
                moves.insert(newPos);
            }
        }

        return moves;
    }

    /*
    SPIDER
    Ruch długości 3 bez zawracania oraz w każdym pośrednim polu musi się z kimś stykać.
    do zaimplementowania banalne :
    prosty rekurencyjny algorytm
    1) idziesz w każdym możliwym kierunku
    2) zwiększasz counter ruchów
    3) dodajesz pole do listy odwiedzonych
    4) wracasz do punktu 1 lub kończysz rekurencję dodając pole do zbioru dostępnych ruchów
    */

    std::set<Position> MovementManager::getSpiderMoves(Position position) const
    {
        std::set<Position> visited;

        std::function<void(Position, int)> dfs = [&](Position pos, int depth)
        {
            if (calculateNeighbours(pos, "BLACK") + calculateNeighbours(pos, "WHITE") == 0)
            {
                return;
            }

            if (depth == 3)
            {
                visited.insert(pos);
                return;
            }

            visited.insert(pos);

            for (auto &neighbour : getNeighbours(pos))
            {
                if (visited.find(neighbour) == visited.end() && isEmpty(neighbour))
                {
                    dfs(neighbour, depth + 1);
                }
            }
        };

        dfs(position, 0);
        return visited;
    }

    /*
    GRASSHOPPER:
    Prosta ligika:
    sprawdż w których kierunkach możesz iść
    i idź w każdą stronę do końca planszy(aż napotkasz puste pole) dodaj do zbioru dostępnych ruchów
    */
    std::set<Position> MovementManager::getGrasshopperMoves(Position position) const
    {
        std::set<Position> moves;
        std::vector<Position> directions = {N, NE, SE, S, SW, NW};

        for (auto &direction : directions)
        {
            Position current = position;

            // nie może skakać na puste pola obok siebie
            if (isEmpty(current + direction))
            {
                continue;
            }

            while (!isEmpty(current))
            {
                current = current + direction;
            }

            moves.insert(current);
        }

        return moves;
    }

    std::set<Position> MovementManager::getAntMoves(Position position) const
    {
        position.x += 0;
        position.y += 0;

        return emptyTiles;
    }

    std::set<Position> MovementManager::getAvailableMoves(Tile tile) const
    {
        /*nie chce mi sie zamieniac na enum*/
        try{
        if (tile.type == "QUEEN")
        {
            std::cerr << "enter Queen" << std::endl;
            return getQueenBeeMoves(tile.position);
        }
        else if (tile.type == "BEETLE")
        {
            std::cerr << "enter Beetle" << std::endl;
            return getBeetleMoves(tile.position);
        }
        else if (tile.type == "SPIDER")
        {
            std:: cerr << "enter Spider" << std::endl;
            return getSpiderMoves(tile.position);
        }
        else if (tile.type == "GRASSHOPPER")
        {
            std::cerr << "enter Grasshopper" << std::endl;
            return getGrasshopperMoves(tile.position);
        }
        else if (tile.type == "ANT")
        {
            std::cerr << "enter Ant" << std::endl;
            return getAntMoves(tile.position);
        }
        else
        {
            return std::set<Position>(); // Return an empty set for unknown types
        }}catch(std::exception &e){
            std::cerr << "wtf" << e.what() << std::endl;
            return std::set<Position>();
        }
    }

}