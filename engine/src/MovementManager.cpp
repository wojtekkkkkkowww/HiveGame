#include <queue>
#include <stack>
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
    Może się poruszać na sąsiednie pola na tym samym poziomie lub różnym o 1.
    */
    std::set<Position> MovementManager::getBeetleMoves(Position position) const
    {
        std::set<Position> moves;
        std::vector<Position> directions = {N, NE, SE, S, SW, NW};
        auto beetle_level = getLevel(position);

        for (auto &direction : directions)
        {
            Position newPos = {position.x + direction.x, position.y + direction.y};
            if (abs(beetle_level - getLevel(newPos)) <= 2) // trzeba zweryfikować !
            {
                moves.insert(newPos);
            }
        }

        return moves;
    }

    /*
    SPIDER
    Ruch długości 3 bez zawracania oraz w każdym pośrednim polu musi się z kimś stykać.
    implementacja wymaga bfs który powiększa się najpierw w szerz a potem w głąb, żeby móc znaleść wszyskie ścieżki długości 3
    */

    std::set<Position> MovementManager::getSpiderMoves(Position position) const
    {
        std::set<Position> visited;
        std::queue<std::pair<Position, int>> q;
        std::set<Position> spiderNeighbours = getNeighbours(position);
        std::set<Position> result;

        q.push({position, 0});
        visited.insert(position);

        while (!q.empty())
        {
            auto [currentPos, depth] = q.front();
            q.pop();

            if (depth == 3)
            {
                result.insert(currentPos);
                continue;
            }

            for (auto &neighbor : getNeighbours(currentPos))
            {
                if (visited.find(neighbor) == visited.end() && isEmpty(neighbor))
                {
                    int c = (spiderNeighbours.find(neighbor) != spiderNeighbours.end()) ? 1 : 0;
                    if (calculateNeighbours(neighbor, "BLACK") + calculateNeighbours(neighbor, "WHITE") - c > 0)
                    {
                        Position direction = {neighbor.x - currentPos.x, neighbor.y - currentPos.y};
                        if (isDirectionBlocked(currentPos, direction))
                        {
                            continue;
                        }

                        visited.insert(neighbor);
                        q.push({neighbor, depth + 1});
                    }
                }
            }
        }

        return result;
    }

    /*
    GRASSHOPPER:
    Przeskakuje przez figury na puste pole za nimi. W dowolnym kierunku.
    Musi przeskoczyć przez co najmniej jedną figurę.
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

    /*
    Może ruszyć się na dowolne pole do którego może fizycznie dojść.

    dfs po polach które są w zbiorze emptyTiles
    w taki sposób że do sąsiada można pójść tylko jeżeli dany kierunek jest dobry
    lista odwiedzonych pól to możliwe ruchy dla ant
   */
    std::set<Position> MovementManager::getAntMoves(Position position) const
    {
        std::set<Position> visited;
        std::stack<Position> stack;

        for (auto &neighbours : getNeighbours(position))
        {
            if (emptyTiles.find(neighbours) != emptyTiles.end())
            {
                stack.push(neighbours);
            }
        }
        while (!stack.empty())
        {
            Position current = stack.top();
            stack.pop();
            if (visited.find(current) != visited.end())
            {
                continue;
            }
            visited.insert(current);

            std::vector<Position> directions = {N, NE, SE, S, SW, NW};

            for (const Position &direction : directions)
            {
                Position neighbor = {current.x + direction.x, current.y + direction.y};
                if (emptyTiles.find(neighbor) != emptyTiles.end() && visited.find(neighbor) == visited.end() && !isDirectionBlocked(current, direction))
                {
                    stack.push(neighbor);
                }
            }
        }

        return visited;
    }

    std::set<Position> MovementManager::getAvailableMoves(Tile tile) const
    {
        /*
        string nie jest obsługiwany w switch, do zamiany na enum kiedyś
        */
        if (tile.type == "QUEEN")
        {
            return getQueenBeeMoves(tile.position);
        }
        else if (tile.type == "BEETLE")
        {
            return getBeetleMoves(tile.position);
        }
        else if (tile.type == "SPIDER")
        {
            return getSpiderMoves(tile.position);
        }
        else if (tile.type == "GRASSHOPPER")
        {
            return getGrasshopperMoves(tile.position);
        }
        else if (tile.type == "ANT")
        {
            return getAntMoves(tile.position);
        }
        else
        {
            return std::set<Position>();
        }
    }

}