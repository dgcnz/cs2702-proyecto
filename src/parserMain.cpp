#include "FileWatcher.h"
#include "queryParser.hpp"

using namespace std;

const string FILE_NAME   = "query.txt";
const string FOLDER_PATH = "graphic-interface/src/public/files/monitor/";

int main()
{
    QueryParser *qp = new QueryParser(FOLDER_PATH);
    FileWatcher  fw{FOLDER_PATH, chrono::milliseconds(100)};
    fw.start([qp](string path_to_watch, FileStatus status) -> void {
        if (!filesystem::is_regular_file(
                std::filesystem::path(path_to_watch)) &&
            status != FileStatus::erased)
            return;
        switch (status)
        {
        case FileStatus::created:
            cout << "File created: " << path_to_watch << '\n';
            break;
        case FileStatus::modified:
            cout << "File modified: " << path_to_watch << '\n';
            qp->parserUtil(FILE_NAME);
            break;
        case FileStatus::erased:
            cout << "File erased: " << path_to_watch << '\n';
            break;
        default:
            cout << "Error! Unknown file status.\n";
        }
    });
    return 0;
}
