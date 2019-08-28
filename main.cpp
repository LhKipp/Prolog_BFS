#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <iostream>
#include <string>

#include "src/program_arguments/parser.h"

#include "src/app_runner/normal_mode.h"

int main(int argc, const char *argv[]) {
    using namespace std;

    const char*args[] = {""};
    int result = Catch::Session().run(1, args);


//    Parsing CommandLine Arguments
    program_arguments::parser parser{};
    auto map = parser.getMap(argc, argv);

    if (map.count(program_arguments::help_option)) {
        cout << parser.generic << "\n";
        return 0;
    }

    return app_runner::normal_mode(map);


}
