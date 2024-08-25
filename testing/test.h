#ifndef _TESTING_H
#define _TESTING_H

#define DEBUG

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include "open/core.h"
#include "Except.h"
#include "Assert.h"

void writeln(char *);
void writefln(const char *, ...);
void vwritefln(const char *, va_list);

except_class_declare(AssertException, Exception);

except_class_define(AssertException, Exception);

#define TEST(__test_func)                                                                            \
    ({                                                                                               \
        bool pass = true;                                                                            \
        writefln("TEST CASE [%s]", #__test_func);                                                    \
        try                                                                                          \
        {                                                                                            \
            __test_func();                                                                           \
        }                                                                                            \
        catch (Throwable, e)                                                                         \
        {                                                                                            \
            pass = false;                                                                            \
        }                                                                                            \
        finally                                                                                      \
        {                                                                                            \
            writefln("==========================================   [%-4s]", pass ? "PASS" : "FAIL"); \
        }                                                                                            \
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
void writefln(const char *format, ...)
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
void vwritefln(const char *format, va_list args)
{
    vprintf(format, args);
    puts("");
}

#endif //  _TESTING_H