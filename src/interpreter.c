#include "interpreter.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Evaluates an expression tree and returns its numeric result.
 * 
 * @param expr The expression tree to evaluate.
 * @return double The computed numerical value of the expression.
 */
double evalExpr(Expr* expr) {
    switch (expr->type) {
        case EXPR_NUMBER:
            return strtod(expr->as.number.start, NULL);

       case EXPR_BINARY:
            {
                double left = evalExpr(expr->as.binary.left);
                double right = evalExpr(expr->as.binary.right);

                switch (expr->as.binary.operator.type)
                {
                    case TOKEN_PLUS: return left + right;
                    case TOKEN_MINUS: return left - right;
                    case TOKEN_STAR: return left * right;
                    case TOKEN_SLASH: return left / right;
                    default:
                        printf("unknown operator. \n");
                        return 0;
                }
            }
            
     default:
            printf("unknown expression type. \n");
            return 0;
    }
}
