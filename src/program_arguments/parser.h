//
// Created by leonhard on 05.07.19.
//

#ifndef PROLOG_BFS_PARSER_H
#define PROLOG_BFS_PARSER_H

#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace program_arguments {
    const char * const prolog_file_option = "prolog-file";
    const char* const help_option = "help";
    const char*  const socket_option = "socket-connection";

    class parser {
    public:
        parser() { init(); };

        po::options_description generic{"Supported options"};
        po::options_description hidden{"Hidden options"};


        po::variables_map getMap(int argc, const char *argv[]) {
            po::positional_options_description pos_options;
            pos_options.add(prolog_file_option, -1);

            po::options_description cmdline_options;
            cmdline_options.add(generic).add(hidden);
            po::variables_map vm;
            po::store(po::command_line_parser(argc, argv).
                    options(cmdline_options).positional(pos_options).run(), vm);
            po::notify(vm);

            return vm;
        }

    private:
        void init() {
            generic.add_options()
                    (help_option, "show help message")
                    (socket_option, po::value<std::string>(),
                     "establishes a socket connection to the supplied file and communicates to user over it");
            hidden.add_options()
                    (prolog_file_option, po::value<std::vector<std::string> >(), "prolog file");
        }


    };
}


#endif //PROLOG_BFS_PARSER_H
