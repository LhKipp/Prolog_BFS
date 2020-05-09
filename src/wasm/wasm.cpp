#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "../wam/bfs_organizer/bfs_organizer.h"
#include <wam/bfs_organizer/data/result.h>

#include <emscripten/bind.h>
using namespace emscripten;
using namespace wam;

// Binding code
EMSCRIPTEN_BINDINGS(bfs_organizer) {
        class_<wam::bfs_organizer>("bfs_organizer")
                .constructor()
                .function("clear", &wam::bfs_organizer::clear)
                .function("validateQueryCode", &wam::bfs_organizer::validate_query)
                .function("validateProgramCode", &wam::bfs_organizer::validate_program)
                .function("loadProgram", &wam::bfs_organizer::load_program)
                .function("loadQuery", &wam::bfs_organizer::load_query)
                .function("getAnswer", &wam::bfs_organizer::get_answer)
                .function("getUnificationTree", &wam::bfs_organizer::get_unification_tree)
        ;
}
