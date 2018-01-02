#include "set.h"
#include <stdio.h>
#include <stdlib.h>

//functions to pass to create_set
int int_comp(const void* a,const void* b) {
	if( *(int*)a > *(int*)b) return(1);
	if( *(int*)a < *(int*)b) return(-1);
	return(0);
}

void dint(void * a){
		free((int*)a);
	}

void print_int(const int* a){
		printf("%d\n",*a);
	}

int main(int argc, char* argv[]) {


	/* creating an empty set of integers. The last argument is the size of 
	each element in the set. This is neccassary to allow the library to be
	as general as possible and allow for any type of element. */
	set seta = set_create(int_comp, dint, print_int, sizeof(int));
	/* Sets keep track of elements using pointers. Whenever you insert an
	element you pass in a pointer. 
	*/
	int * x = malloc(sizeof(int));
	int * y = malloc(sizeof(int));
	int * z = malloc(sizeof(int));
	int * w = malloc(sizeof(int));
	int * u = malloc(sizeof(int));
	*x = 10;
	*y = 9;
	*z = 12;
	*w = 11;
	*u = 8;
	set_insert_element(seta, x);
	set_insert_element(seta, y);
	set_insert_element(seta, u);
	set_insert_element(seta, w);
	set_insert_element(seta, z);

	// get the number of elements in a set
	int sizea = set_count(seta);
	// print a set (currently very ugly WIP)
	set_print(seta);

	// create another set
	set setb = set_create(int_comp, dint, print_int, sizeof(int));
	int * a = malloc(sizeof(int));
	int * b = malloc(sizeof(int));
	int * c = malloc(sizeof(int));
	int * d = malloc(sizeof(int));
	int * e = malloc(sizeof(int));
	*a = 10;
	*b = 29;
	*c = 5;
	*d = 21;
	*e = 2;
	set_insert_element(setb, a);
	set_insert_element(setb, b);
	set_insert_element(setb, c);
	set_insert_element(setb, d);
	set_insert_element(setb, e);

	int *iarray[10];
	for(int i = 0; i < 10; i++){
		int * temp = malloc(sizeof(int));
		*temp = i;
		iarray[i] = temp;
	}
	set list_set = set_create(int_comp, dint, print_int, sizeof(int));
	set_list_insert(list_set, iarray, 10);

	set union_set = set_union(seta, setb);
	set inter_set = set_intersection(seta, setb);
	printf("PRINTING inter_set: \n");
	set_print(inter_set);
	printf("PRINTING union_set: \n");
	set_print(union_set);
	printf("PRINTING list_set: \n");
	set_print(list_set);

	/*destroying the sets frees them and their elements from memory.
	 i.e x,y,y,w,z etc are freed after this 
	 */
	set_destroy(seta);
	set_destroy(setb);
	set_destroy(union_set);
	set_destroy(inter_set);
}