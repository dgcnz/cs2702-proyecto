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

TEST_F(DiskBasedBtree, Insert100)
{
    bool                         trunc_file = true;
    std::shared_ptr<pagemanager> pm =
        std::make_shared<pagemanager>("btree.index", trunc_file);
    btree<int, BTREE_ORDER> bt(pm);
    load("tests/t100.csv", bt);

}

TEST_F(DiskBasedBtree, Insert1000)
{
    bool                         trunc_file = true;
    std::shared_ptr<pagemanager> pm =
        std::make_shared<pagemanager>("btree.index", trunc_file);
    btree<int, BTREE_ORDER> bt(pm);
    load("tests/t1000.csv", bt);

}

TEST_F(DiskBasedBtree, Insert10000)
{
    bool                         trunc_file = true;
    std::shared_ptr<pagemanager> pm =
        std::make_shared<pagemanager>("btree.index", trunc_file);
    btree<int, BTREE_ORDER> bt(pm);
    load("tests/t10000.csv", bt);

}
