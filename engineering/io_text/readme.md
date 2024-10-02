As I began to re-aquaint myself with C, I had come across some blogs on a more OO way to code in C. One tutorial on object-oriented programming in C had used a **`string`** (`char*`) *-like* `struct` to demonstrate the concept. I mimicked the process because it actually made C's verbosity more tolerable. In a way, an OO approach made C *easier* and *more approachable*. That said, there came the realization that C++ (in my opinion) was an awful attempt at making C development less of a burden. It is a verbose language - what with the prototyping and header files. The real nail in the coffin for me was the constant CMake nagging the C++ connoisseurs - i.e. savvy soothsayers of the code. The problem I have with C++ is that it adds more than necessary to make C *more approachable*. Instead, again, in my opinion, C++ got added a bunch more ways to do things without any safeguards to prevent novice developers from thinking they didn't have to `free` any of their `malloc`s because "I'm using C-C plus plus. It manages memory for me." >,O  

Well, okay ... maybe not everyone thinks that but when it's happened enough you can recall a spedific incident, then that's too often. But, the 2nd big reason I am avoiding any significant development with C++ is because I don't want to learn **`CMake`**. I'm using strictly **GCC** for compiling. I'm learning how to use the command line to achieve what I need. It would be nice if there were a .json format I could give GCC to configure what kind compiling I need done. Of course, many may argue that a .json file is just a specialized format of a *make* file. I'll concede that is a valid counter to my opinion. So, then it simply comes to what I find *easier* to use. ;)  

I digress ... way off sidebar range. Back to what is the back ground on my motivation. The *`String`* project showed me something immediately that C was lacking. If you're familiar with the `public static class String` in C#, you know that it has a bunch of static methods that require a `string` type to operate on: `String.split(myString, " ")`, for example. My goal was to attempt this in C, beginning with a `struct string_;`.

``` c
//	string.h

struct string_ {
  char *buffer;
  size_t size;
} string;
```

After a little while this struct became cumbersome. Any time I was dealing with the need for a `char*` I used the `struct string_` which meant I was always going that extra level to reference the `str->buffer`. I scrapped the `struct` and settled for something a bit unorthodox.  

``` c
typedef char *string
```

This is unorthodox in any purist's circle - *you don't hide the fact that it's a pointer*. "Why not?" When you see the variable, look at how its members are referenced - that is, the context in which it is used.  

``` c
void some_string func(Foo myFoo) {
  myFoo.id = new_id();
}

// ... or ...

void some_string_func(Foo myFoo) {
  myFoo->id = new_id();
}
```

The context is clear. You can also hover over the declared type (in most IDEs) and it will tell you. Also, if you make it a way of differentiating what objects you intend to use as *value* types versus *reference* types. To me using `string` as a `char*` made sense. This way, my code will tell the reader whether I am using an otherwise ambiguous `char*` as a *pointer* or an actual *char array* - or, `string`. I have applied that concept to other types:

``` c
typedef struct io_file *file;
typedef struct io_dir *directory;
typedef struct io_stream *stream;
```

I know that each of these should be used as a reference-type object rather than value-type. When I specifically wish to pass on in the value-type context, it is then on purpose. In that instance, the purpose is clearly observed by using the more verbose `struct io_file` declaration.

Even though I removed the `struct io_string` object, I kept the interface I developed. This is the paradigm I continue writing in because it is very simple to repeat. Once you know how the pattern repeats itself, the more comfortable it becomes.

