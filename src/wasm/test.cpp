#define CATCH_CONFIG_RUNNER
#include <iostream>
#include <emscripten/bind.h>
#include "../../catch.hpp"
#include <filesystem>
#include <string>
namespace fs = std::filesystem;

using namespace emscripten;

class Tests {
public:
    int runCatch() {
        const char*test_args[] = {""};
        return Catch::Session().run(2, test_args);
    }
};

// Binding code
EMSCRIPTEN_BINDINGS(Tests) {
        class_<Tests>("Tests")
                .constructor()
                .function("runCatch", &Tests::runCatch);
}

