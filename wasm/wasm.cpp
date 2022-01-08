#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"
#include <emscripten/bind.h>
#include <chrono>

using namespace emscripten;
using namespace wam;


class PrologBFSWasmWrapper{
    wam::bfs_organizer *bfs_organizer = new wam::bfs_organizer;
public:

    void setTimeLimit(size_t microseconds){
        bfs_organizer->set_time_limit(std::chrono::microseconds{microseconds});
    }

    void clear(){
        bfs_organizer->clear_memory();
        delete bfs_organizer;
    }

    /**
     * Checks whether the given code is valid prolog program code
 the code to validate
     */
    compiler::error validateProgramCode(std::string code){
        return bfs_organizer->validate_program(code);
    }

    /**
     * Checks whether the given code is valid prolog query code
 the code to validate
     */
    compiler::error validateQueryCode(std::string code){
        return bfs_organizer->validate_query(code);
    }

    compiler::error loadProgram(std::string prog) {
        return bfs_organizer->load_program(prog);
    }

    compiler::error loadQuery(std::string query) {
        return bfs_organizer->load_query(query);
    }

    wam::result getAnswer() {
        return bfs_organizer->get_answer();
    }

    wam::query_node getUnificationTree() const{
        return bfs_organizer->get_unification_tree();
    }
};

// Binding code
EMSCRIPTEN_BINDINGS(PrologBFSWasmWrapper) {
        class_<PrologBFSWasmWrapper>("PrologBFSWasmWrapper")
                .constructor()
                .function("setTimeLimit", &PrologBFSWasmWrapper::setTimeLimit)
                .function("clear", &PrologBFSWasmWrapper::clear)
                .function("validateQueryCode", &PrologBFSWasmWrapper::validateQueryCode)
                .function("validateProgramCode", &PrologBFSWasmWrapper::validateProgramCode)
                .function("loadProgram", &PrologBFSWasmWrapper::loadProgram)
                .function("loadQuery", &PrologBFSWasmWrapper::loadQuery)
                .function("getAnswer", &PrologBFSWasmWrapper::getAnswer)
                .function("getUnificationTree", &PrologBFSWasmWrapper::getUnificationTree);
}

