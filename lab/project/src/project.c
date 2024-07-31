#include <stdio.h>
#include <string.h>

#include "../include/project.h"
#include "../include/opio.h"

const version ver = {
    .major = 0,
    .minor = 1,
    .build = 0};

path *cwd;
string *title;

const char COL = ':';
const char *DEBUG = "--debug";
const char *LOG = "-l";
const char *DIR = "-d";

/*
    TODO: move to Utilities
    Essentially provides a ToString function for *Version ptr
*/
static string *version_info(const version *pVer)
{
    char pre[] = "v.";
    char dot[] = ".";

    return String.format("%s%02d%s%02d%s%03d", pre, pVer->major, dot, pVer->minor, dot, pVer->build);
}
static void process_options()
{
    char **arg = arg_list;

    while (*++arg)
    {
        printf("%s:\n", *arg);

        if (strcmp(*arg, DEBUG) == 0)
        {
            is_debug = true;
            printf("  is_debug: %s\n", is_debug ? "true" : "false");

            continue;
        }
        if (strstr(*arg, DIR) != NULL)
        {
            char *ndx = strchr(*arg, COL);

            if (ndx != NULL && ++ndx != NULL)
            {
                proj_dir = ndx;
            }

            printf(" dir: %s\n", proj_dir);

            continue;
        }
        if (strstr(*arg, LOG) != NULL)
        {
            is_log = true;
            printf("  is_log: %s\n", is_log ? "true" : "false");
            // look for ':'
            char *ndx = strchr(*arg, COL);

            if (ndx != NULL && ++ndx != NULL)
            {
                log_path = ndx;
            }

            printf("  log_path: %s\n", log_path);

            continue;
        }
    }
}
static void debug_info()
{
    printf("ENTRY: %s\n", exe_file);
    printf("CWD: %s\n", cwd->path->buffer);
}

void init(char *file, int argc, char *argv[])
{
    exe_file = file;
    arg_count = argc;
    arg_list = argv;
    is_debug = false;
    is_log = false;

    printf("(%d)", arg_count);

    cwd = malloc(sizeof(path));
    cwd->path = CURRENT_DIR;
    title = String.alloc("OpenPlatform Project Tool");

    char vi[VER_STR_LEN];
    string *pVerStr = version_info(&ver);
    printf("%s [%s]\n", title->buffer, pVerStr->buffer);
    String.free(pVerStr);

    process_options();

    if (!proj_dir)
    {
        printf("error: %s\n", "missing project directory '-d:<project_dir>'");
    }
}
void cleanup()
{
    String.free(cwd->path);
    free(cwd);
    String.free(title);
}
bool run()
{
    if (is_debug)
    {
        debug_info();
        return true;
    }

    return true;
}