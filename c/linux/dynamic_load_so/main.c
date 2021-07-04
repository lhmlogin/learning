#include <stdio.h>
// 首先导入了dlfcn.h头文件
#include <dlfcn.h>

extern int minus(int a, int b);

int main() {
    void* handle1 = dlopen("./surf.so", RTLD_LAZY);  // dlopen的第一个参数制定.so文件位置，可以使用绝对路径。第二个参数可以自行查阅资料
    void* handle2 = dlopen("./add.so", RTLD_LAZY);
    int (*add)(int a, int b), (*valor_analysis_surf)(void);  // 定义一个函数指针

    /*根据动态链接库操作句柄与符号，返回符号对应的地址*/
    add = dlsym(handle2, "add");  // 获取函数
    valor_analysis_surf = dlsym(handle1, "valor_analysis_surf");

    int x1 = add(7, 8);  // 执行函数
    printf("7 + 8 = %d\n", x1);
    int x2 = minus(9, 8);  // 执行函数
    printf("9 - 8 = %d\n", x2);
    valor_analysis_surf();

    dlclose(handle1);
    dlclose(handle2);

    return 0;
}


