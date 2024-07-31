#ifndef PROJECT_H
#define PROJECT_H

#define HELP

#include <stdio.h>
#include <stdbool.h>

#include "../include/cstring.h"

static const int VER_STR_LEN = 12;

typedef char **args;

static char *exe_file;
static int arg_count;
static args arg_list;
static bool is_debug;
static bool is_log;
static char *log_path = "./";
static char *proj_dir = NULL;

typedef struct version
{
    int major;
    int minor;
    int build;
} version;

//  prototypes
void init(char *file, int argc, char *argv[]);
void cleanup();
bool run();

#endif //  PROJECT_H