``` c
// string.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

extern const struct Open_String
{
    //  empty string
    string const empty;
    //  Determine if a string is empty; returns FALSE for NULL
    bool (*is_empty)(string);
    //  Determine if a string is null or empty
    bool (*is_null_or_empty)(string);
    //  Initializes a new string to specified length
    void (*new)(size_t, string *);
    //  Allocates a new string from a copy of the given source
    void (*alloc)(string, string *);
    //  Resize the current string to the new size
    void (*resize)(size_t, string *);
    //  Copy string text to destination
    void (*copy)(string, const string);
    //  Begin copy source to destination at position
    void (*copy_to)(string, string, int loc);
    //  Copy substring to new string
    string (*subcopy)(string, int, int);
    //  [NOT WORKING] Determines whether the current string is freeable
    bool (*freeable)(string);
    //  Free the current string
    void (*free)(string);
    //  Returns the string length
    size_t (*length)(string);
    //  Appends the supplied string at the end of thecurrent string
    void (*append)(string, const string);

    /*
        //  Appends a formatted string to the current string
        void (*appendf)(string, const string, ...);
     */

    //  Formats the given parameters returning a new string
    void (*format)(const string, string *, ...);
    //  Truncate the current string
    void (*truncate)(size_t, string *);
    //  Join multiple strings separated with the given delimeter.
    void (*join)(string, string *, ...);
    //  determines the size of the joined string with delimeter
    size_t (*join_len)(string, ...);
    //  Split a string on a given delimter.
    string *(*split)(char, string);
} String;
```

At first I called this an API. It is more like an interace to the **`string`** (`char*`) object. This is an architectural pattern for implementing modular programming at a pseudo-component level. It's pseudo because C isn't an object-oriented language. So, the architectural pattern I'm implementing gives a pseudo-OO facade. It's close enough to make C *more approachable*. But this is just the *interface*. Now we have to implement the interface ... like so:

``` c
//	string.c
const struct Open_String String = {
    .empty = "",
    .is_empty = &str_empty,
    .is_null_or_empty = &str_null_or_empty,
    .new = &str_new,
    .alloc = &str_alloc,
    .resize = &str_resize,
    .copy = &str_copy,
    .copy_to = &str_copy_to,
    .freeable = &str_freeable,
    .free = &str_free,
    .length = &get_str_len,
    .append = &str_append,
    .format = &str_format,
    .truncate = &str_trunc,
    .join = &str_join,
    .join_len = &str_join_len,
    .split = &str_split,
};
```

Each function assignment is a *pointer* to one of the functions in the source. For example:

``` c
//	a few sample methods
/*
    Determine if a string is null or empty
*/
bool str_null_or_empty(string pStr)
{
    if (!pStr || strcmp(String.empty, pStr) == 0)
    {
        return true;
    }

    return false;
}
/*
    Initializes a new '\0'-filled string to the specified length; adds 1 for string terminator
*/
void str_new(size_t capacity, string *out)
{
    (*out) = Allocator.alloc(capacity + 1, INITIALIZED);
}
/*
    Allocates a new string from a copy of the given source
*/
void str_alloc(string source, string *out)
{
    str_new(strlen(source), out);
    strcpy((*out), source);
}

void str_resize(size_t size, string *out)
{
    size_t len = get_str_len(*out);
    if (len == size)
    {
        return;
    }
    if (len > size)
    {
        str_trunc(size, out);
    }
    else
    {
        (*out) = realloc(*out, size);
    }
}
```

As I said earlier, I have applied this same technique to several other objects. The concept is still going through its paces being put into practice. A reproducable pattern will speed up development because of the repetition involved. Plus, there is an `Allocator` that can help diagnoze possible memory leaks by tracking *`allocations`* and *`deallocations`*. The idea fairly simple: Provide a few light-weight, core components that make C more approachable. Then we will begin putting together more libraries that wrap critical infrastructure into *interfacable* components.  

In all this effort, I have discovered the lack of a few things that would elevate the foundational C paradigm to a *high-level* language. C is unapproachable because developers learn to fear C -- either through other developers' attitudes toward C, or due to a catastrophic experience with memory implosion. I get it. And, C can be a daunting language model. Finding methods or functions isn't always intuitive, either. You have to know what *`header`* file to `include` so that you can use certain functions. For example, however, with `string`, all the functions you can carry out on `char*` are grouped together under the `String` interface.  

