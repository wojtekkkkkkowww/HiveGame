#include "MovementFunctions.hpp"

namespace hive
{

    std::set<Position> getQueenBeeMoves(Board &board, Position position)
    {
        std::set<Position> neighbours = board.getNeighbours(position);
        std::set<Position> moves;

        for (auto &neighbour : neighbours)
        {
            if (board.isEmpty(neighbour))
            {
                moves.insert(neighbour);
            }
        }
        return moves;
    }

    std::set<Position> getAntMoves(Board &board, Position position)
    {   position.x += 0;
        position.y += 0;

        return board.emptyTiles;
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

    std::set<Position> getSpiderMoves(Board &board, Position position)
    {
        std::set<Position> visited;

        std::function<void(Position, int)> dfs = [&](Position pos, int depth)
        {
            if(board.calculateNeighbours(pos,"BLACK") + board.calculateNeighbours(pos,"WHITE") == 0)
            {
                return;
            }
            
            if (depth == 3)
            {
                visited.insert(pos);
                return;
            }

            visited.insert(pos);
        
            for (auto &neighbour : board.getNeighbours(pos))
            {
                if (visited.find(neighbour) == visited.end() && board.isEmpty(neighbour))
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

    std::set<Position> getGrasshopperMoves(Board &board, Position position)
    {
        std::set<Position> moves;
        std::vector<Position> directions = {N, NE, SE, S, SW, NW};

        for (auto &direction : directions)
        {
            Position current = position;
            while (!board.isEmpty(current))
            {
                current.x += direction.x;
                current.y += direction.y;
            }

            if (board.isEmpty(current))
            {
                moves.insert(current);
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

    std::set<Position> getBeetleMoves(Board &board, Position position)
    {
        std::set<Position> moves;
        std::vector<Position> directions = {N, NE, SE, S, SW, NW};
        auto beetle_level = board.getLevel(position);

        for (auto &direction : directions)
        {
            Position newPos = {position.x + direction.x, position.y + direction.y};
            if ((board.isEmpty(newPos) || abs(beetle_level - board.getLevel(newPos)) <= 1))
            {
                moves.insert(newPos);
            }
        }

        return moves;
    }

}