#ifndef SKM_TOKEN_H
#define SKM_TOKEN_H

struct skm_token {
	const char *ptr;
	size_t len;
};

int skm_tokenize(struct skm_token *tokens, size_t token_max, const char *src);

#endif /* SKM_TOKEN_H */
