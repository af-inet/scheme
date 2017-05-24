#ifndef SCM_AST_H
#define SCM_AST_H

#include "token.h"

struct scm_token *
scm_ast(struct scm_token *token_list, size_t count);

#endif /* SCM_AST_H */
