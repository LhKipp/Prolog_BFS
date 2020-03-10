//
// Created by leonhard on 10.03.20.
//

#ifndef PROLOG_BFS_EXEC_STATE_H
#define PROLOG_BFS_EXEC_STATE_H

namespace wam{

    enum class EXEC_STATE{
        //If the exec failed
        FAIL,
        //If the exec finished the unification process and a final var binding could be found
        SUCCESS,
        //If the exec is to be continued
        RUNNING,
        //If the exec was RUNNING and ended not failing, nor succedding in a call or proceed instrc.
        ARCHIVED
    };
};
#endif //PROLOG_BFS_EXEC_STATE_H
