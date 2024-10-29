#include <queue>
#include <stack>
#include "MovementManager.hpp"

namespace hive
{
    std::set<Position> MovementManager::getQueenBeeMoves(Position position) const
    {
        std::set<Position> directions = {NW, NE, E, SE, SW, W};
        std::set<Position> moves;

        for (auto &direction : directions)
        {
            Position newPos = position + direction;
            if (isEmpty(newPos) && !isDirectionBlocked(position, direction, 1))
            {
                moves.insert(newPos);
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
        //THIS SHOULD BE APLIED FOR ALL TYPES OF TILES
        // An idea of refactoring : Beatle beatle(position,this) // it pass board as reference
        //  return beatle.getMoves(); 
        
        std::set<Position> moves;
        std::vector<Position> directions = {NW, NE, E, SE, SW, W};
        auto beetle_level = getLevel(position);

        for (auto &direction : directions)
        {
            Position newPos = position + direction;
            if (beatleJumpUp(newPos, beetle_level))
            {
                int newBeetleLevel = getLevel(newPos) + 1;
                if (!isDirectionBlocked(position, direction, newBeetleLevel))
                {
                    moves.insert(newPos);
                }

                continue;
            }

            if (!isDirectionBlocked(position, direction, beetle_level))
                moves.insert(newPos);
        }

        return moves;
    }

    bool MovementManager::beatleJumpUp(const hive::Position &newPos, int beetle_level) const
    {
        return getLevel(newPos) >= beetle_level;
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

                    // Jeżeli pająk jest sąsiadem to go nie liczymy
                    int isSpiderNeighbour = (spiderNeighbours.find(neighbor) != spiderNeighbours.end()) ? 1 : 0;
                    if (calculateNeighbours(neighbor, 'B') + calculateNeighbours(neighbor, 'W') - isSpiderNeighbour > 0)
                    {
                        Position direction = neighbor - currentPos;
                        if (isDirectionBlocked(currentPos, direction, 1))
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
        std::vector<Position> directions = {NW, NE, E, SE, SW, W};

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

        std::vector<Position> directions = {NW, NE, E, SE, SW, W};
        for (auto &direction : directions)
        {
            Position neighbour = position + direction;
            if (emptyTiles.find(neighbour) != emptyTiles.end() && !isDirectionBlocked(position, direction, 1))
            {
                stack.push(neighbour);
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

            std::vector<Position> directions = {NW, NE, E, SE, SW, W};

            for (const Position &direction : directions)
            {
                Position neighbor = current + direction;
                if (emptyTiles.find(neighbor) != emptyTiles.end() && visited.find(neighbor) == visited.end() && !isDirectionBlocked(current, direction, 1))
                {
                    stack.push(neighbor);
                }
            }
        }

        return visited;
    }

    std::set<Position> MovementManager::getAvailableMoves(Tile tile) const
    {
        switch (tile.type)
        {
        case 'Q':
            return getQueenBeeMoves(tile.position);
        case 'B':
            return getBeetleMoves(tile.position);
        case 'S':
            return getSpiderMoves(tile.position);
        case 'G':
            return getGrasshopperMoves(tile.position);
        case 'A':
            return getAntMoves(tile.position);
        default:
            return std::set<Position>();
        }
    }

}