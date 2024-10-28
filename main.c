#include <stdio.h>
#include <stdlib.h>

#include "redir.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <inp> <cmd> <out>\n", argv[0]);
        return 1;
    }

    return redir(argv[1], argv[2], argv[3]);
}
