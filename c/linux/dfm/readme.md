gcc -o main main.c -lsurf -L./bin -Wl,-rpath=./bin
gcc -o main main_simple.c -l:surf.so -L./bin -Wl,-rpath=./bin

gcc -shared -o ./bin/libsurf.so surf.c -fPIC
gcc -shared -o ./bin/surf.so surf.c -fPIC
gcc surf.c -c -fPIC -o surf.o
ld surf.o -shared ./internal_func.so -o surf.so


###编译生成动态加载主函数，并且map所有函数到程序空间
gcc -o main main.c -ldl -rdynamic

The -rdynamic option puts all symbols in the dynamic symbol table (which is mapped into memory), not only the names of the used symbols. Read further about it here. Of course you can also provide function pointers (or a struct of function pointers) that define the interface between the library and your main program. It's actually the method what i would choose probably. I heard from other people that it's not so easy to do -rdynamic in windows, and it also would make for a cleaner communication between library and main program (you've got precise control on what can be called and not), but it also requires more house-keeping.


gcc -shared lib_attr.c -o ./lib_attr.so -fPIC -I ../inc

####
gcc balance.c ./lib/*.c -c -fPIC -o balance.o -I /home/hamm/ncc/c-style/inc
ld balance.o -shared ./lib/so/*.so -o balance.so
ld balance.o -shared /frontline/incam/release/bin/prog/lib/lib_export.so -o balance.so

gcc -fPIC -shared -o balance.so lib_balance.c balance.c -I /home/hamm/ncc/c-style/inc

ld balance.so -shared /frontline/incam/release/bin/prog/lib/lib_export.so -o balance.so

gcc balance.c lib_balance.c -c -fPIC -o balance.o -I /home/hamm/ncc/c-style/inc
###not work

####work compile code
gcc balance.c lib_balance.c -c -fPIC -I /home/hamm/ncc/c-style/inc
ld balance.o lib_balance.o -shared /frontline/incam/release/bin/prog/lib/lib_export.so -o balance.so
####

###link will generate big so file
gcc *.c -c -fPIC -I /home/hamm/ncc/c-style/inc
ld balance.o ./lib/o/*.o -shared /frontline/incam/release/bin/prog/lib/lib_export.so -o balance.so
###


###still very big
gcc balance.c -c -fPIC -ffunction-sections -fdata-sections -I /home/hamm/ncc/c-style/inc
gcc *.c -c -fPIC -ffunction-sections -fdata-sections -I /home/hamm/ncc/c-style/inc
ld balance.o ./lib/o/*.o --gc-sections -shared /frontline/incam/release/bin/prog/lib/lib_export.so -o balance.so

ld balance.o -L ./lib/o/ --gc-sections -shared /frontline/incam/release/bin/prog/lib/lib_export.so -o balance.so  ###not work
####



###list so symbol names.
nm -D <So file name>






gcc -o hello hello.c -I /home/hello/include -L /home/hello/lib -lworld
上面这句表示在编译hello.c时：

-I /home/hello/include表示将/home/hello/include目录作为第一个寻找头文件的目录，寻找的顺序是：/home/hello/include-->/usr/include-->/usr/local/include 

 

-L /home/hello/lib表示将/home/hello/lib目录作为第一个寻找库文件的目录，寻找的顺序是：/home/hello/lib-->/lib-->/usr/lib-->/usr/local/lib

 

-lworld表示在上面的lib的路径中寻找libworld.so动态库文件（如果gcc编译选项中加入了“-static”表示寻找libworld.a静态库文件），程序链接的库名是world


使用GCC编译动态链接库的项目时，在其他目录下执行很可以出现找不到动态链接库的问题。

这种情况多发生在动态链接库是自己开发的情况下，原因就是程序运行时找不到去何处加载动态链接库。

可能会说在编译时指定了链接的目录啊！编译时指定的 -L的目录，只是在程序链接成可执行文件时使用的。程序执行时动态链接库加载不到动态链接库。


解决办法有两种，第一程序链接时指定链接库的位置，就是使用-wl,-rpath=<link_path>参数，<link_path>就是链接库的路径。如：

gcc -o foo foo.c -L. -lfoo -Wl,-rpath=./
