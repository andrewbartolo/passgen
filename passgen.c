#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>


#define VALID_CHARS "!#$%&()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
	"[]^_`abcdefghijklmnopqrstuvwxyz{|}~"
#define USE_DEV_RANDOM false



void
die(char* argv[], const char* const msg)
{
    printf("ERROR: %s\n", msg);
    printf("usage: %s <password length>\n", argv[0]);
    exit(1);
}

int
qsort_cmp(const void* a, const void* b)
{
    return *((char*) a) - *((char*) b);
}


int
main(int argc, char* argv[])
{
    if (argc != 2) die(argv, "Must supply password length");
    size_t password_len = atol(argv[1]);
    if (password_len <= 0) die(argv, "Must supply valid password length");

    // use strdup() b/c text-segment char literal is const (so couldn't sort)
    char* chars = strdup(VALID_CHARS);
    size_t n_chars = strlen(chars);
    // sort, just because we can
    qsort(chars, n_chars, sizeof(char), qsort_cmp);


    int gr_flags = USE_DEV_RANDOM ? GRND_RANDOM : 0;
    
    printf("your new password: ");

    size_t n_gen = 0;
    while (n_gen < password_len) {
	unsigned char rnd;
        assert(getrandom(&rnd, sizeof(rnd), gr_flags) == sizeof(rnd));

	// if index fell outside the char array, resample and try again.
	if (rnd >= n_chars) continue;

	char ch = chars[rnd];
	printf("%c", ch);
	++n_gen;
    }
    printf("\n");

    free(chars);
    return 0;
}
