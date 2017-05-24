#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "token.h"

struct tokenizer {
	const char *src;
	const char *start;
	struct scm_token *tokens;
	size_t index;
};

enum {
	type_none = 0,
	type_whitespace,
	type_alpha,
	type_numeric,
};

const char type_table[256] = {
	[0 ... 255] = type_none,
	[' '] = type_whitespace,
	['\t'] = type_whitespace,
	['\r'] = type_whitespace,
	['\n'] = type_whitespace,
	['a' ... 'z'] = type_alpha,
	['A' ... 'Z'] = type_alpha,
	['0' ... '9'] = type_numeric
};

unsigned char is_alpha(unsigned char c)      { return type_table[c] == type_alpha; }
unsigned char is_whitespace(unsigned char c) { return type_table[c] == type_whitespace; }
unsigned char is_numeric(unsigned char c)    { return type_table[c] == type_numeric; }

int digit_table[256] = {
	[0 ... 255] = -1,
	['0'] = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

char
str_to_int(int *dest, const char *ptr, int len)
{
	int i, digit, acc = 0;

	for (i = 0; i < len; i++)
	{
		digit = digit_table[(unsigned)ptr[i]];
		if (digit == -1)
			return 1; /* failure */
		acc = (acc * 10) + digit;
	}

	*dest = acc;
	return 0; /* success */
}

void
next_token(struct tokenizer *t)
{
	int tmp;
	switch (*t->src) {
	case '-':
	case '+':
	case '*':
	case '!':
		t->tokens[t->index++] = (struct scm_token){.type=T_OP, .ptr=t->src, .len=1};
		t->src++;
		break;
	case '(':
		t->tokens[t->index++] = (struct scm_token){.type=T_OPEN, .ptr=t->src, .len=1};
		t->src++;
		break;
	case ')':
		t->tokens[t->index++] = (struct scm_token){.type=T_CLOSE, .ptr=t->src, .len=1};
		t->src++;
		break;
	default:

		if (is_alpha(*t->src)) {

			/* create a symbol token */
			t->tokens[t->index] = (struct scm_token) {
				.type = T_SYM,
				.ptr = t->src,
				.len = 1
			};

			t->src++;

			while (is_alpha(*t->src)) {
				t->tokens[t->index].len += 1;
				t->src++;
			}

			t->index++;

		} else if (is_numeric(*t->src)) {

			/* create a number token */
			t->tokens[t->index] = (struct scm_token) {
				.type = T_NUM,
				.ptr = t->src,
				.len = 1
			};

			t->src++;

			while (is_numeric(*t->src)) {
				t->tokens[t->index].len += 1;
				t->src++;
			}

			if (str_to_int(&tmp,
				t->tokens[t->index].ptr,
				t->tokens[t->index].len)
				== 0) {
				t->tokens[t->index].value = tmp;
			} else {
				t->tokens[t->index].type = T_INVALID;
				printf("failed to convert string to int [%d] '%c'\n", (int)(t->src - t->start), *t->src);
			}

			t->index++;

		} else if (is_whitespace(*t->src)) {

			/* do nothing */
			t->src++;

		} else {

			/* unexpected character */
			printf("unexpected [%d] '%c'\n", (int)(t->src - t->start), *t->src);
			t->src++;
		}
		break;
	}
}

int
scm_tokenize(struct scm_token *tokens, size_t token_max, const char *src)
{
	struct tokenizer *t = &(struct tokenizer){
		.start = src,
		.src = src,
		.tokens = tokens,
		.index = 0
	};

	do {
		next_token(t);
	} while (*t->src);

	return t->index;
}

