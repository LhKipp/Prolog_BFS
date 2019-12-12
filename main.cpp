#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <iostream>
#include <string>

#include "src/program_arguments/parser.h"

#include "src/app_runner/normal_mode.h"
#include "src/wam/data/regist.h"

int main(int argc, const char *argv[]) {
    using namespace std;

    std::cout << sizeof(wam::regist);
    const char*args[] = {"BFS_Organizer_Tests"};
    int result = Catch::Session().run(2, args);


//    Parsing CommandLine Arguments
    program_arguments::parser parser{};
    auto map = parser.getMap(argc, argv);

    if (map.count(program_arguments::help_option)) {
        cout << parser.generic << "\n";
        return 0;
    }

    //return app_runner::normal_mode(map);
    return 0;


}
