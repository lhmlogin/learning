#include<stdio.h>
#ifdef WINDOWS_NT
    #define MSG "I am in Windows"
#else
    #define MSG "I am in Linux"
#endif

int main(void) {
    puts(MSG);
}
