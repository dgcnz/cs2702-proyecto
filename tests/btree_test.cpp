#include <algorithm>
#include <btree.h>
#include <fmt/core.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <iterator>
#include <pagemanager.h>
#include <string>

// PAGE_SIZE 64 bytes
#define PAGE_SIZE 64

// Other examples:
// PAGE_SIZE 1024 bytes => 1Kb
// PAGE_SIZE 1024*1024 bytes => 1Mb

// PAGE_SIZE = 2 * sizeof(long) +  (BTREE_ORDER + 1) * sizeof(int) +
// (BTREE_ORDER + 2) * sizeof(long) PAGE_SIZE = 2 * sizeof(long) + (BTREE_ORDER)
// * sizeof(int) + sizeof(int) + (BTREE_ORDER) * sizeof(long) + 2 * sizeof(long)
// PAGE_SIZE = (BTREE_ORDER) * (sizeof(int) + sizeof(long))  + 2 * sizeof(long)
// + sizeof(int) +  2 * sizeof(long)
//  BTREE_ORDER = PAGE_SIZE - (2 * sizeof(long) + sizeof(int) +  2 *
//  sizeof(long)) /  (sizeof(int) + sizeof(long))

#define BTREE_ORDER                                                            \
    ((PAGE_SIZE - (2 * sizeof(long) + sizeof(int) + 2 * sizeof(long))) /       \
     (sizeof(int) + sizeof(long)))

struct DiskBasedBtree : public ::testing::Test
{
};
using namespace db::disk;

TEST_F(DiskBasedBtree, IndexingRandomElements)
{
    bool                         trunc_file = true;
    std::shared_ptr<pagemanager> pm =
        std::make_shared<pagemanager>("btree.index", trunc_file);
    std::cout << "PAGE_SIZE: " << PAGE_SIZE << std::endl;
    std::cout << "BTREE_ORDER: " << BTREE_ORDER << std::endl;
    std::cout << "BTREE_ORDER: " << BTREE_ORDER << std::endl;
    using char_btree = btree<char, BTREE_ORDER>;
    char_btree  bt(pm);
    std::string values = "zxcnmvfjdaqpirueeeeaaaa";
    for (auto c : values)
        bt.insert(c);

    bt.print_tree();
    std::string          s;
    char_btree::iterator first = bt.begin();
    char_btree::iterator last  = bt.end();
    std::copy(first, last, std::back_inserter(s));
    std::sort(values.begin(), values.end());
    EXPECT_EQ(s, values);
}

TEST_F(DiskBasedBtree, Persistence)
{
    std::shared_ptr<pagemanager> pm =
        std::make_shared<pagemanager>("btree.index");
    using char_btree = btree<char, BTREE_ORDER>;
    char_btree  bt(pm);
    std::string values = "123456";
    for (auto c : values)
        bt.insert(c);
    bt.print_tree();

    char_btree::iterator first = bt.begin();
    char_btree::iterator last  = bt.end();
    std::string          s;
    std::copy(first, last, std::back_inserter(s));

    std::string all_values = "zxcnmvfjdaqpirueeeeaaaa123456";
    std::sort(all_values.begin(), all_values.end());
    EXPECT_EQ(s, all_values);
}

TEST_F(DiskBasedBtree, RangeSearch)
{
    std::shared_ptr<pagemanager> pm =
        std::make_shared<pagemanager>("btree.index");
    using char_btree = btree<char, BTREE_ORDER>;
    char_btree  bt(pm);
    std::string values = "aaaaacdeeee";
    bt.print_tree();

    char_btree::iterator first = bt.lower_bound('a');
    char_btree::iterator last  = bt.upper_bound('e');
    std::string          s;
    EXPECT_EQ(*first, 'a');
    EXPECT_EQ(*last, 'f');
    std::copy(first, last, std::back_inserter(s));

    EXPECT_EQ(s, values);
}

struct Student
{
    long        id;
    bool        passed;
    std::string name;
    std::string surname;
    int         n_credits;
};
struct Pair
{
    long id;
    long page_id;
    bool operator<(const Pair &p) const
    {
        return id < p.id;
    } // compare using only id.

