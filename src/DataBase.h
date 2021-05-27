#pragma once

#include <btree.h>
#include <pagemanager.h>

#define PAGE_SIZE  64
#define BTREE_ORDER  ((PAGE_SIZE - (2 * sizeof(long) + sizeof(int) +  2 * sizeof(long)) ) /  (sizeof(int) + sizeof(long)))

class DataBase {
public:
    DataBase (std::shared_ptr<db::disk::pagemanager> pm, char *name);
    int load_file(char *name);

private:
    char *filename;
    std::fstream file;
    db::disk::btree<char *, BTREE_ORDER> bt;
};

