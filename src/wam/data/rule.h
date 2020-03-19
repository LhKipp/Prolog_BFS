//
// Created by leonhard on 14.03.20.
//

#ifndef PROLOG_BFS_RULE_H
#define PROLOG_BFS_RULE_H

#include <vector>

namespace wam{
    struct rule{
    private:
        std::vector<compiled_atom> _atoms;
        source_code_info _code_info;

    public:

        inline bool is_query(){
            return _code_info.line_begin == std::numeric_limits<unsigned>::max();
        }

        inline bool has_body()const{
            return _atoms.size() >= 2;
        }

        inline bool is_fact() const{
            return _atoms.size() == 1;
        }

        std::vector<compiled_atom>& atoms(){
            return _atoms;
        };

        const std::vector<compiled_atom>& atoms()const{
            return _atoms;
        };

        template<class... Args>
        void add_atom(Args&&... args){
            _atoms.emplace_back(args...);
            _atoms.back().set_parent_rule(this);
        }

        const source_code_info &code_info() const {
            return _code_info;
        }

        void set_code_info(const source_code_info& code_info){
            _code_info = code_info;
        }
    };

}
#endif //PROLOG_BFS_RULE_H
