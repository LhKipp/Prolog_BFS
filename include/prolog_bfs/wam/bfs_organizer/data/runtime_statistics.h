//
// Created by leonhard on 24.05.20.
//

#ifndef PROLOG_BFS_RUNTIME_STATISTICS_H
#define PROLOG_BFS_RUNTIME_STATISTICS_H


namespace wam{
    class runtime_statistics {
    private:
        unsigned int queries_asked = 0;
    public:

        void inc_queries_asked_count(){
            queries_asked += 1;
        }

        unsigned int get_queries_asked_count()const{
            return queries_asked;
        }

    };
}


#endif //PROLOG_BFS_RUNTIME_STATISTICS_H
