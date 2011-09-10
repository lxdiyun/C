#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>

char *get_regerror(int errcode, regex_t *compiled)
{
	size_t length = regerror(errcode, compiled, NULL, 0);
	char *buffer = malloc(length);
	if (!buffer)
		return NULL;
	(void) regerror(errcode, compiled, buffer, length);
	return buffer;
}

int regtest(const char *pattern, const char* string)
{
	regex_t reg;
	regmatch_t *subexprs= NULL;
	int ret;
	int i;

	if (0 != (ret=regcomp(&reg, pattern, 
			REG_EXTENDED)))
	{
		char *buffer = get_regerror(ret, &reg);
		if (buffer) 
		{
			fprintf(stderr, "regcomp:[%d]%s\n", ret, buffer);
			free(buffer);
		}
		return -1;
	}
	
	subexprs = malloc((reg.re_nsub+1)*sizeof(regmatch_t));
	if (!subexprs)
	{
		fprintf(stderr, "error malloc subexprs\n");
		regfree(&reg);
		return -1;
	}

	if (0 != (ret=regexec(&reg, string, reg.re_nsub+1, 
			subexprs, 0)))
	{
		char *buffer = get_regerror(ret, &reg);
		if (buffer)
		{
			fprintf(stderr, "regexec:[%d]%s\n", ret, buffer);
			free(buffer);
		}
		
		regfree(&reg);
		return -1;
	}
	for (i = 0; i <= reg.re_nsub; i++)
	{
		printf("[%d]:", i);

		if (subexprs[i].rm_so == subexprs[i].rm_eo)
		{
			printf("[EMPTY SUBEXPR]\n");
		}
		else if (subexprs[i].rm_so == -1 || 
				subexprs[i].rm_eo == -1)
		{
			printf("[NO SUBEXPR]\n");
		}
		else
		{
			fwrite(string+subexprs[i].rm_so, 1,
					subexprs[i].rm_eo-subexprs[i].rm_so, stdout);
			printf("\n");
		}
	}
	regfree(&reg);
	if (subexprs) free(subexprs);
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: regtest pattern string\n");
		return -1;
	}

	fprintf(stderr, "pattern:%s\n", argv[1]);
	fprintf(stderr, "string:%s\n", argv[2]);

	return regtest(argv[1], argv[2]);
}
