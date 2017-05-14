#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "token.h"

enum charset_t {
	s_operator,
	s_alphanumeric,
	s_whitespace,
	s_start,
	s_eof,
	s_unknown,
};

struct tokenizer {
	const char *src;
	const char *start;
	struct skm_token *tokens;
	enum charset_t last;
	enum charset_t next;
	size_t index;
};

const char *_operator =
	"+-*()!";
const char *_alphanumeric =
	"abcdefghijklmnopqrstuvwxyz"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"1234567890";
const char *_whitespace =
	" \t\n\r\n";

unsigned char
char_in(const char c, const char *s)
{
	while (*s)
		if (c == *s++)
			return 1;
	return 0;
}

enum charset_t
charset(char c)
{
	if (c == '\0') { return s_eof; }
	if (char_in(c, _operator))     { return s_operator; }
	if (char_in(c, _alphanumeric)) { return s_alphanumeric; }
	if (char_in(c, _whitespace))   { return s_whitespace; }
	return s_unknown;
}

int
skm_next(struct tokenizer *t)
{
	switch (t->last) {

	case s_start:
	case s_whitespace:
	case s_operator:
		switch (t->next) {
		case s_eof:
		case s_whitespace:
			return 0;
		case s_alphanumeric:
			t->tokens[t->index] = (struct skm_token){
				.len = 1,
				.ptr = t->src
			};
			return 0;
		case s_operator:
			t->tokens[t->index] = (struct skm_token){
				.len = 1,
				.ptr = t->src
			};
			t->index += 1;
			return 0;
		default: break;
		}
		break;

	case s_alphanumeric:
		switch (t->next) {
		case s_eof:
		case s_whitespace:
			t->index += 1;
			return 0;
		case s_alphanumeric:
			t->tokens[t->index].len += 1;
			return 0;
		case s_operator:
			t->index += 1;
			t->tokens[t->index] = (struct skm_token){
				.len = 1,
				.ptr = t->src
			};
			t->index += 1;
			return 0;
		default: break;
		}
	default: break;
	}
	return -1;
}

int
skm_tokenize(struct skm_token *tokens, size_t token_max, const char *src)
{
	struct tokenizer *t = &(struct tokenizer){
		.start = src,
		.src = src,
		.tokens = tokens,
		.last = s_start,
		.index = 0
	};
	
	do
	{
		t->next = charset(*t->src);

		if (skm_next(t) == -1)
		{
			printf("unexpected[%d] '%c'\n", (int)(t->src - t->start), *t->src);
			return -1;
		}

		t->last = t->next;

		if (t->index > token_max)
		{
			printf("ran out of tokens (%lu > %lu)\n", t->index, token_max);
			return -1;
		}

	} while (*t->src++);

	return t->index;
}

