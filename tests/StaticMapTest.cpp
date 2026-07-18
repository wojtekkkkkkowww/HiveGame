#include "StaticMap.hpp"
#include <gtest/gtest.h>
#include <map>
#include <random>
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

TEST(StaticMapTest, MatchesStdMapAcrossRandomOperations)
{
    hive::StaticMap<int, int, 8> actual;
    std::map<int, int> expected;
    std::mt19937 random(0xC0FFEE);
    std::uniform_int_distribution<int> keyDistribution(0, 11);
    std::uniform_int_distribution<int> valueDistribution(-1000, 1000);
    std::uniform_int_distribution<int> operationDistribution(0, 2);

    for (int iteration = 0; iteration < 10'000; ++iteration)
    {
        const int key = keyDistribution(random);
        const int operation = operationDistribution(random);

        if (operation == 0)
        {
            actual.erase(key);
            expected.erase(key);
        }
        else if (operation == 1)
        {
            const int value = valueDistribution(random);
            if (expected.contains(key) || expected.size() < actual.capacity())
            {
                actual.insert(key, value);
                expected[key] = value;
            }
            else
            {
                EXPECT_THROW(actual.insert(key, value), std::length_error);
            }
        }
        else
        {
            EXPECT_EQ(actual.contains(key), expected.contains(key));
            if (expected.contains(key))
            {
                EXPECT_EQ(actual.at(key), expected.at(key));
            }
            else
            {
                EXPECT_THROW(actual.at(key), std::out_of_range);
            }
        }

        ASSERT_EQ(actual.size(), expected.size());
        for (const auto &[storedKey, storedValue] : actual)
        {
            ASSERT_TRUE(expected.contains(storedKey));
            EXPECT_EQ(storedValue, expected.at(storedKey));
        }
    }
}
