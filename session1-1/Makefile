# Rule for generating the main program
1-1program : 1-1program.o 1-1circle.o
	gcc 1-1program.o 1-1circle.o -o 1-1program
# Compile source files
1-1program.o : 1-1program.c 1-1circle.h
	gcc -c 1-1program.c
1-1circle.o : 1-1circle.c 1-1circle.h
	gcc -c 1-1circle.c
# Clean object files
clean :
	rm -f 1-1program.o 1-1circle.o
