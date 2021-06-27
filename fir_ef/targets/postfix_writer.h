#ifndef __FIR_TARGETS_POSTFIX_WRITER_H__
#define __FIR_TARGETS_POSTFIX_WRITER_H__

#include "targets/basic_ast_visitor.h"

#include <sstream>
#include <cdk/emitters/basic_postfix_emitter.h>
#include <stack>
#include <set>

namespace fir {

    //!
    //! Traverse syntax tree and generate the corresponding assembly code.
    //!
    class postfix_writer : public basic_ast_visitor {
        cdk::symbol_table<fir::symbol> &_symtab;
        cdk::basic_postfix_emitter &_pf;
        int _lbl;

        std::shared_ptr<fir::symbol> _function;
        std::set<std::string> _functions_to_declare;
        std::vector<std::string> _leave;
        std::vector<std::string> _restart;
        bool _inFunctionBody = false;
        bool _inFunctionArgs = false;
        int _offset = 0;

        std::string _currentBodyRetLabel; // where to jump when a return occurs of an exclusive section ends
        std::string _currentEpilogueRetLabel; // same but if epilogue exists then jump to it

    public:
        postfix_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<fir::symbol> &symtab,
                       cdk::basic_postfix_emitter &pf) :
                basic_ast_visitor(compiler), _symtab(symtab), _pf(pf), _lbl(0) {
        }

    public:
        ~postfix_writer() {
            os().flush();
        }

    private:
        /** Method used to generate sequential labels. */
        inline std::string mklbl(int lbl) {
            std::ostringstream oss;
            if (lbl < 0)
                oss << ".L" << -lbl;
            else
                oss << "_L" << lbl;
            return oss.str();
        }

    public:
        // do not edit these lines
#define __IN_VISITOR_HEADER__

#include "ast/visitor_decls.h"       // automatically generated

#undef __IN_VISITOR_HEADER__
        // do not edit these lines: end

    };

} // fir

#endif
