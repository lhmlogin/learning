#ifndef NAMES_ST_H_
#define NAMES_ST_H_

#include <string.h>
#define SLEN 32

struct names_st
{
    char first[SLEN];
    char last[SLEN];
};

typedef struct names_st names;

void get_names(names *);
void show_names(const names *);
char * s_gets(char* st, int n);

#if defined(NAMES_ST_H_)
    #define FLAG 1
#else
    #define FLAG 0
#endif


#endif

