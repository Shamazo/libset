all: set.c rb_tree.c test.c
	gcc -c -g -Wall set.c -o set.o 
	gcc -c -g rb_tree.c -o rb_tree.o
	ar rcs libset.a set.o rb_tree.o
	gcc -g test.c -L. -lset -o test.o
clean:
	rm -f wildcard *.o
lib: set.c rb_tree.c 
	gcc -c -Wextra set.c -o set.o 
	gcc -c rb_tree.c -o rb_tree.o
	ar rcs libset.a set.o rb_tree.o
test: test.c
	gcc -o test.o test.c -L. -lset
example: example.c
	gcc -g -o example.o example.c -L. -lset
memcpytest: memcpytest.c
	gcc -g memcpytest.c -o memcpytest.o