#ifndef _TESTING_H
#define _TESTING_H

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>

#include "core.h"
#include "except/Except.h"
#include "except/Assert.h"

int test_count = 0;
int pass_count = 0;
int set_count = 0;

void writeln(string);
void writefln(const string, ...);
void vwritefln(const string, va_list);

bool doTests = true;

#define BEGIN_SET(__test_set, __do_tests)                                    \
    {                                                                        \
        doTests = __do_tests;                                                \
        char *format = "%s SET";                                             \
        size_t flen = snprintf(NULL, 0, format, doTests ? "BEGIN" : "SKIP"); \
        char *pChar = malloc(flen);                                          \
        sprintf(pChar, format, doTests ? "BEGIN" : "SKIP");                  \
        printf("%-10s", pChar);                                              \
        free(pChar);                                                         \
        writefln("[%s]", #__test_set);                                       \
        if (doTests)                                                         \
        {                                                                    \
            set_count = 0;                                                   \
            puts("---------------------------------------------------");     \
        }

#define TEST(__test_func)                                                                                \
    ({                                                                                                   \
        bool pass = true;                                                                                \
        if (doTests)                                                                                     \
        {                                                                                                \
            writefln("*** TEST CASE [%s]", #__test_func);                                                \
            try                                                                                          \
            {                                                                                            \
                __test_func();                                                                           \
                ++pass_count;                                                                            \
            }                                                                                            \
            catch (Throwable, e)                                                                         \
            {                                                                                            \
                pass = false;                                                                            \
            }                                                                                            \
            finally                                                                                      \
            {                                                                                            \
                writefln("==========================================   [%-4s]", pass ? "PASS" : "FAIL"); \
                ++test_count;                                                                            \
                ++set_count;                                                                             \
            }                                                                                            \
        }                                                                                                \
    })

#define END_SET(__test_set)                                      \
    if (doTests)                                                 \
    {                                                            \
        char *format = "END SET   [%s]";                         \
        size_t flen = snprintf(NULL, 0, format, #__test_set);    \
        char *pChar = malloc(flen);                              \
        sprintf(pChar, format, #__test_set);                     \
        printf("%-25s", pChar);                                  \
        writefln("%13s %3d", "Tests:", set_count);               \
        free(pChar);                                             \
    }                                                            \
    puts("==================================================="); \
    doTests = true;                                              \
    }

#define TEST_STATS() ({                        \
    writefln("Total Tests: %29d", test_count); \
    writefln("Tests PASS:  %29d", pass_count); \
})
void write_header(char *header)
{
    writefln("%s", header);
    puts("===================================================");
}

void writeln(char *line)
{
    printf("%s", line);
    puts("");
}
void writefln(const string format, ...)
{
    va_list args;
    va_start(args, format);

    //  need to parse format to know what type is needed here
    // char *arg = va_arg(args, char *);
    //  ========================
    //  this works if NULL is appended to a list of args -
    //      ex: arg1, arg2, ..., argX, NULL
    // while (arg)
    // {
    //     writeln(arg);
    //     arg = va_arg(args, char *);
    // }
    vwritefln(format, args);
    va_end(args);
}
void vwritefln(const string format, va_list args)
{
    vprintf(format, args);
    puts("");
}

//  utility definitions ...
bool __test_path_exists(string pPath)
{
    return access(pPath, F_OK) == 0 ? true : false;
}

const struct Test_Utils
{
    bool (*path_exists)(string);
} Utils;

const struct Test_Utils Utils = {
    .path_exists = &__test_path_exists};

#endif //  _TESTING_H