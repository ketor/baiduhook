# BHook #
BHook is a hooking framework for the C/C++ program on Linux platform.
  * It can dynamically replace the original function to change the program logic at runtime.
  * Supports for replacing any function, including shared library function(such as system function), static library function and program function.
  * Provides the default hooking library for system function.
  * It can be used for debugging, mocking, testing and so on.
  * Baidu C/C++ Mocking Framework is based on the BHook.


# Install #
```
$ ./configure --prefix={your install path}
$ make
$ make install
```
# Getting Started #
## Use the default hooking library ##
Let the program produce the disk full exception by hooking **write** function that set _errno_ to ENOSPC(`#define ENOSPC 28`) with 80% probability:
```
$ bhook -e write -p 80 -i 28 ./a.out
```
## User-defined system function ##
1. Source code：
```
//main.cpp
#include <stdio.h>                                                                                          
                                                                                                            
int main()                                                                                                  
{                                                                                                           
    printf("Hello, world!\n");                                                                              
    return 0;                                                                                               
} 
```
2. Hook code：
```
//my_printf.cpp
#include <stdio.h>                                                                                          
#include "bhook.h"                                                                                          
                                                                                                            
int my_printf(const char *format, ...)                                                                      
{                                                                                                           
    typedef int (*REALFUNC)(const char *format, ...);                                                       
    REALFUNC real_printf = (REALFUNC)get_real_func("printf");                                               
                                                                                                            
    real_printf("this is my printf function!\n");                                                           
    return 0;                                                                                               
}                                                                                                           
                                                                                                            
void __attribute__ ((constructor)) my_printf_init(void)                                                     
{                                                                                                           
    attach_func("printf", (void *)my_printf);                                                               
}
```
Compiled into a shared library：
```
$ g++ -shared -rdynamic -o my_printf.so -fPIC my_printf.cpp -I${BHOOK_ROOT}/include
```
3. Start and load the shared library by bhook：
```
$ bhook -u my_printf.so ./main
```
It will print out "this is my printf function!" instead of "Hello, world!".
## User-defined program function ##
1. Source code, the **func**function always returns 0:
```
//simple.cpp
#include <stdio.h>                                                                                          
                                                                                                            
int func()                                                                                                  
{                                                                                                           
    printf("this is func\n");                                                                               
    return 0;                                                                                               
}                                                                                                           
int main()                                                                                                  
{                                                                                                           
    int ret = func();                                                                                       
    if(ret == 0) {                                                                                          
        printf("Everything is OK!\n");                                                                      
    } else {                                                                                                
        printf("Oh, no! Impossible!\n");                                                                    
    }                                                                                                       
    return 0;                                                                                               
}
```
Compile and run, the output is:
```
this is func
Everything is OK!
```
2. Hook code：
```
//hook.cpp
#include <stdio.h>                                                                                          
#include "bhook.h"                                                                                          
                                                                                                            
int my_func()                                                                                               
{                                                                                                           
    printf("this is my hook func and will return -1!\n");                                                   
    return -1;                                                                                              
}                                                                                                           
                                                                                                            
void __attribute__ ((constructor)) hook_init(void)                                                          
{                                                                                                           
    attach_func_lib("func", (void *)my_func);                                                               
}
```
Compiled into a shared library：
```
$ g++ -shared -rdynamic -o hook.so -fPIC hook.cpp -I${BHOOK_ROOT}/include
```
3. Start and load the shared library by bhook：
```
$ bhook -u hook.so ./simple
```
Since the **func** function is replaced by **my\_func** that always returns -1, the output will be:
```
this is my hook func and will return -1!
Oh, no! Impossible!
```