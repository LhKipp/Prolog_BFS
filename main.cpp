#include <iostream>
#include "src/program_arguments/parser.h"

void socket_mode(boost::program_options::variables_map map);

int main(int argc, const char *argv[]) {
    using namespace std;

    //Parsing CommandLine Arguments
    vector<string> prolog_files;
    program_arguments::parser parser{};
    auto map = parser.getMap(argc, argv);

    if (map.count(program_arguments::help_option)) {
        cout << parser.generic << "\n";
        return 1;
    }

    //The program either runs in socket mode (webbrowser - server - bfs_prolog
    //or in normal mode (user - terminal - bfs_prolog)
    if(map.count(program_arguments::socket_option)){
        return socket_mode(map);
    } else{
        return normal_mode(map);
    }


}