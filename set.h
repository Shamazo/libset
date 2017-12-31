#include "rb_tree.h"
/*
Hamish Nicholson. 
*/

/* Behind the scenes sets are red-black trees*/
typedef struct rb_tree *set; 
typedef struct rb_node *element;

/* Set functions  */
set set_create(int (*comp_func)(const void*, const void*),
    void (*dest_func)(void*),
    void (*print_func)(const void*),
    size_t key_size);
element set_insert_element(set seta, void * key); 
int set_is_member(set seta, void * key);
void set_delete_element(set seta, void * key);
void set_print(set seta); 
void set_destroy(set seta);
int set_count(set seta);
set set_union(set set1, set set2);
set set_intersection(set set1, set set2);
