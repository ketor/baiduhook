Demos as follow:
  * **System** shows hooking system function
    * **disk**
      * [Write](http://code.google.com/p/baiduhook/source/browse/trunk/test/disk/write/writefile.cpp) trigger disk error by using default hooking library
        1. `bhook ./writefile`
        1. `pgrep writefile`
        1. `bhook -e write -p 100 -i 28 <pid>`
      * trigger disk error by using user hooking library.(Source:[user\_write.cpp](http://code.google.com/p/baiduhook/source/browse/trunk/test/disk/user_write/user_write.cpp), Hook:[hook.cpp](http://code.google.com/p/baiduhook/source/browse/trunk/test/disk/user_write/hook.cpp))
        1. `bhook -u hook.so ./user_write`
    * **memory**
      * [Malloc](http://code.google.com/p/baiduhook/source/browse/trunk/test/memory/api/malloc.cpp) hooks system function **malloc** by using API
    * **net** similar to disk demos
      * Network is unreachable:  `bhook -e connect -p 100 -i 101 <pid>`
      * Connection reset by peer:  `bhook -e recv -p 100 -i 104 <pid>`
    * **other**
      * hooks system function **printf** through shared library.(Source:[main.cpp](http://code.google.com/p/baiduhook/source/browse/trunk/test/system/user_printf/main.cpp), Hook:[my\_printf.cpp](http://code.google.com/p/baiduhook/source/browse/trunk/test/system/user_printf/my_printf.cpp))
        1. `bhook -u my_printf.so ./main`
  * **Program** shows a variety of hooking functions for the program
    * [Simple](http://code.google.com/p/baiduhook/source/browse/trunk/test/program/simple/hook.cpp) hooks simple function through shared library
    * [Library](http://code.google.com/p/baiduhook/source/browse/trunk/test/program/library/attachlib.cpp) hooks static library function by using API.
    * [API](http://code.google.com/p/baiduhook/source/browse/trunk/test/program/api/api.cpp) hooks system function **fread** and program function **func**
  * **Unittest** shows using API with google test.
    * [Simple](http://code.google.com/p/baiduhook/source/browse/trunk/test/unittest/simple/test_attach.cpp) hooks simple function.
    * [Class](http://code.google.com/p/baiduhook/source/browse/trunk/test/unittest/class/test_class.cpp) hooks class public/private function.
    * [SharedLibrary](http://code.google.com/p/baiduhook/source/browse/trunk/test/unittest/sharedlibrary/test_so.cpp) hooks shared-library function.