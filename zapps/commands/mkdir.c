#include <syscall.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char *suffix = malloc(256);
    strcpy(suffix, argv[2]);
    char *current_dir = malloc(256);
    strcpy(current_dir, argv[1]);
    if (!strcmp(suffix, "..")) {
        printf("$3Un dossier ne peut pas avoir comme nom .. !\n");
    } else {
        c_fs_make_dir(current_dir, suffix);
    }
    free(current_dir);
    free(suffix);
    return 0;
}
