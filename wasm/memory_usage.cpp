#include <emscripten/bind.h>
#include <malloc.h>

using namespace emscripten;

int get_memory_usage() {
    return (int) mallinfo().uordblks;
}

EMSCRIPTEN_BINDINGS(memory_usage) {
    function("getMemoryUsage", &get_memory_usage);
}