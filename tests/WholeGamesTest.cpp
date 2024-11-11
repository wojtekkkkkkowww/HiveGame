#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include "BaseTest.hpp"

using namespace hive;

class WholeGamesTest : public BaseTest
{
public:
    WholeGamesTest() : BaseTest()
    {
    //saveState = true;
    }
    
};

namespace
{
    /*
    Games are collected from the
    https://www.boardspace.net/
    */

    // HV-SmartBot-guest-2024-10-19-1929
    std::vector<std::string> officialGame1 = {
        "WB1",
        "BG1 WB1\\",
        "WQ -WB1",
        "BQ BG1-",
        "WS1 WB1/",
        "BS1 /BG1",
        "WQ \\WB1",
        "BG2 /BQ",
        "WS1 BQ-",
        "BG2 \\WQ",
        "WB2 WQ-",
        "BS1 -WQ",
        "WA1 WS1-",
        "BG3 \\BS1",
        "WA1 BQ/",
        "BG3 WQ/",
        "WB2 BG3.",
        "BA1 \\BS1",
        "WA2 \\WA1",
        "BB1 /BS1",
        "WG1 WA2-",
        "BB1 /WQ",
        "WG1 /BQ",
        "BB1 WQ.",
        "WG2 WB2/",
        "BB1 -WA2", 
        "WB1 BG1.",
        "BA1 WQ\\",
        "WA2 BQ\\",
        "BA2 \\BS1",
        "WB1 \\BQ"};

    // HV-guest-Dumbot-2024-10-19-1941.sgf
    std::vector<std::string> officialGame2 = {
        "WG1",
        "BG1 /WG1",
        "WQ WG1-",
        "BA1 /BG1",
        "WA1 \\WQ",
        "BA2 /BA1",
        "WA1 /BA2",
        "BQ \\BA1",
        "WS1 \\WQ",
        "BS1 -BQ",
        "WS1 \\BQ",
        "BS1 WS1/",
        "WB1 -WS1",
        "BB1 /BQ",
        "WB1 -BQ",
        "BB1 WB1.",
        "WG2 WQ-",
        "BB2 BG1\\",
        "WG2 BQ/",
        "BG1 \\WQ",
        "WG3 /WA1",
        "BS2 BG1/",
        "WG3 BQ-",
        "BG2 /BB1",
        "WQ BS2\\",
        "BA3 BS2/",
        "WQ BA3\\",
        "BG3 \\BS2", 
        "WQ BA3-",
        "BB2 BA1.",
        "WQ BA3/",
        "BS1 -BG3",
        "WB2 -WA1",
        "BS1 \\WQ",
        "WB2 -BA2",
        "BA2 WA1-",
        "WG2 /BQ",
        "BB2 WG2.",
        "WA2 WG1\\",
        "BA1 WQ/",
        "WA2 BQ/",
        "BG2 WA1\\",
        "WA3 WG1\\",
        "BA2 WA3\\",
        "WS2 -WA1",
        "BG3 WQ\\",
        "WS2 BG2\\",
        "BB2 BB1.",
        "WS2 WG2\\",
        "BA1 -WB2",
        "WS2 /BA2",
        "BA1 WQ/",
        "WS2 WG2\\",
        "BB2 WS1.",
        "WB2 WS2.",
        "BA2 WQ-",
        "WB2 BQ\\"};

        //U!HV-Dumbot-seer-2024-10-20-0239.sgf
        std::vector<std::string> officialGame3 = {
        "WS1",
        "BG1 -WS1",
        "WB1 WS1\\",
        "BQ \\BG1",
        "WB2 WS1/",
        "BA1 -BQ",
        "WQ WB1-",
        "BA2 /BQ",
        "WA1 WB2-",
        "BA2 WA1/",
        "WG1 \\WQ",
        "BS1 /BG1",
        "WG2 WQ/",
        "BS1 /WQ",
        "WG2 /BS1",
        "BA3 \\BA2",
        "WQ BS1-",
        "BA1 WQ-",
        "WS2 WA1\\",
        "BB1 BA1\\",
        "WA2 WG2\\",
        "BA3 WA2\\",
        "WA3 WS2-",
        "BB1 BA1.",
        "WG3 -WA2",
        "BB1 WQ.",
        "WA3 BA1\\",
        "BS2 BA3-",
        "WA3 BS2/",
        "BA1 \\WA3",
        "WA3 BB1/",
        "BA1 WA3-",
        "WG3 \\BB1",
        "BS2 BB1\\",
        "WG3 BS2\\",
        "BQ \\WS1",
        "WB2 BQ.",
        "BG1 WS2-",
        "WB1 WS1.",
        "BA1 WA3\\",
        "WB1 \\BS1",
        "BG1 WG2-",
        "WA3 BA1/",
        "BB2 /BA3",
        "WS2 BA2/",
        "BB2 BA3.",
        "WA3 WA1\\",
        "BB2 WA2.",
        "WG2 BA3\\",
        "BB2 BG1.",
        "WA3 BA1/",
        "BB2 BS1.",
        "WA3 \\WG1",
        "BB2 WG1\\",
        "WS1 \\WB2",
        "BB1 \\BA1"
        };

}

TEST_F(WholeGamesTest, OfficialGame1)
{
    std::cout << "OfficialGame1 " << officialGame1.size() << std::endl;
    playGame(officialGame1);
    ASSERT_EQ(getGameStatus(), "WHITE_WINS");
}

TEST_F(WholeGamesTest, OfficialGame2)
{
    std::cout << "OfficialGame2 " << officialGame2.size() << std::endl;
    playGame(officialGame2);
    ASSERT_EQ(getGameStatus(), "WHITE_WINS");
}

TEST_F(WholeGamesTest, OfficialGame3)
{
    std::cout << "OfficialGame3 " << officialGame3.size() << std::endl;
    playGame(officialGame3);    
    ASSERT_EQ(getGameStatus(), "BLACK_WINS");
}