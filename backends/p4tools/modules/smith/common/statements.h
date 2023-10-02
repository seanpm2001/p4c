#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_STATEMENTS_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_STATEMENTS_H_

#include "backends/p4tools/modules/smith/common/generator.h"
#include "ir/indexed_vector.h"
#include "ir/ir.h"

namespace P4Tools::P4Smith {

class StatementGenerator : public Generator {
 public:
    explicit StatementGenerator(const SmithTarget &target) : Generator(target) {}

    virtual ~StatementGenerator() = default;

    virtual IR::Statement *genStatement(bool is_in_func);

    IR::IndexedVector<IR::StatOrDecl> genBlockStatementHelper(bool is_in_func);

    virtual IR::BlockStatement *genBlockStatement(bool is_in_func);

    virtual IR::IfStatement *genConditionalStatement(bool is_in_func);

    static void removeLval(IR::Expression *left, IR::Type *type);

    virtual IR::Statement *genAssignmentStatement();

    virtual IR::Statement *genMethodCallExpression(IR::PathExpression *methodName,
                                                   IR::ParameterList params);

    virtual IR::Statement *genMethodCallStatement(bool is_in_func);

    virtual IR::Statement *genAssignmentOrMethodCallStatement(bool is_in_func);

    virtual IR::ExitStatement *genExitStatement();

    virtual IR::SwitchStatement *genSwitchStatement();

    IR::ReturnStatement *genReturnStatement(const IR::Type *tp = nullptr);
};

}  // namespace P4Tools::P4Smith

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_STATEMENTS_H_ */
