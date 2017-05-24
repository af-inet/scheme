#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "eval.h"

int
eval(struct scm_token *root);

int
eval_op(char c, int a, int b)
{
	switch (c) {
	case '+': return a + b;
	case '-': return a - b;
	case '*': return a * b;
	default:
		printf("cannot eval op: '%c'\n", c);
		return 0;
	}
}

int
eval_if(struct scm_token *token)
{
	/* TODO: sanity checks */
	struct scm_token *_expr;
	struct scm_token *_then;
	struct scm_token *_else;
	_expr = token->children;
	_then = token->children->next;
	_else = token->children->next->next;
	if (tmp = eval(_expr)) {
		return eval(_then);
	} else {
		return eval(_else);
	}
}

int
eval_sym(struct scm_token *token)
{
	if (strncmp(token->ptr, "if", 2) == 0) {
		return eval_if(token);
	} else {
		printf("unexpected symbol: '%.*s'\n", (int)token->len, token->ptr);
		return 0;
	}
}

int
eval(struct scm_token *root)
{
	struct scm_token *next;
	int acc = 0;

	if (root->type == T_NUM) {
		return root->value;
	}

	else if (root->type == T_OP) {

		if ((next = root->children) == NULL) {
			printf("operater with no children: '%.*s'\n", (int)root->len, root->ptr);
			return 0;
		}

		/* initialize the accumulator with the first value */
		acc = eval(next);

		/* apply the operator to each subsequant value */
		while (( next = next->next )) {
			acc = eval_op(*root->ptr, acc, eval(next));
		}

		return acc;
	}

	else if (root->type == T_SYM) {
		/* TODO: implemented symbols */
		/* evaluate to zero for now */
		return eval_sym(root);
	}

	else {
		printf("cannot eval token: '%.*s'\n", (int)root->len, root->ptr);
		return 0;
	}
}

int
scm_eval(struct scm_token *root)
{
	return eval(root);
}
