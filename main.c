#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


#include "token.h"
#include "ast.h"
#include "eval.h"

/* returns null-terminated string representation for a token
 * MT-Unsafe
 */
char *
scm_token_str(struct scm_token *token)
{
	static char buf[255];
	bzero(buf, sizeof(buf));

	snprintf(
		buf, sizeof(buf),
		"%.*s",
		(int) token->len, token->ptr
	);

	return buf;
}

/* prints tokens as a flat list
 */
void
scm_tokens_print(struct scm_token *tokens, size_t count)
{
	int i;
	for (i = 0; i < count; i++)
		printf("%s ", scm_token_str(&tokens[i]));
	printf("\n");
}

void
scm_ast_print(struct scm_token *token, size_t level)
{
	do {
		for (size_t i = 0; i < level; i++) {
			printf("  ");
		}

		printf("%s\n", scm_token_str(token));

		if (token->children) {
			scm_ast_print(token->children, level+1);
		}
	}
	while (( token = token->next ));
}



void
print_usage(char *name)
{
	printf(
		"usage: %s <filename>\n",
		name
	);
}

char *
alloc_file(const char *path, off_t *size)
{
	struct stat info;
	char *buffer;
	int fd;

	if ((fd = open(path, O_RDONLY)) == -1)
	{
		return NULL;
	}

	if ((fstat(fd, &info) == -1))
	{
		close(fd); /* clean up file descriptor */
		return NULL;
	}

	if ((buffer = malloc(info.st_size + 1)) == NULL)
	{
		close(fd);
		return NULL;
	}

	if ((read(fd, buffer, info.st_size) == -1))
	{
		close(fd);
		free(buffer); /* clean up this buffer since we're not returning it */
		return NULL;
	}

	buffer[info.st_size] = '\0'; /* null terminator */
	*size = info.st_size;
	close(fd);
	return buffer;
}

void
run(const char *src)
{
	#define TOKEN_MAX 255
	struct scm_token tokens[TOKEN_MAX] = {0};
	struct scm_token *root;
	int len;

	len = scm_tokenize(tokens, TOKEN_MAX, src);
	if (len == -1)
	{
		printf("scm_tokenize: failed\n");
		return;
	}

	root = scm_ast(tokens, len);
	if (root == NULL)
	{
		printf("scm_ast: failed\n");
		return;
	}

	scm_ast_print(root, 0);

	printf("%d\n", scm_eval(root));
}

int
main(int argc, char **argv)
{
	const char *filename, *src;
	off_t len;

	if (argc != 2) {
		print_usage(argv[0]);
		return 1;
	}

	filename = argv[1];

	src = alloc_file(filename, &len);
	if (src == NULL) {
		printf("failed to open file: %s\n", filename);
		return 1;
	}

	run(src);

	free((void *)src);

	return 0;
}
