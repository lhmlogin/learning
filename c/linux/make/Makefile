accumulate: main.o calc.o echo.o
	gcc -o accumulate main.o calc.o echo.o

main.o: main.c calclib.h
	gcc -c main.c

calc.o: calc.c
	gcc -c calc.c

echo.o: echo.c
	gcc -c echo.c

clean:
	rm *.o
