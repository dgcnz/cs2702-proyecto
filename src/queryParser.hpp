#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//Structures
#include "btree.h"
#include "pagemanager.h"

using namespace std;

class QueryParser {
private:
    //estructura 1
    //estructura 2
    int indexType = 0; //0: B+Tree ; 1: Sequential

public:

    QueryParser() {
        //construir estructura 1
        //construir estructura 2

    }

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

    string convertToUpperCase(string data) {
        std::for_each(data.begin(), data.end(), [](char & c) {
            c = ::toupper(c);
        });
        return data;
    }

    string readQuery(string folderPath, string fileName) {
        ifstream file(folderPath+fileName);
        string query;
        getline(file, query);
        file.close();
        return query;
    }

    void SELECT_FUNCTION(vector<string> tokenize_query) {
        //SELECT * FROM users;
        //switch(this->indexType) {
            //case 0:
                //call b+tree
            //case 1:
                //call sequential
        //}
    }

    void INSERT_FUNCTION(vector<string> tokenize_query) {
        //INSERT INTO users VALUES (1,Andrea,Diaz,22,peruana);
        cout << "Funciona el INSERT!\n";
    }

    void DELETE_FUNCTION(vector<string> tokenize_query) {
        //DELETE FROM users WHERE id = 12;
    }

    void INDEX_TYPE_SELECTION(vector<string> tokenize_query) {
        //USING INDEX BT o SF
        string indexType = convertToUpperCase(tokenize_query[2]);
        if(indexType == "BT") this->indexType = 0;
        else if(indexType == "SF") this->indexType = 1;
        cout << "Error al asignar el index " << indexType << "\n";
    }

    void parserUtil(string folderPath, string fileName) {
        string query = readQuery(folderPath, fileName);
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
};




