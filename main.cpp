#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <iostream>
#include "src/program_arguments/parser.h"

#include "src/app_runner/normal_mode.h"
#include "src/app_runner/socket_mode.h"

int main(int argc, const char *argv[]) {
    using namespace std;

    int result = Catch::Session().run(argc, argv);


    //Parsing CommandLine Arguments
    program_arguments::parser parser{};
    auto map = parser.getMap(argc, argv);

    if (map.count(program_arguments::help_option)) {
        cout << parser.generic << "\n";
        return 1;
    }

    //The program either runs in socket mode (webbrowser - server - bfs_prolog
    //or in normal mode (user - terminal - bfs_prolog)
    if(map.count(program_arguments::socket_option)){
        return app_runner::socket_mode(map);
    } else{
        return app_runner::normal_mode(map);
    }


}