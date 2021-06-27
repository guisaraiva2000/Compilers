#ifndef __FIR_AST_RETURN_H__
#define __FIR_AST_RETURN_H__

#include <cdk/ast/expression_node.h>

namespace fir {

    class return_node: public cdk::expression_node {
    public:
        return_node(int lineno) :
                cdk::expression_node(lineno) {
        }

    public:
        void accept(basic_ast_visitor *sp, int level) {
            sp->do_return_node(this, level);
        }

    };

} // fir

#endif