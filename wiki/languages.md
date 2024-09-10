# OpenFramework
## OpenPlatform Specifications

### Section II

**Languages: _C_ & _ASM_**

`open_framework` is written in **C**. We are taking a conservative approach to developing the platform to increase clarity. **C** has become a *scary* language to a lot of newcomers. It should not be a daunting task to write code in **C**. That is one of reasons this *Framework* exists and the driving force behind the **OPI**.  

*OpenPlatform* specifications for **C**:
- With rare exceptions, objects that should be *`freed`* by the caller will not be returned by a function. Instead, construct the function definition with an *out* parameter.
  - *Example*
    - > *Don't*
      >``` c
      >string *get_name(int id);
      >```
    - > *Do*
      >``` c
      >bool get_name(int id, string **);
      >```
  - ***Exception***
    - >```c
      >string *my_str = String.new();
      >```
- The exception is allowed under the specification because of long practice (_familiarity_) and ease of use.
  - >*Example*: compare the following two conventions
    >``` c
    >//  with the exception, we can return a new file object to the Stream.new(...) function
    >Stream.new(File.new("./my_file.txt"), READ | WRITE, &pStream);
    >
    >// ... or
    >//  strict adherance to the specification
    >file *pFile;
    >File.new("./my_file.txt", &pFile);
    >Stream.new(pFile, READ | WRITE, &pStream);
    >```
  - Both are correct implementations of the specification. The intent behind this specification is to convey to the caller that they are responsible for calling `free(...)` when it is appropriate. In this particular implementation, `Stream.free(stream *)` will also close the encapsulated `file`.
- Use the `Allocator` to manage allocations and deallocations:
  - `char *myString = Allocator.alloc(25);`
  - `Allocator.dealloc(myString);`


| Previous | | Next |  
| :-------------- | :--------------: | --------------: |  
| [Project Structure](proj_structure.md)       |   [Table of Contents](../README.md)   | [Reusable Code](reusable_code.md) |
