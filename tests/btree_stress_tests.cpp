#include <algorithm>
#include <btree.h>
#include <fmt/core.h>
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <iterator>
#include <pagemanager.h>
#include <string>

#define PAGE_SIZE 64

#define BTREE_ORDER                                                            \
    ((PAGE_SIZE - (2 * sizeof(long) + sizeof(int) + 2 * sizeof(long))) /       \
     (sizeof(int) + sizeof(long)))

struct DiskBasedBtree : public ::testing::Test
{
};
using namespace db::disk;

btree<int, BTREE_ORDER> bt100(std::make_shared<pagemanager>("btree100.index",
                                                            true));
btree<int, BTREE_ORDER> bt1000(std::make_shared<pagemanager>("btree100.index",
                                                             true));
btree<int, BTREE_ORDER> bt10000(std::make_shared<pagemanager>("btree100.index",
                                                              true));

void load(std::string filename, btree<int, BTREE_ORDER> &bt)
{
    std::ifstream file;
    file.open(filename);
    if (!file.good())
        throw std::runtime_error("invalid file");
    char buf[128];
    while (file.getline(buf, 128, ','))
    {
        bt.insert(std::stoi(buf));
        file.getline(buf, 128);
    }
}

TEST_F(DiskBasedBtree, Insert100) { load("tests/t100.csv", bt100); }

TEST_F(DiskBasedBtree, Insert1000) { load("tests/t1000.csv", bt1000); }

TEST_F(DiskBasedBtree, Insert10000) { load("tests/t10000.csv", bt10000); }

TEST_F(DiskBasedBtree, Search10000)
{
    srand(time(NULL));
    int  n  = rand() % 100;
    auto it = bt10000.find(n);
    EXPECT_EQ(*it, n);
}

TEST_F(DiskBasedBtree, RangeSearch10000)
{
    srand(time(NULL));
    int n           = rand() % 100;
    auto [itl, itr] = bt10000.find(n, 100);
    EXPECT_EQ(*itl, n);
}

TEST_F(DiskBasedBtree, Delete10000)
{
    srand(time(NULL));
    int  n  = rand() % 100;
    auto it = bt10000.remove(n);
}
