#include<stdio.h>
#include<stdlib.h>
#include"calclib.h"

int main(int argc,char *argv[]){
	int num=atoi(*++argv);
	int acc;
	acc=calc(num);
	echo(acc);
}

