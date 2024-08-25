# OpenFramework
## OpenPlatform Specifications

### Section II

**Languages: _C_ & _ASM_**

`open_framework` is written in **C**. We are taking a purist approach to developing the platform to minimize confustion and maximize understanding. **C** has become a *scary* language to a lot of newcomers. It should not be a daunting task to write code in **C**. That is one of reasons this *Platform* exists.  

*OpenPlatform* specifications for **C**:
- With rare exceptions, objects that should be *`freed`* by the caller will not be returned by a function. Instead, construct the function definition with an *out* parameter.
 - *Example*
 - > *Don't*
 ``` c
 string *get_name(int id);
 ```
 - > *Do*
 ``` c
 bool get_name(int id, string **);
 ```


| Previous | | Next |  
| :-------------- | :--------------: | --------------: |  
| [Project Structure](proj_structure.md)       |   [Table of Contents](../README.md)   | [Reusable Code](reusable_code.md) |
