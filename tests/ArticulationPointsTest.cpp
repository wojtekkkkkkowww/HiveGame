#include <gtest/gtest.h>
#include "BaseTest.hpp"
#include "Game.hpp"
#include "ArticulationPoints.hpp"
#include "MoveValidator.hpp"

using namespace hive;

class ArticulationPointsTest : public BaseTest
{
public:
    ArticulationPointsTest() : BaseTest(), articulationPointFinder(board)
    {
    }

    ArticulationPointFinder articulationPointFinder;
    void checkArticularPoints(std::map<Position, std::deque<Tile>> &tiles, std::set<Position> &expectedPositions)
    {
        startNewGame();
        board.setBoardTiles(tiles);
        
        std::set<Position> articulationPoints = articulationPointFinder.findArticulationPoints();
        EXPECT_EQ(articulationPoints, expectedPositions);
        if (articulationPoints == expectedPositions)
        {
            std::cerr << "correct check \n";
        }
        else
        {
            for (auto &pos : articulationPoints)
            {
                std::cerr << "articulation point: " << pos.x << " " << pos.y << std::endl;
            }
        }
    }
};

namespace
{
    std::map<Position, std::deque<Tile>> tiles1 = {
        {Position{0, 0}, {Tile('W', 'Q')}},
        {Position{1, 0}, {Tile('B', 'A')}},
        {Position{0, 1}, {Tile('W', 'A')}},
        {Position{-1, 0}, {Tile('B', 'G')}},
        {Position{0, -1}, {Tile('W', 'G')}}};
    std::set<Position> expectedPositions1 = {{0, 0}};

    std::map<Position, std::deque<Tile>> tiles2 = {
        {Position{0, 0}, {Tile('W', 'Q')}},
        {Position{1, 0}, {Tile('B', 'A')}},
        {Position{0, 1}, {Tile('W', 'A')}},
        {Position{-1, 0}, {Tile('B', 'G')}},
        {Position{0, -1}, {Tile('W', 'G')}},
        {Position{-1, 1}, {Tile('B', 'Q')}}};
    std::set<Position> expectedPositions2 = {};

    std::map<Position, std::deque<Tile>> tiles3 = {
        {Position{-1, 0}, {Tile('B', 'G')}},
        {Position{-1, 1}, {Tile('W', 'G')}},
        {Position{0, 1}, {Tile('W', 'A')}},
        {Position{1, 0}, {Tile('B', 'A')}}};
    std::set<Position> expectedPositions3 = {{-1, 1}, {0, 1}};

    std::map<Position, std::deque<Tile>> tiles4 = {
        {Position{-1, 0}, {Tile('B', 'A')}},
        {Position{-2, 0}, {Tile('B', 'A')}},
        {Position{-2, 1}, {Tile('B', 'B')}},
        {Position{-3, 1}, {Tile('B', 'Q')}},
        {Position{-1, 3}, {Tile('B', 'A')}},
        {Position{0, 0}, {Tile('W', 'A')}},
        {Position{0, 1}, {Tile('W', 'A')}},
        {Position{0, 2}, {Tile('W', 'Q')}},
        {Position{-4, 2}, {Tile('W', 'A')}}};
    std::set<Position> expectedPositions4 = {{-3, 1}, {-1, 0}, {0, 0}, {0, 1}, {0, 2}};

    std::map<Position, std::deque<Tile>> tiles5 = {
        {Position{0, 0}, {Tile('W', 'Q')}},
        {Position{1, 0}, {Tile('B', 'A')}},
        {Position{-1, 0}, {Tile('W', 'A')}},
        {Position{-2, 0}, {Tile('B', 'A')}}};
    std::set<Position> expectedPositions5 = {{0, 0}, {-1, 0}};

}

TEST_F(ArticulationPointsTest, TestArticulationPoints1)
{
    std::cerr << "test 1 \n";
    checkArticularPoints(tiles1, expectedPositions1);
}

TEST_F(ArticulationPointsTest, TestArticulationPoints2)
{
    std::cerr << "test 2 \n";
    checkArticularPoints(tiles2, expectedPositions2);
}

TEST_F(ArticulationPointsTest, TestArticulationPoints3)
{
    checkArticularPoints(tiles3, expectedPositions3);
}

TEST_F(ArticulationPointsTest, TestArticulationPoints4)
{
    checkArticularPoints(tiles4, expectedPositions4);
}

TEST_F(ArticulationPointsTest, TestArticulationPoints5)
{
    checkArticularPoints(tiles5, expectedPositions5);
}