#include "StaticMap.hpp"
#include <gtest/gtest.h>
#include <string>

namespace
{
using TestMap = hive::StaticMap<int, std::string, 3>;
}

TEST(StaticMapTest, InsertsUpdatesFindsAndErases)
{
    TestMap map;
    map.insert(1, "one");
    map[2] = "two";
    map.insert(1, "updated");

    ASSERT_EQ(map.size(), 2U);
    EXPECT_TRUE(map.contains(1));
    EXPECT_EQ(map.at(1), "updated");
    EXPECT_EQ(map.at(2), "two");

    map.erase(1);
    EXPECT_FALSE(map.contains(1));
    EXPECT_EQ(map.size(), 1U);
    EXPECT_EQ(map.at(2), "two");
}

TEST(StaticMapTest, EnforcesCapacityAndReusesErasedSlot)
{
    TestMap map{{1, "one"}, {2, "two"}, {3, "three"}};

    EXPECT_THROW(map[4], std::length_error);
    map.erase(2);
    EXPECT_NO_THROW(map[4] = "four");
    EXPECT_EQ(map.at(4), "four");
}

TEST(StaticMapTest, IteratesOnlyActiveEntries)
{
    TestMap map{{1, "one"}, {2, "two"}};
    std::size_t visited = 0;

    for (const auto &[key, value] : map)
    {
        EXPECT_FALSE(value.empty());
        EXPECT_GT(key, 0);
        ++visited;
    }

    EXPECT_EQ(visited, map.size());
}
