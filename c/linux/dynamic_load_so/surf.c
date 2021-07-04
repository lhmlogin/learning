#include <stdio.h>
//extern int run_in_main(void);
//extern int add(int a, int b);
extern int minus(int a, int b);

int valor_analysis_surf(void)
{
	int re;
    printf("I am in valor_analysis_surf function\n");
	//re=add(5,3);
	//printf("5 + 3 = %d\n",re);
	re=minus(5,3);
	printf("5 - 3 = %d\n",re);
    //run_in_main();
}
