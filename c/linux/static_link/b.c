int printf( const char* format, ...);
int shared = 1;
void swap (int* a, int* b)
{
	*a ^= *b ^= *a ^= *b;
}

void echo (int value)
{
	printf("%d",value);
}

