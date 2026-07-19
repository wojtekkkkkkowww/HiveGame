#include "ArticulationPoints.hpp"
#include "BaseTest.hpp"
#include <gtest/gtest.h>

using namespace hive;

class ArticulationPointsTest : public BaseTest
{
  public:
    ArticulationPointsTest() : BaseTest(), articulationPointFinder(board) {}

    ArticulationPointFinder articulationPointFinder;

    void checkArticularPoints(const std::map<Position, std::deque<Tile>> &tiles,
                              const std::set<Position> &expectedPositions)
    {
        startNewGame();
        board.setBoardTiles(tiles);

        articulationPointFinder.findArticulationPoints();
        EXPECT_EQ(board.articulationPoints, expectedPositions);
        if (board.articulationPoints == expectedPositions)
        {
            std::cerr << "correct check \n";
        }
        else
        {
            for (const auto &pos : board.articulationPoints)
            {
                std::cerr << "articulation point: " << pos.x << " " << pos.y << std::endl;
            }
        }
    }
};

namespace
{
const std::map<Position, std::deque<Tile>> tiles1 = {
    {Position{0, 0}, {Tile(PieceType::Queen, PlayerColor::White)}},
    {Position{1, 0}, {Tile(PieceType::Ant, PlayerColor::Black)}},
    {Position{0, 1}, {Tile(PieceType::Ant, PlayerColor::White)}},
    {Position{-1, 0}, {Tile(PieceType::Grasshopper, PlayerColor::Black)}},
    {Position{0, -1}, {Tile(PieceType::Grasshopper, PlayerColor::White)}}};
const std::set<Position> expectedPositions1 = {{0, 0}};

const std::map<Position, std::deque<Tile>> tiles2 = {
    {Position{0, 0}, {Tile(PieceType::Queen, PlayerColor::White)}},
    {Position{1, 0}, {Tile(PieceType::Ant, PlayerColor::Black)}},
    {Position{0, 1}, {Tile(PieceType::Ant, PlayerColor::White)}},
    {Position{-1, 0}, {Tile(PieceType::Grasshopper, PlayerColor::Black)}},
    {Position{0, -1}, {Tile(PieceType::Grasshopper, PlayerColor::White)}},
    {Position{-1, 1}, {Tile(PieceType::Queen, PlayerColor::Black)}}};
const std::set<Position> expectedPositions2 = {};

const std::map<Position, std::deque<Tile>> tiles3 = {
    {Position{-1, 0}, {Tile(PieceType::Grasshopper, PlayerColor::Black)}},
    {Position{-1, 1}, {Tile(PieceType::Grasshopper, PlayerColor::White)}},
    {Position{0, 1}, {Tile(PieceType::Ant, PlayerColor::White)}},
    {Position{1, 0}, {Tile(PieceType::Ant, PlayerColor::Black)}}};
const std::set<Position> expectedPositions3 = {{-1, 1}, {0, 1}};

const std::map<Position, std::deque<Tile>> tiles4 = {
    {Position{-1, 0}, {Tile(PieceType::Ant, PlayerColor::Black)}},
    {Position{-2, 0}, {Tile(PieceType::Ant, PlayerColor::Black)}},
    {Position{-2, 1}, {Tile(PieceType::Beetle, PlayerColor::Black)}},
    {Position{-3, 1}, {Tile(PieceType::Queen, PlayerColor::Black)}},
    {Position{-1, 3}, {Tile(PieceType::Ant, PlayerColor::Black)}},
    {Position{0, 0}, {Tile(PieceType::Ant, PlayerColor::White)}},
    {Position{0, 1}, {Tile(PieceType::Ant, PlayerColor::White)}},
    {Position{0, 2}, {Tile(PieceType::Queen, PlayerColor::White)}},
    {Position{-4, 2}, {Tile(PieceType::Ant, PlayerColor::White)}}};
const std::set<Position> expectedPositions4 = {{-3, 1}, {-1, 0}, {0, 0}, {0, 1}, {0, 2}};

const std::map<Position, std::deque<Tile>> tiles5 = {
    {Position{0, 0}, {Tile(PieceType::Queen, PlayerColor::White)}},
    {Position{1, 0}, {Tile(PieceType::Ant, PlayerColor::Black)}},
    {Position{-1, 0}, {Tile(PieceType::Ant, PlayerColor::White)}},
    {Position{-2, 0}, {Tile(PieceType::Ant, PlayerColor::Black)}}};
const std::set<Position> expectedPositions5 = {{0, 0}, {-1, 0}};

} // namespace

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