#include <stdio.h>
#include "names_st.h"
#include "names_st.h"

int main(void) {
    names candidate;
    int flag = FLAG;

    get_names(&candidate);
    printf("Lest's welcome ");
    show_names(&candidate);
    printf(" to this program!\n");
    printf("%d\n",flag);
    return 0;
}