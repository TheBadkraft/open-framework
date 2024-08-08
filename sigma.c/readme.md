## Sigma.C ## 
What is **Sigma.C** (or, *Sig.C*)? A new derivative of C. The name may change to something else but I hope it retains the `C` moniker.

The goal is that most C will compile under *Sig.C*. We wil be extending the features allowing some "work" to be done under the hood.

There are 4 goals with the simple main program. Each of the following are valid:

1. `int main(int argc, char *argv[]) { ... }`
2. `int main(int argc, char **argv) { ... }`
3. `int main(void) { ... }`
4. `int main() {... }`

It is also worth noting that `return 0` is not necessary.

Furthermore, testing `main` entry with variadic parameters: `int main(int argc, ...) { ... }`
GCC does compile this adding about 15 lines of source to the resulting ASM.

Right off, I think we need to be able to call C functions from *Sig.C*. Knowing what that looks like from ASM is not difficult.

The *Sig.C* specification will provide for all 5 of the accepted entry points in addition to a method-less approach:

``` c#
//  main.ct

printf("Welcom to Sig.C\n");
```

This brings into question how we access command parameters. Before we do that, however, let's resolved iterating over variadic parameters.
One of the goals with *Sig.C* is to reduce dependency on `#define` as a mechnanism to generate code. Aside from the fragility of `#define`, it is error-prone, ugly, and hides a lot from the reader.

Variadic iteratation depends on telling the compiler the last arg prior to the `...` (ellipses). Then the compiler gets to do some fumbly stuff depending on ... once again, `#define`d source and some *`__built_in`* goodies.

We ought to have an iterator mechanism. Let's use a `keyword` to let the compiler know what we're doing. We can also, then, provide proper iterator functionality.

An `Iterator` mechanism then brings us to understand how collections work and, therefore, how an *iterator* traverses the collection, knowing when it is done.  

First, however, let's accomplish 2 goals:
1. generate source **as** source (ASM) from *Sig.C* (*method-less* `main.ct`)
2. include (import ...???) C function `printf`

## ASM ##
What does the `as` look like?
A bare-bones ASM file is as follows:
``` c#
.global	_start

.text
_start:
	movl  $1, %eax   # 1 ("exit") -> EAX
	movl  $0, %ebx   # 0 (with success) -> EBX
	int   $0x80      # see previous
.data
```

This theoretically renders as:
``` c
// basic, bare-bones executable
int main()
{
}
```

What does the equivalent GCC .s source look like?
``` c#
	.file	"main.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 12.3.0-1ubuntu1~23.04) 12.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
```

*NOTE: I would like to keep comments in the structure. This can help track comments like place holders and know where the code is.*  

Lastly, unless there needs to be some reason to add the `.` directives that GCC employs, we are going to try and stick to forming correct **as** ASM. It is part of the GCC tool chain so we should be able to find all the necessary documentation to make this work.

What are we going to write the *Sigma.C* toolchain in? That is, the parsers, pre-processors, etc, etc.? I am looking at a hybrid approach. I think it is extremely noble that *FASM* is written in assembly. But, I'm partial to letting the bits that have already been developed to do certain tasks be put to use. I don't have a particular inclination to introduce any C++. So, we will use a combination of ASM & C ... maybe, eventually, *Sigma.C* will take the place of C. For now, though, I think C is perfectly suited for the task at hand.

What are our expectations? Well, let's jump right in and see what I envision for *Sigma.C*.  

