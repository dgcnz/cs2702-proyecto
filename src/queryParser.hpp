#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

const string __path = "./graphic-interface/src/public/files/";

size_t split(const string &txt, vector<string> &strs, char ch) {
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();
    while( pos != string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }
    strs.push_back( txt.substr( initialPos, min( pos, txt.size() ) - initialPos + 1 ) );
    return strs.size();
}

string convertToUpperCase(string data){
    std::for_each(data.begin(), data.end(), [](char & c) {
        c = ::toupper(c);
    });
    return data;
}

string readQuery() {
    ifstream file(__path+"query.txt");
    string query;
    getline(file, query);
    file.close();
    return query;
}

void SELECT_FUNCTION(vector<string> tokenize_query) {
    //SELECT * FROM users;
}

void INSERT_FUNCTION(vector<string> tokenize_query) {
    //INSERT INTO users VALUES (1,Andrea,Diaz,22,peruana);
}

void DELETE_FUNCTION(vector<string> tokenize_query) {
    //DELETE FROM users WHERE id = 12;
}

void INDEX_TYPE_SELECTION(vector<string> tokenize_query) {
    //USING INDEX SEQUENTIAL or USING INDEX BTREE 
}

void parserUtil() {
    string query = readQuery();
    vector<string> tokenize_query;
    split(query, tokenize_query, ' ');
    string command = convertToUpperCase(query.substr(0,6));
    if(command == "SELECT") SELECT_FUNCTION(tokenize_query);
    else if(command == "INSERT") INSERT_FUNCTION(tokenize_query);
    else if(command == "DELETE") DELETE_FUNCTION(tokenize_query);
    else if(command == "USING ") INDEX_TYPE_SELECTION(tokenize_query);
    else {
        cout << "Comando invalido.\n";
    }
}




