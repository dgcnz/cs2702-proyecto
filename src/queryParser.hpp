#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// Structures
#include "btree.h"
#include "pagemanager.h"

using namespace std;

int const BF = 4;
struct Register
{
    int  show_id;
    char type[20];
    char title[256];
    char director[256];
    int  release_year;
    char rating[10];
    char duration[10];

    bool operator<(Register const &r) { return show_id < r.show_id; }
    bool operator>(Register const &r) { return show_id > r.show_id; }
    bool operator==(Register const &r) { return show_id == r.show_id; }
    bool operator<=(Register const &r) { return show_id <= r.show_id; }
    vector<string> serialize() const
    {
        vector<string> ans;
        ans.push_back(to_string(show_id));
        ans.push_back(string(type));
        ans.push_back(string(title));
        ans.push_back(string(director));
        ans.push_back(to_string(release_year));
        ans.push_back(string(rating));
        ans.push_back(string(duration));
        return ans;
    }

    friend ostream &operator<<(ostream &os, const Register &r)
    {
        os << r.show_id << ", " << r.type << ", " << r.title << ", "
           << r.director << ", " << r.release_year << ", " << r.rating << ", "
           << r.duration;
        return os;
    }
};

bool parse_line(string line, Register &r)
{
    stringstream ss(line);
    char         buf[256];
    ss.getline(buf, 256, ',');
    r.show_id = stoi(buf);
    ss.getline(r.type, 20, ',');
    ss.getline(r.title, 256, ',');
    ss.getline(r.director, 256, ',');
    ss.getline(buf, 256, ',');
    r.release_year = stoi(buf);
    ss.getline(r.rating, 10, ',');
    ss.getline(r.duration, 10);
    return true;
};

void load(std::string filename, db::disk::btree<Register, BF> &bt)
{
    std::ifstream file;
    file.open(filename);
    if (!file.good())
        throw std::runtime_error("invalid file");
    std::string line;
    std::getline(file, line); // ignore header
    Register r;
    while (getline(file, line))
    {
        if (parse_line(line, r))
        {
            // cout << r << endl;
            bt.insert(r);
        }
    }
}

class QueryParser
{
  private:
    // estructura 1
    // estructura 2
    db::disk::btree<Register, BF> bt;
    int                           indexType = 0; // 0: B+Tree ; 1: Sequential
    string                        folderPath;

  public:
    QueryParser(string folderPath)
        : bt(std::make_shared<db::disk::pagemanager>("btree.index", true)),
          folderPath(folderPath)
    {
        load("src/dataset.csv", bt);
        // construir estructura 2
    }

    size_t split(const string &txt, vector<string> &strs, char ch)
    {
        size_t pos        = txt.find(ch);
        size_t initialPos = 0;
        strs.clear();
        while (pos != string::npos)
        {
            strs.push_back(txt.substr(initialPos, pos - initialPos));
            initialPos = pos + 1;

            pos = txt.find(ch, initialPos);
        }
        strs.push_back(
            txt.substr(initialPos, min(pos, txt.size()) - initialPos + 1));
        return strs.size();
    }

    string convertToUpperCase(string data)
    {
        std::for_each(
            data.begin(), data.end(), [](char &c) { c = ::toupper(c); });
        return data;
    }

    string readQuery(string fileName)
    {
        ifstream file(folderPath + fileName);
        string   query;
        getline(file, query);
        file.close();
        return query;
    }

    void SELECT_FUNCTION(vector<string> tokenize_query)
    {
        // SELECT * FROM users;
        switch (this->indexType)
        {
        case 0:
            bt.write(folderPath + "../input.csv");
            // case 1 : call sequential
        }
    }

    void INSERT_FUNCTION(vector<string> tokenize_query)
    {
        // INSERT INTO users VALUES (1,Andrea,Diaz,22,peruana);
        cout << "Funciona el INSERT!\n";
    }

    void DELETE_FUNCTION(vector<string> tokenize_query)
    {
        // DELETE FROM users WHERE id = 12;
    }

    void INDEX_TYPE_SELECTION(vector<string> tokenize_query)
    {
        // USING INDEX BT o SF
        string indexType = convertToUpperCase(tokenize_query[2]);
        if (indexType == "BT")
            this->indexType = 0;
        else if (indexType == "SF")
            this->indexType = 1;
        cout << "Error al asignar el index " << indexType << "\n";
    }

    void parserUtil(string fileName)
    {
        string         query = readQuery(fileName);
        vector<string> tokenize_query;
        split(query, tokenize_query, ' ');
        string command = convertToUpperCase(query.substr(0, 6));
        if (command == "SELECT")
            SELECT_FUNCTION(tokenize_query);
        else if (command == "INSERT")
            INSERT_FUNCTION(tokenize_query);
        else if (command == "DELETE")
            DELETE_FUNCTION(tokenize_query);
        else if (command == "USING ")
            INDEX_TYPE_SELECTION(tokenize_query);
        else
        {
            cout << "Comando invalido.\n";
        }
    }
};
