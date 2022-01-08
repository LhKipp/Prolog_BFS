/* #define CATCH_CONFIG_RUNNER */
/* #include "catch.hpp" */
#include <iostream>

#include "prolog_bfs/program_arguments/parser.h"
#include "prolog_bfs/app_runner/normal_mode.h"
#include "prolog_bfs/wam/data/heap_reg.h"

int main(int argc, const char *argv[]) {
    using namespace std;

    /* const char*test_args[] = {""}; */
    /* int result = Catch::Session().run(argc, argv); */


//    Parsing CommandLine Arguments
    program_arguments::parser parser{};
    auto map = parser.getMap(argc, argv);

    if (map.count(program_arguments::help_option)) {
        cout << parser.generic << "\n";
        return 0;
    }

    return app_runner::normal_mode(map);
}
