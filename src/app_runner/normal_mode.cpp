//
// Created by leonhard on 05.07.19.

//

#include <iostream>
#include "normal_mode.h"
#include "../program_arguments/parser.h"

namespace app_runner {

    int normal_mode(boost::program_options::variables_map& map) {
        using namespace std;
        if (!map.count(program_arguments::prolog_file_option)) {
            cout << "No prolog files have been specified." << endl
                 << "Use prolog_bfs --help for advice." << endl
                 << "Stopping execution!";
        }

        const vector<string> file_paths = map[program_arguments::prolog_file_option].as<vector<string>>();

        //Parse every provided prolog file
        for (const auto &path : file_paths) {

        }
    }
}
