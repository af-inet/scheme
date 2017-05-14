#include <stdio.h>
#include <stdlib.h>

#include "token.h"

int main (int argc, char **argv) {

	struct skm_token tokens[255] = {0};
	const char *src = "The (quick) brown fox";
	int len;

	if ((len=skm_tokenize(tokens, sizeof(tokens), src)) == -1)
	{
		printf("skm_tokenize: failed\n");
		return 1;
	}

	for (int i=0; i<len; i++) {
		printf("%.*s ",
			(int)tokens[i].len,
			tokens[i].ptr);
	}
	printf("\n");

	return 0;
}
