#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

/* Appends a node to the end of a linked list (parent.children)
 * not super efficient since we traverse the whole list every time,
 * but it doesn't need to be efficient for now.
 */
void
scm_token_append(struct scm_token *parent, struct scm_token *child)
{
	if (parent->children) {
		parent = parent->children;
		while (parent->next) {
			parent = parent->next;
		}
		parent->next = child;
	} else {
		parent->children = child;
	}
}

struct scm_token *
scm_ast(struct scm_token *token_list, size_t count)
{
	size_t i;
	struct scm_token *token;
	struct scm_token *stack[255] = {0};
	size_t            stack_ptr = 0;

	/* TODO: check for out of memory condition */
	/* TODO: find a more elegant solution than `scm_token_append` */

	if (token_list[0].type != T_OPEN) {
		printf("expected first token to be '(', found '%.*s'\n",
			(int)token_list[0].len, token_list[0].ptr);
		return NULL;
	}

	#define STACK_PEAK() \
		stack[stack_ptr-1]

	#define STACK_POP() \
		stack[--stack_ptr]

	#define STACK_PUSH(token) \
		stack[stack_ptr++] = token

	for (i = 0; i < count; i++)
	{
		token = &token_list[i];

		switch (token->type) {
		case T_OPEN:
			/* next token should be the operator for this expression
			 * TODO: what about null expressions, aka "()"?
			 */
			token = &token_list[++i];

			if (! (token->type == T_OP
				|| token->type == T_SYM
				|| token->type == T_NUM)) {
				printf("expected token after parenthesis[%lu], expected (OP|SYM|NUM) found '%.*s'\n",
					i, (int)token->len, token->ptr);
				return NULL;
			}

			if (stack_ptr > 0) {
				scm_token_append(STACK_PEAK(), token);
			}

			STACK_PUSH(token);
			break;

		case T_CLOSE:
			STACK_POP();
			break;

		default:
			scm_token_append(STACK_PEAK(), token);
			break;
		}
	}

	if (stack_ptr != 0) {
		printf("unbalanced parenthesis\n");
		return NULL;
	}

	return &token_list[1];
	#undef STACK_PEAK
	#undef STACK_POP
	#undef STACK_PUSH
}
