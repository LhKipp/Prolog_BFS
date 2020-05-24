//
// Created by leonhard on 30.07.19.
//

#ifndef PROLOG_BFS_CONFIG_H
#define PROLOG_BFS_CONFIG_H

namespace wam::config{
    const char* const UNBOUND_VAR_PREFIX = "G_";
    const int UNBOUND_VAR_PREFIX_SIZE = 2;

    //69 is a very special number: Qouting wikipedia here:
    // "Of note is that 69^2 (4761) and 69^3 (328509) together use every decimal digit from 0-9.
    // 69 is equal to 105 octal, while 105 is equal to 69 hexadecimal."
    // Thats why we use 69 here.
    const unsigned BUILT_IN_PREDICATES_LINE_NUMBER = 1696969;
}
#endif //PROLOG_BFS_CONFIG_H
