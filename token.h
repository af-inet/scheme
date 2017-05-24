#ifndef SCM_TOKEN_H
#define SCM_TOKEN_H

#include <stdlib.h>

enum scm_token_type_t {
	T_END = 0,
	T_OP,
	T_OPEN,
	T_NUM,
	T_CLOSE,
	T_SYM,
	T_INVALID,
};

struct scm_token {
	enum scm_token_type_t type;
	const char *ptr;
	size_t len;

	int value;

	struct scm_token *next;
	struct scm_token *children;
};

int scm_tokenize(struct scm_token *tokens, size_t token_max, const char *src);

#endif /* SCM_TOKEN_H */
