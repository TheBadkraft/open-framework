#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/codex.h"
#include "../include/version.h"

char **GLYPH_TYPES = (char *[]){
    "<letter>", "<digit>", "<symbol>", NULL};
char **GLYPHS = (char *[]){
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",
    "0123456789",
    "`~!@#$%^&*()_-+=[]{}\\|;:'\",.<>/?"};

typedef struct
{
    enum Glyph type;
    glyph *glyphs;
} glyph_t;
typedef struct
{
    char *name;
    version *ver;
    glyph_t glyphs[3];
    size_t glyph_count;
} codex_t;

static codex_t *codex;

static size_t count_string_array(char **source)
{
    char **ptr = source;
    char *c = *ptr;
    while (c)
    {
        // printf("%s\n", c);
        c = *++ptr;
    }

    return ptr - source;
}
static void writeln(char *text)
{
    printf("%s\n", text);
}
static void print_info()
{
    const char *format = "%s Codex [%s]";
    char *ver = Version.to_string(codex->ver);

    int length = snprintf(NULL, 0, format, codex->name, ver);
    char *info = malloc(length);

    sprintf(info, format, codex->name, ver);
    writeln(info);

    free(ver);
    free(info);
}
static bool load_glyphs()
{
    size_t count = count_string_array(GLYPH_TYPES);

    int ndx = 0;
    // char **ptr = GLYPH_TYPES;
    // char *pNdx = *ptr;
    glyph_t *pGlSet;
    glyph *set = NULL;
    enum Glyph type;

    while (ndx < count)
    {
        set = GLYPHS[ndx];
        type = (enum Glyph)ndx;

        pGlSet = malloc(sizeof(glyph_t));
        pGlSet->type = type;
        pGlSet->glyphs = set;

        codex->glyphs[ndx] = *pGlSet;
        // printf("%s %s\n", GLYPH_TYPES[ndx], set);

        ++ndx;
    }

    codex->glyph_count = count;

    return true;
}

bool load_codex()
{
    codex = malloc(sizeof(codex_t));
    codex->name = "Sigma.C";
    codex->ver = Version.new(0, 1, 1, ALPHA, "tuxedo");

    if (load_glyphs())
    {
        print_info();
        return true;
    }
    else
    {
        writeln("failed to load glyphs");
        return false;
    }
}
void destroy_codex()
{
    free(codex->ver);
    free(codex);
}
void display_glyphs(enum Glyph gl)
{
    int ndx = 0;
    glyph *g = codex->glyphs[gl].glyphs;
    size_t count = strlen(g);

    printf("%-10s", GLYPH_TYPES[gl]);

    // printf("%c, %c", *g, *(g + 1));
    while (ndx < count)
    {
        printf("%c%s", *g, (ndx + 1) < count ? ", " : "");

        ++g;
        ++ndx;
    }

    printf("\n");
}

const struct Codex_T Codex = {
    .load = &load_codex,
    .destroy = &destroy_codex,
    .info = &print_info,
    .glyphs = &display_glyphs};