Let's assume what we want is the following:
``` c#
.global	_start

.text
_start:
	movl  $4, %eax   # 4 (code for "write" syscall) -> EAX register
	movl  $1, %ebx   # 1 (file descriptor for stdout) -> EBX (1st argument to syscall)
	movl  $msg, %ecx # 32-bit address of msg string -> ECX (2nd argument)
	movl  $len, %edx # length of msg string -> EDX (3rd arg)
	int   $0x80      # interrupt with location 0x80 (128), which invokes the kernel's system call procedure

	movl  $1, %eax   # 1 ("exit") -> EAX
	movl  $0, %ebx   # 0 (with success) -> EBX
	int   $0x80      # see previous
.data
msg:
	.ascii  "Welcome to Sigma.C!\n"  # inline ascii string
	len =   . - msg           	 # assign (current address - address of msg start) to symbol "len"
``` 
Given the above assembly from our intermediate compilation, we might expect:
``` c#
//	file: main.ct

//  include syntax is similar; simplified grammaticals
#include io.h;

//  standard return data type
int main () {
    //  standard modernized output syntax
    writef("Welcome to Sigma.C\n");

    //  return value is not implicitly required
}
```

Why keep the *`#include *.h;`* syntax? 
Options ... C# doesn't require a *header* file because it generates symbol files. We could have symbol files generated as *Sig.C* matures. Then we have a hybrid approach. Symbol files are a partially compiled format where headers are fully legible. There are advantages to both and likely may have reasons that both make sense. My initial approach at this point (because I don't have any compiled source or header files) is to create something of a symbol file. Might look something like this:

``` c#
!# sigma.c(asm)

void writef(len, msg) {
    movl  $4, %eax   // 4 (code for "write" syscall) -> EAX register
	movl  $1, %ebx   // 1 (file descriptor for stdout) -> EBX (1st argument to syscall)
	movl  $msg, %ecx // 32-bit address of msg string -> ECX (2nd argument)
	movl  $len, %edx // length of msg string -> EDX (3rd arg)
	int   $0x80      // interrupt with location 0x80 (128), which invokes the kernel's system call procedure
}
```

That's it ... simple.
So, now we just need a ~~parser~~ *Lexer*, right? Okay. But that's just the beginning ...

## Parsing ##
We are going to start small. We want to understand what we are doing. I found a great article by Nora Sandler that seems like it will get things going in the right direction. I also looked at how we might write our own custom front end for GCC. Both of these are viable options; indeed, both will likely be the intention. With a custom compiler we are going to be able to know exactly what we want from a GCC front end.

Without further ado ...

**NOTE: Nora is using AT&T _assembly syntax_. We will be using GCC's AS syntax.**

Nora writes a simple *C* `main` method:

``` c
int main() {
    return 2;
}
```

This will be our first task - to parse our *Sigma.C* `main` method. At this point, we look identical.

We need 3 things:
1. Codex -- provides information about how we build our language
2. Lexer -- identifies terms built from the Codex
3. Parser -- parses source files based on instructions by the Lexer

We are going to start simple. The *Codex* contains **_glyphs_** and **_terms_**.

**Glyphs**
- letters
- digits
- symbols

**Terms**
- keywords
- ~~data types~~	*(removed - data types are general keywords)*
- ~~decorators~~	*(replaced with **paired**)*
- paired
- matched
- identifiers
- operators
- terminals
- etc (TBD)

Nora suggests using *regex* to parse. I'm not too keen (*personally*) on regex. I will take a kind of Backus-Naur approach. Regardless, Here is what we need to identify:

_identifier_:
 **main**

~~_decorators_~~ _paired_:
 **()**
 **{}**

_keyword_:
 **int**
 **return**

_value_:
 **2**

_terminal_:
 **;**

As of 8/8/2024 ... 
*Lexer* is performing basic functions:
1. *naive* tokenization - with a naive set of rules, the *Lexer* creates a linked list of tokens
2. basic term recognition - determines **keyword**, **paired**, etc.
*3. counts all paired symbols to ensure each is closed* (by the end of the day ...)


In my previous attempts to develop a compiler, I had not built a *Lexer*. I was putting all the work into the *Parser*. I have been aware of *Backus-Naur* and under `C#` I had a great parser using *Backus-Naur*. Nora goes into *Backus_Naur* as well. As we move forward, I won't intentionally use any *RegEx*.


References:
[Nora Sandler](https://norasandler.com/2017/11/29/Write-a-Compiler.html)