    bool operator<=(const Pair &p) const
    {
        return id <= p.id;
    } // compare using only id.

    bool operator>(const Pair &p) const { return id > p.id; }

    bool operator==(const Pair &p) const { return id == p.id; }
};
// TEST_F(DiskBasedBtree, Dictionary1)
// {
//
//     std::shared_ptr<pagemanager> pm =
//         std::make_shared<pagemanager>("btree2.index");
//     btree<Pair, BTREE_ORDER> bt(pm);
//
//     pagemanager record_manager("students.bin");
//     long        page_id;
//     Student     p{10, false, "alex", "orihuela", 150};
//     page_id = 0;
//     record_manager.save(page_id, p);
//
//     bt.insert(Pair{p.id, page_id});
//
//     Student p1{20, true, "Luis", "sanchez", 100};
//     page_id = 1;
//     record_manager.save(page_id, p1);
//     bt.insert(Pair{p1.id, page_id});
//
//     Student p2{30, false, "alvaro", "valera", 150};
//     page_id = 2;
//     record_manager.save(page_id, p2);
//     bt.insert(Pair{p2.id, page_id});
//
//     Student p3{40, true, "javier", "Mayori", 15};
//     page_id = 3;
//     record_manager.save(page_id, p3);
//     bt.insert(Pair{p3.id, page_id});
//     // repeat ... Index one millon records (or one millon students)
// }
//
// TEST_F(DiskBasedBtree, Dictionary2)
// {
//     pagemanager record_manager("students.bin");
//
//     std::shared_ptr<pagemanager> pm =
//         std::make_shared<pagemanager>("btree.index");
//     btree<Pair, BTREE_ORDER>           bt(pm);
//     btree<Pair, BTREE_ORDER>::iterator iter = bt.find(Pair{50, -1});
//     btree<Pair, BTREE_ORDER>::iterator end  = bt.find(Pair{100, -1});
//     for (; iter != end; iter++)
//     {
//         auto    pair = *iter;
//         Student s;
//         record_manager.recover(pair.page_id, s);
//         std::cout << pair.id << s.passed << s.name << s.surname <<
//         s.n_credits
//                   << std::endl;
//     }
// }

// struct Register
// {
//     int  key;
//     char text[20];
//     bool operator<(const Register &r) const { return key < r.key; }
//     bool operator<=(const Register &r) const { return key <= r.key; }
//     bool operator>(const Register &r) const { return key > r.key; }
//     bool operator==(const Register &r) const { return key == r.key; }
//     std::vector<std::string> serialize()
//     {
//         return {std::to_string(key), std::string(text)};
//     }
// };

TEST_F(DiskBasedBtree, Delete)
{
    std::shared_ptr<pagemanager> pm =
        std::make_shared<pagemanager>("btree.index", true);
    using char_btree = btree<char, BTREE_ORDER>;
    char_btree bt(pm);
    std::cout << "DELETE" << std::endl;
    std::string val    = "abcdef";
    std::string valaft = "acdef";
    for (auto c : val)
        bt.insert(c);

    bt.remove('b');
    std::vector<char> registers = bt.entries(bt.begin(), bt.end());
    std::string    s;
    std::copy(
        std::begin(registers), std::end(registers), std::back_inserter(s));

    EXPECT_EQ(s, valaft);
}

// TEST_F(DiskBasedBtree, IndexingRandomElements2) {
//   bool trunc_file = true;
//   std::shared_ptr<pagemanager> pm =
//   std::make_shared<pagemanager>("btree.index", trunc_file); std::cout <<
//   "PAGE_SIZE: " << PAGE_SIZE << std::endl; std::cout << "BTREE_ORDER: " <<
//   BTREE_ORDER << std::endl; btree<char, BTREE_ORDER> bt(pm); std::string
//   values = "148"; for(auto c : values) {
//     bt.insert(c);
//   }
//   bt.print_tree();
//
// 	char val = '1';
// 	cout << "to erase -> " << val << '\n';
// 	bt.erase (val);
//   bt.print_tree ();
//
// 	std::ostringstream out;
//   bt.print(out);
//   std::sort(values.begin(), values.end());
//   EXPECT_EQ(out.str(), "48");
// }
