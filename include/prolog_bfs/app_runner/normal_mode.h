//
// Created by leonhard on 05.07.19.
//

#ifndef PROLOG_BFS_NORMAL_MODE_H
#define PROLOG_BFS_NORMAL_MODE_H

#include <boost/program_options/variables_map.hpp>

namespace app_runner {
    /*
     * Runs the program in normal mode (user - terminal - prolog_bfs)
     *
     * Any prolog file, provided as commandline argument, will be parsed.
     * Then, the user is set into an internal console mode, in which he can ask querys
     * If the user types "halt." execution of the program stops.
     */
    int normal_mode(boost::program_options::variables_map &map);
}

#endif //PROLOG_BFS_NORMAL_MODE_H