*"So why is all this in the `io_text` namespace?"*  

I'm getting to that ... now. Fast forward to this point and I had already decided that I will write a new, high-level iteration of C ... **_Sigma.C_**. In this project, I've already begun assembling some core components, a few I've already mentioned - `file` `directory`, `stream`. There are more coming along, especially in the *Sigma.C Compiler* source. Now, I am about to be diving into the *parser* side of the compiler and I'm learning about **_parser combinators_**. That is what is going to go on in here. Plus this namespace, `io_text`, gives me a place to put the `struct io_token` object and it's interface. There is an `Indexer` interface that will be in this namespace as well.  

We will have the standard tests for unit testing Tokens and the Indexer. The Parser, rather than working directly on a string source, will consume tokens. I'll be using MPC to learn what I'm doing with parser combinators. I feel like calling them *micro parsers*. We should be able to read up an EBNF like so:  

``` html
#  candidate: Sigma.C language definition
<program>   ::= <function>
<function>  ::= int <id> ( ) { <statement> }
<statement> ::= return <expr> ;
<expr>      ::= <int>
```

One thing I'm really thinking about is releasing a robust mathematics EBNF & Parser. With an input stream, one could feed formula into the number cruncher and get results in the output stream. I could see where some lambda functionality would be useful. ... anyway , just a wild thought. And the more I think about it, the more it makes sens to start with a Mathematical Machine Processor. A really strong focus on standardizing EBNF would be great but I think there is still some way to go with that because there has not yet been a widely accepted parser that is EBNF-friendly.  

But we might just give it a shot. There may be some configuration standards for EBNF which means we can provide a default configuration. If so, what would the configuration look like? JSON? Most likely.  

A legend of sorts:
``` html
<non-terminal> := <definition>
```

Look at the **_Sigma.C_** language definition above. I've seen some EBNF with non-terminals (always on LHS of the `::=` *operator*) annotated on the left without any enclosure - i.e., no '<' or '>'. However, non-terminals are always enclosed on the RHS. It would appear that the ISO standard is not followed by anyone 100%.  

``` html
program   ::= <function>
function  ::= int <id> ( ) { <statement> }
statement ::= return <expr>
expr      ::= <int>
```

Also, some EBN formats use ';' as end of statement symbols. I do not see the purpose in using a ';' at the end of a rule statement. Each statement is delimted by a new line ('\n') and if we want to carry a rule across multiple lines, we can escape the '\n'. Having looked at the ISO standard for EBNF, I don't see this being in direct conflict. Also, many, many sources presenting themselves as knowledge bases do not adhere to the ISO standard. That tells me that the ISO standard is (*likely*) too cluttered, not useful enough, or just not the style people like. So, I really don't mind going a bit on my own with the format. No doubt, I will fiddle with things until I am satisfied. My goal is to write the meta language (which is what EBNF is) as brief as possible while making parsing intuitive and unambiguous. Brevity is probably the more important so long as ambiguity is not the result. And, even though it is *clean* to see compulsory text not enclosed with ' or ", it creates ambiguity. For instance, where a '(' and ')' are compulsory, say for function parameters, other formats might describe

``` html
params ::= '(' [<id>],[',' <id>]+ | ['void'] ')' ;
```

The symbols enclosed in ' are *expected* ... or compulsory. Compulsory is not 100% accurate as, in the example above, *'void'* is enclosed in **[** and **]**. This makes `void` optional. But, if you're going to write something other than a parameter identifier, then it has to be `void` ... or just leave it blank. Without some enclosure, these compulsory (or *expected*) terms would look like this:

``` html
params ::= ( [<id>],[, <id>]+ | [void] ) ;
```

Do you see the glaring ambiguity already? There are two really big problems:
1. Parens **(** and **)** are grouping symbols in EBNF. Are we grouping something here or are we describing a syntactic format?
2. The first , (comma) between **[<id>]** and **[, <id>]+** is intended for concatenation (*thus I write it without spaces*); the second comma in **[, <id>]+** is intended to build a comma-delimited list of parameters. So, in the first example, the use of ' made it far more clear the purpose of each comma. So, I thought of possibly implementing an alternative:

``` html
params ::= \( [<id>],[\, <id>]+ | [void] \) ;
```

Escaping the normal interpretation of these symbols is an accepted practice with strings. While this does appear to resolve ambiguity, in the end, I think escaping symbols in the EBNF decreases readability and increases parser complexity. I think keeping the single **'** as the enclosure for expected values is probably the better choice. If it seems I am talking myself out of using an escape for end of statement (`\'n'` ), I am not (*yet*).  

I am going to start building *micro-parsers* (or, parser combinators) from the smallest pieces. I did this once about 10-12 years ago with C#. I didn't know what I had created but it was phenomenal and pretty fast. I am hesitant to use *`regex`* for letters and digits. I don't know why ... I just a am. Maybe it is the lack of readability. If I am going to include any kind of regular expression model, then that sequence must be recognized by the parser. And, introducing **_regex_** parsing, while cool and fancy to some, is a *black box* that isn't always portable and, unless you're name is Roger, ain't very readable.  

Now, I am not a fan of long, verbose options. For example:

``` html
letter ::= 'a' | 'b' | 'c' | ... | 'z' | 'A' | 'B' | 'C' | ... | 'Z' ;
```

Okay, it's long and tedious to type. I don't like looking at it because there is so much there just to say, *"A `letter` can be replaced by any one of _"*. This and digit is the **_only_** place I'd want to provide a regular expression. But if I do here, then, why wouldn't I have regular expression parsers elsewhere? Dilemmas ...  

The obvious answer then, is to do something a little different since every EBN format seems to take on a slightly different flavor:

``` html
letter ::= ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ":1)
```

I much prefer this and it's meaning can be understood pretty quick. First, I have a grouping notated with **(** and **)**. It's really just window dressing to aid in readability. So it's not necessary. Next, you'll see that I used the double " instead of the single ' used on otherwise *compulsory* symbols. Since I am creating a *list* of similar, atomic (*single character*) symbols and not a **compulsory** literal, I am using " to enclose the items. At the end of the list I use ':' to provide a parameter to the parser. Since we are creating a list of atomic symbols, we need to tell the parser *how many* or *how to* satisfy the condition. `letter` definitely implies a single selection, so we postfix the *list* with `:1`. It's brief and unambiguous. Consistency is the key and it's not often you will need this particular notation. In fact you won't really need this notation with my parser because, well, you won't see these in any EBN formats. They're built in. You can supply your own rule for `letter` and `digit` ... or just use `<letter>` or `<digit>` at your leisure in your own EBN format.

Taking all this into consideration, maybe '::=' could be more *brief* as well. **'::='** says *'expands into'* or *'can be replaced with'*. I'll be mixing and matching a bit until I find something I like. I may just drop one of the ':'. For now, this is what I'm starting with internally:

``` html
letter := "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ":|
digit  := "1234567890":|
```

I expect the indexer to tokenize like so:

>! tokens list:
>! -------------
>! letter
>! :=
>! "
>! abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ
>! "
>! :
>! |
>! \n

You'll see I'm using the '|' since it is as good as saying ':1' which means *just one*. The '"' mean an atomic list. There is no *delimeter* so it is assumed that each of the smallest elements enclosed in the **"** is a separate element of the list. I have something in mind that might expand on this concept, but we'll see how this develops.  

Okay, so, let's assume for now that I have internally defined a default EBN format (*above*). We've designated the parser **_op_**eration as an **or** function with the *pipe* (**|**). That's our first *micro-parser*. Return true as soon as the *input* finds a single match. But, before we can get to the parser, we need to lex ...  

First lex pass results in the following:
>! tokens:  
>! :> letter  
>! :> :=  
>! :> "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ":|  
>! :>  
>!  
>! --------------------------------  
