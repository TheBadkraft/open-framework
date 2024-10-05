Welcome to the **_`OpenPlatform [OPI]`_** `open_framework.core` library. I lay all my thoughts and processes out here for everyone to see where I'm coming from and, if you can figure it out, where I'm going. At this point, I'm sure someone has a padded room all ready for me. I haven't put anything on paper much about the *`core`* part of the open_framework. It's been slowly budding and growing as I need things. Naturally, my first pitch into C was working with strings. Fun ... okay, not really. But it got me back into the swing of the C paradigm. An interesting thing happened along the way, though - my *C#* self began having conflicts with the *C* self. So I had to compromise. I'll let you know up front - if you are a *C purist*, you might not like what I've done with C. On the other hand, maybe you will. It's a little different because my natural instinct is to want to work in an object-oriented paradigm. I wouldn't say C is the furthest thing from OOP ... but it's not really in the same ball park. Otherwise, it would be like putting some bases on the ground and inviting your friends over for a game of baseball. But you don't have a bat, baseball, gloves, or even a pitcher's mound. But you have bases, right?  

Okay, enough of that. I began working on a core library to get re-acquainted with C. I began by creating a `string` struct and that became really tedious to use. I adopted a form of OO with the introduction of what I call *interfaces*. In the header file we build the following:

``` c
//	.h

extern const struct IInterface {
  // function members
  void(*do_foo)(int, string);
} Interface;
```

Then, in the source file we fill it all in:

``` c
//	.c

void i_do_foo(int, string);

void i_do_foo(int id, string name) {
	// do your foo here...
}

const struct IInterface Interface {
	.do_foo = &i_do_foo
}
```

As I increased the amount of work going into the *core* components - `IFile`, `IDirectory`, `IPath`, `IStream` - I began having issues with `malloc`s and keeping track of what needed to be freed ... <*groan*>. So, I came up with `IAllocator`. All the interface that implement a `.new(...)` function will have a `.dispose(...)` function. And everything that is `malloc`'d will go through the Allocator. This is the first object where I implemented any sort of collection. It is a list of re-usable elements. The Allocator allocates 64 elements in it's list (so far, I have yet to need to increase it). When objects are `malloc`'d, it's handle, or `uintptr_t` is stored in the first empty element. As objects are *`disposed`*, we free and remove the item from the allocated collection. Pretty simple.  

Now, I've known for a bit that I was going to need to come up with some collection components. I was putting it off until it was needed. Well, it's needed...  

I started working on my indexers, lexers, and parsers and I was trying to get things to work without creating a collection. It can work, but it's difficult to manage, very rigid, not very expressive. In short, I need to create a couple of collection (enumerable and iterator) function as well as a basic core collection object. From the core collection object, we can specialize into probably 5 different types of specialized collections:
1. `List`
2. `Stack`
3. `Queue`
4. `Dictionary`
5. `HashSet`

One thing I learned while writing the Allocator is what's best for storing pointers. For the sake of portability, `void *` is not it. I already mentioned `uintptr_t`. This will cast up to `void *` and vice versa. This is required by C99 standard if I recall. So, look for the core collection to simply be something like `typedef handle *array`. That just makes for a whole lot less typing. And, once you get acclimated to my C paradigm, it will make sense.  

In the Allocator, I use `handle` as the alias for `uintptr_t`. See `types.h` for more flavor ... like `typedef void *object`. If you are familiar with C#, this may make more sense. I'm not trying to make something C is *not*. I'm working within what C allows to emulate as closely as possible something it isn't. I draw the line, however, at writing huge production macros. I use very few and only where I'm implementing a truly useful macro, not writing reems of source so I can have syntactic sugar. They say, "Sugar rots the teeth." Well, syntactic sugar rots the brain. It's like buying the muscle car already muscled out and not knowing a thing about what's going on under the hood. But, I digress ... again.

Obviously, the issue with such a collection is that it is not a homogenous collection ... that is, neither the `collection` nor its interface enforce *type safety*. You can place any *`handle`* (`uintptr_t`) in the array and it doesn't care. Well, I don't either. But, as I was pondering the best path forward I decided that I am providing the building blocks. It is up to the developer to put them together in a way that is suitable for his/her requirements. I will have a very tight approach to developing 3 very basic types of *collections*: array, set, key/value. I will demonstrate how to create a type-safe interface when we get to that point. At some point someone may coerce me to write a macro that generates source for a type-safe collection. But, as of now, I am not inclined to venture into those weeds. There be snakes ...  

Beginning the collection object, here is my initial test output after a day of work:
> ```
> *** TEST CASE [_collection_add_multiple]
> Collection.add: add multiple elements to collection
> (0x55555555fc20 ... 0x55555555fc20 ... 0x55555555fc28) <- 0x55555555fe70
> (0x55555555fc20 ... 0x55555555fc28 ... 0x55555555fc30) <- 0x555555560090
> (0x55555555fc20 ... 0x55555555fc30 ... 0x55555555fc38) <- 0x5555555600b0
> (0x55555555fc20 ... 0x55555555fc38 ... 0x55555555fc40) <- 0x5555555600d0
> (0x55555555fc20 ... 0x55555555fc40 ... 0x55555555fc48) <- 0x5555555600f0
> (0x55555555fc20 ... 0x55555555fc48)
> mem Diff: 40
> count: 5
> collection data:
> ---------------------------------------
> [0] (0x55555555fc20 : 0x55555555fc48) person: [0] Sam
> [1] (0x55555555fc28 : 0x55555555fc48) person: [1] Harold
> [2] (0x55555555fc30 : 0x55555555fc48) person: [2] Betty
> [3] (0x55555555fc38 : 0x55555555fc48) person: [3] James
> [4] (0x55555555fc40 : 0x55555555fc48) person: [4] Jill
> =======================================
> ```

What you're looking at is analysis of the memory locations and what is going into them. The top 5 entries are demonstrating the beginning memory location of the list `0x55555555fc20` followed by the *current* element location, again `0x55555555fc20`, and the terminal element location after adding the element, `0x55555555fc28`. You can trace the advancement of the terminal element with the advancement of the current element. The middle memory location - or, *current* element - is where the object is being stored. The stored object is the last memory location. Each object being stored should be `malloc`'d or else ... well, behavior will be undefined.  

The *collection data* section is simply outputing the collection's stored data. The memory locations enclosed in `(...)` correlate to what we see in the top section. In this case, the left location is the middle *current* element location. The numbers in brackets are indices (on the left) and data (on the right) -- `p->id`.

It's worth noting that one should not use this collection to store primitives. You should `malloc` your memory to ensure persistence.  

This is how simple iteration of the collection is:

``` c
handle *hPtr = coll->list;
while (hPtr != (handle *)(coll->last))
{
    person *pData = (person *)(*hPtr);
    printf("person: [%d] %s\n", pData->id, pData->name);

    ++hPtr;
}
```



