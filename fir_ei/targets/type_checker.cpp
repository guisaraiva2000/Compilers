#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated
#include <cdk/types/primitive_type.h>

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

//---------------------------------------------------------------------------

void fir::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
    // EMPTY
}

//---------------------------------------------------------------------------

void fir::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
    // EMPTY
}

void fir::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
    // EMPTY
}

void fir::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
    // EMPTY
}

void fir::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
    // EMPTY
}

void fir::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
    // EMPTY
}

void fir::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
    // EMPTY
}

//---------------------------------------------------------------------------

void fir::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
    ASSERT_UNSPEC;
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
    ASSERT_UNSPEC;
    node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
}

//---------------------------------------------------------------------------

void fir::type_checker::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
    node->argument()->accept(this, lvl + 2);
    if (!node->argument()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in argument of unary expression");

    // in FIR, expressions are always int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_neg_node(cdk::neg_node *const node, int lvl) {
    processUnaryExpression(node, lvl);
}

void fir::type_checker::do_identity_node(fir::identity_node *const node, int lvl) {
    processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void fir::type_checker::processBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
    ASSERT_UNSPEC;
    node->left()->accept(this, lvl + 2);
    if (!node->left()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of binary expression");

    node->right()->accept(this, lvl + 2);
    if (!node->right()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of binary expression");

    // in FIR, expressions are always int
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
    processBinaryExpression(node, lvl);
}

void fir::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
    processBinaryExpression(node, lvl);
}

void fir::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
    processBinaryExpression(node, lvl);
}

void fir::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
    processBinaryExpression(node, lvl);
}

void fir::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
    processBinaryExpression(node, lvl);
}

void fir::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
    processBinaryExpression(node, lvl);
}

void fir::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
    processBinaryExpression(node, lvl);
}

void fir::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
    processBinaryExpression(node, lvl);
}

void fir::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
    processBinaryExpression(node, lvl);
}

void fir::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
    processBinaryExpression(node, lvl);
}

void fir::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
    processBinaryExpression(node, lvl);
}


//---------------------------------------------------------------------------

void fir::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
    ASSERT_UNSPEC;
    const std::string &id = node->name();
    std::shared_ptr<fir::symbol> symbol = _symtab.find(id);

    if (symbol != nullptr) {
        node->type(symbol->type());
    } else {
        throw id;
    }
}

void fir::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
    ASSERT_UNSPEC;
    try {
        node->lvalue()->accept(this, lvl);
        node->type(node->lvalue()->type());
    } catch (const std::string &id) {
        throw "undeclared variable '" + id + "'";
    }
}

void fir::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
    ASSERT_UNSPEC;

    try {
        node->lvalue()->accept(this, lvl);
    } catch (const std::string &id) {
        auto symbol = std::make_shared<fir::symbol>(cdk::primitive_type::create(4, cdk::TYPE_INT), id, 0);
        _symtab.insert(id, symbol);
        _parent->set_new_symbol(symbol);
        node->lvalue()->accept(this, lvl);
    }

    if (!node->lvalue()->is_typed(cdk::TYPE_INT))
        throw std::string("wrong type in left argument of assignment expression");

    node->rvalue()->accept(this, lvl + 2);
    if (!node->rvalue()->is_typed(cdk::TYPE_INT))
        throw std::string("wrong type in right argument of assignment expression");

    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------

void fir::type_checker::do_evaluation_node(fir::evaluation_node *const node, int lvl) {
    node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_while_node(fir::while_node *const node, int lvl) {
    node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void fir::type_checker::do_if_node(fir::if_node *const node, int lvl) {
    node->condition()->accept(this, lvl + 4);
}

void fir::type_checker::do_if_else_node(fir::if_else_node *const node, int lvl) {
    node->condition()->accept(this, lvl + 4);
}

//////////////////////////////////////////////////////////////////////////////

void fir::type_checker::do_variable_declaration_node(fir::variable_declaration_node *const node, int lvl) {
    // ?
}

void fir::type_checker::do_null_node(fir::null_node *const node, int lvl) {
    ASSERT_UNSPEC;
    node->type(cdk::reference_type::create(4, nullptr));
}

void fir::type_checker::do_input_node(fir::input_node *const node, int lvl) {
    node->type(cdk::primitive_type::create(0, cdk::TYPE_UNSPEC));
}

void fir::type_checker::do_write_node(fir::write_node *const node, int lvl) {
    node->arguments()->accept(this, lvl + 2);
}

void fir::type_checker::do_address_of_node(fir::address_of_node *const node, int lvl) {
    ASSERT_UNSPEC;
    node->lvalue()->accept(this, lvl + 2);
    if (!node->lvalue()->is_typed(cdk::TYPE_UNSPEC) &&
        !node->lvalue()->is_typed(cdk::TYPE_VOID))
        node->type(cdk::reference_type::create(4, node->lvalue()->type()));
    else
        throw std::string("Wrong type in unary logical expression.");
}

void fir::type_checker::do_sizeof_node(fir::sizeof_node *const node, int lvl) {
    ASSERT_UNSPEC;
    node->expression()->accept(this, lvl + 2);
    node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void fir::type_checker::do_stack_alloc_node(fir::stack_alloc_node *const node, int lvl) {
    ASSERT_UNSPEC;
    node->argument()->accept(this, lvl + 2);
    if (node->argument()->is_typed(cdk::TYPE_UNSPEC)) {
        fir::input_node *input = dynamic_cast<fir::input_node *>(node->argument());

        if (input != nullptr)
            node->argument()->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
        else
            throw std::string("Unknown node with unspecified type.");
    } else if (!node->argument()->is_typed(cdk::TYPE_INT))
        throw std::string("Integer expression expected in allocation expression.");

    node->type(cdk::primitive_type::create(0, cdk::TYPE_UNSPEC));
}

void fir::type_checker::do_function_call_node(fir::function_call_node *const node, int lvl) {
    // EMPTY
}

void fir::type_checker::do_function_declaration_node(fir::function_declaration_node *const node, int lvl) {
    // EMPTY
}

void fir::type_checker::do_function_definition_node(fir::function_definition_node *const node, int lvl) {
    // EMPTY
}

void fir::type_checker::do_block_node(fir::block_node *const node, int lvl) {
    if (node->declarations())
        node->declarations()->accept(this, lvl + 2);

    if (node->instructions())
        node->instructions()->accept(this, lvl + 2);
}

void fir::type_checker::do_body_node(fir::body_node *const node, int lvl) {
    if (node->epilogue())
        node->epilogue()->accept(this, lvl + 2);

    if (node->block())
        node->block()->accept(this, lvl + 2);

    if (node->prologue())
        node->prologue()->accept(this, lvl + 2);
}

void fir::type_checker::do_index_node(fir::index_node *const node, int lvl) {
    // ?
}

void fir::type_checker::do_leave_node(fir::leave_node *const node, int lvl) {
    // EMPTY
}

void fir::type_checker::do_restart_node(fir::restart_node *const node, int lvl) {
    // EMPTY
}

void fir::type_checker::do_return_node(fir::return_node *const node, int lvl) {
    // EMPTY
}

void fir::type_checker::do_prologue_node(fir::prologue_node *const node, int lvl) {
    if (node->declarations())
        node->declarations()->accept(this, lvl + 2);

    if (node->instructions())
        node->instructions()->accept(this, lvl + 2);
}