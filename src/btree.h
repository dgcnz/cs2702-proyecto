#pragma once

#include "pagemanager.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace db
{
namespace disk
{

template <class T, int BTREE_ORDER>
class btree;

template <typename T, int BTREE_ORDER>
btree<T, BTREE_ORDER> make_btree(std::string filename = "btree.index")
{
    std::shared_ptr<pagemanager> pm =
        std::make_shared<pagemanager>(filename, true);
    return btree<T, BTREE_ORDER>(pm);
}

template <class T, int BTREE_ORDER>
class node;

template <class T, int BTREE_ORDER>
struct btree_iterator
{
    using node              = typename btree<T, BTREE_ORDER>::node;
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T *;
    using reference         = T &;

    btree<T, BTREE_ORDER> const &bt;
    long                         page_id;
    int                          offset;
    btree_iterator(btree<T, BTREE_ORDER> const &bt,
                   long                         page_id,
                   int                          offset = 0)
        : bt(bt), page_id(page_id), offset(offset)
    {
    }
    btree_iterator &operator=(btree_iterator other)
    {
        page_id = other.page_id;
        offset  = other.offset;
        return *this;
    }

    btree_iterator &operator++()
    {
        node n = bt.read_node(page_id);
        offset++;
        if (n.count <= offset)
        {
            page_id = n.next_id;
            offset  = 0;
        }
        return *this;
    }

    btree_iterator operator++(int)
    {
        btree_iterator it{*this};
        ++(*this);
        return it;
    }

    inline bool operator==(btree_iterator<T, BTREE_ORDER> const &other) const
    {
        return (page_id == other.page_id) and
               (page_id == -1 or offset == other.offset);
    }
    inline bool operator!=(btree_iterator<T, BTREE_ORDER> const &other) const
    {
        return !(operator==(other));
    }

    T operator*() const
    {
        node n = bt.read_node(page_id);
        return n.data[offset];
    }

    bool valid() const
    {
        node ptr = bt.read_node(page_id);
        return ptr.valid[offset];
    }
    void invalidate()
    {
        node ptr          = bt.read_node(page_id);
        ptr.valid[offset] = false;
        bt.write_node(ptr.page_id, ptr);
    }
};

template <class T, int BTREE_ORDER>
struct node
{
    using iterator = disk::btree_iterator<T, BTREE_ORDER>;
    long page_id{-1};
    long next_id{-1};
    long count{0};

    std::array<T, BTREE_ORDER + 1>    data;
    std::array<long, BTREE_ORDER + 2> children;
    std::array<bool, BTREE_ORDER + 1> valid;

    node(long page_id) : page_id{page_id}, count(0), children{0}, data{0}
    {
        std::fill(std::begin(children), std::end(children), 0);
        std::fill(std::begin(valid), std::end(valid), 1);
    }

    void insert_in_node(int pos, const T &value)
    {
        int j = count;
        while (j > pos)
        {
            data[j]         = data[j - 1];
            children[j + 1] = children[j];
            j--;
        }
        data[j]         = value;
        children[j + 1] = children[j];

        count++;
    }
    int lower_bound(T const &value) const
    {
        int pos = 0;
        while (pos < count and data[pos] < value)
            ++pos;
        return pos;
    }
    bool is_overflow() const { return count > BTREE_ORDER; }
    bool is_leaf() const
    {
        return std::count(begin(children), end(children), 0) == children.size();
    }
};

template <class T, int BTREE_ORDER = 3>
class btree
{
  public:
    using node     = disk::node<T, BTREE_ORDER>;
    using iterator = disk::btree_iterator<T, BTREE_ORDER>;
    struct Metadata
    {
        long root_id{1};
        long count{0};
    } header;

    enum state
    {
        BT_OVERFLOW,
        BT_UNDERFLOW,
        NORMAL,
    };

  private:
    std::shared_ptr<pagemanager> pm;

  public:
    btree(std::shared_ptr<pagemanager> pm) : pm{pm}
    {
        if (pm->is_empty())
        {
            node root{header.root_id};
            pm->save(root.page_id, root);
            header.count++;
            pm->save(0, header);
        }
        else
            pm->recover(0, header);
    }

    node new_node()
    {
        header.count++;
        node ret{header.count};
        pm->save(0, header);
        return ret;
    }

    node read_node(long page_id) const
    {
        node n{-1};
        if (page_id != -1)
            pm->recover(page_id, n);
        return n;
    }

    void write_node(long page_id, node n) const { pm->save(page_id, n); }
    void delete_node(long page_id, node n) const { pm->erase(page_id, n); }

    void insert(const T &value)
    {
        node root  = read_node(header.root_id);
        int  state = insert(root, value);

        if (state == BT_OVERFLOW)
            split_root();
    }

    int insert(node &ptr, const T &value)
    {
        int pos = 0;
        while (pos < ptr.count && ptr.data[pos] < value)
            pos++;
        if (ptr.children[pos] != 0)
        {
            long page_id = ptr.children[pos];
            node child   = read_node(page_id);
            int  state   = insert(child, value);
            if (state == BT_OVERFLOW)
                split(ptr, pos);
        }
        else
        {
            ptr.insert_in_node(pos, value);
            write_node(ptr.page_id, ptr);
        }
        return ptr.is_overflow() ? BT_OVERFLOW : NORMAL;
    }

    void split(node &parent, int pos)
    {
        node left      = this->read_node(parent.children[pos]);
        node right     = this->new_node();
        bool left_leaf = left.is_leaf();
        parent.insert_in_node(pos, left.data[BTREE_ORDER / 2]);

        for (int iter = BTREE_ORDER / 2 + !left_leaf, i = 0;
             iter < BTREE_ORDER + 1;
             iter++, i++)
        {
            right.children[i] = left.children[iter];
            right.data[i]     = left.data[iter];
            right.count++;
            left.data[iter]     = T();
            left.children[iter] = 0;
            left.count--;
        }

        right.children[right.count]    = left.children[BTREE_ORDER + 1];
        left.children[BTREE_ORDER + 1] = 0;

        parent.children[pos]     = left.page_id;
        parent.children[pos + 1] = right.page_id;

        if (!left_leaf)
        {
            left.data[BTREE_ORDER / 2] = T();
            left.count--;
        }
        else
        {
            right.next_id = left.next_id;
            left.next_id  = right.page_id;
        }

        write_node(parent.page_id, parent);
        write_node(left.page_id, left);
        write_node(right.page_id, right);
    }

    void split_root()
    {
        node ptr       = this->read_node(this->header.root_id);
        bool root_leaf = ptr.is_leaf();
        node left      = this->new_node();
        node right     = this->new_node();

        int pos  = 0;
        int iter = 0;
        int i;
        for (i = 0; iter < BTREE_ORDER / 2; i++)
        {
            left.children[i] = ptr.children[iter];
            left.data[i]     = ptr.data[iter];
            left.count++;
            iter++;
        }
        left.children[i] = ptr.children[iter];
        if (not root_leaf)
            iter++; // the middle element
        for (i = 0; iter < BTREE_ORDER + 1; i++)
        {
            right.children[i] = ptr.children[iter];
            right.data[i]     = ptr.data[iter];
            right.count++;
            iter++;
        }
        right.children[i] = ptr.children[iter];

        std::fill(std::begin(ptr.children), std::end(ptr.children), 0);
        ptr.children[pos]     = left.page_id;
        ptr.data[0]           = ptr.data[BTREE_ORDER / 2];
        ptr.children[pos + 1] = right.page_id;
        ptr.count             = 1;
        if (root_leaf)
            left.next_id = right.page_id;

        write_node(ptr.page_id, ptr);
        write_node(left.page_id, left);
        write_node(right.page_id, right);
    }

    iterator lower_bound(T const &value) const
    {
        node ptr = read_node(header.root_id);
        while (not ptr.is_leaf())
        {
            int pos = 0;
            while (pos < ptr.count and ptr.data[pos] < value)
                ++pos;
            ptr = read_node(ptr.children[pos]);
        }

        int pos = ptr.lower_bound(value);
        // In case next_id doesn't exist, it returns end()
        if (pos == ptr.count)
            return iterator(*this, ptr.next_id, 0);
        return iterator(*this, ptr.page_id, pos);
    }

    bool remove(T const &key)
    {
        auto it = find_valid(key);
        if (it == this->end())
            return false;
        it.invalidate();
        return true;
    }

    iterator upper_bound(T const &key) const
    {
        auto it = lower_bound(key);
        while (it != this->end() and *it == key)
            ++it;
        return it;
    }
    iterator find(T const &key) const
    {
        auto it = lower_bound(key);
        if (it == this->end() or !(*it == key))
            return this->end();
        return it;
    }

    iterator find_valid(T const &key) const
    {
        auto it = find(key);
        while (it != this->end() and !it.valid())
            ++it;
        if (it == this->end() or !(*it == key))
            return this->end();
        return it;
    }

    std::pair<iterator, iterator> find(T const &low_key,
                                       T const &high_key) const
    {
        iterator first = lower_bound(low_key), last = upper_bound(high_key);
        return {first, last};
    }

    std::optional<T> point_search(T const &key) const
    {
        auto it = find_valid(key);
        if (it != this->end() or !(*it == key))
            return std::nullopt;
        return *it;
    }

    std::vector<T> range_search(T const &low_key, T const &high_key) const
    {
        auto [itl, itr] = find(low_key, high_key);
        return entries(itl, itr);
    }

    std::vector<T> entries(iterator first, iterator last) const
    {
        std::vector<T> ans;
        for (auto it = first; it != last; ++it)
            if (it.valid())
                ans.push_back(*it);
        return ans;
    }

    void write_all(std::string filename)
    {
        write(filename, this->begin(), this->end());
    }

    void write(std::string filename, iterator first, iterator last) const
    {
        std::ofstream file;
        file.open(filename, std::ios::trunc);
        auto records = entries(first, last);
        for (auto r : records)
        {
            std::vector<std::string> content = r.serialize();
            if (content.size())
                file << content[0];
            for (int i = 1, m = content.size(); i < m; ++i)
                file << "," << content[i];
            file << "\n";
        }
        file.close();
    }

    iterator begin() const
    {
        node ptr = read_node(header.root_id);
        while (not ptr.is_leaf())
            ptr = read_node(ptr.children[0]);
        return iterator(*this, ptr.page_id, 0);
    }

    iterator end() const { return iterator(*this, -1, 0); }

    void print(std::ostream &out) const
    {
        node root = read_node(header.root_id);
        print(root, 0, out);
    }

    void print(node &ptr, int level, std::ostream &out) const
    {
        int i;
        for (i = 0; i < ptr.count; i++)
        {
            if (ptr.children[i])
            {
                node child = read_node(ptr.children[i]);
                print(child, level + 1, out);
            }
            else
                out << ptr.data[i];
        }
        if (ptr.children[i])
        {
            node child = read_node(ptr.children[i]);
            print(child, level + 1, out);
        }
    }

    void print_tree() const
    {
        node root = read_node(header.root_id);
        print_tree(root, 0);
        std::cout << "________________________\n";
    }

    void print_tree(node &ptr, int level) const
    {
        int i;
        for (i = ptr.count - 1; i >= 0; i--)
        {
            if (ptr.children[i + 1])
            {
                node child = read_node(ptr.children[i + 1]);
                print_tree(child, level + 1);
            }

            for (int k = 0; k < level; k++)
            {
                std::cout << "    ";
            }
            std::cout << ptr.data[i] << "\n";
        }
        if (ptr.children[i + 1])
        {
            node child = read_node(ptr.children[i + 1]);
            print_tree(child, level + 1);
        }
    }
};

} // namespace disk
} // namespace db
