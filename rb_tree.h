/*
Hamish Nicholson. Based on the descriptions of red black trees in 
Introduction to Algorithms by Cormen, Leiserson, Rivest, and stein. 
*/

/*Data type for stacks, defaults to void * if not defined elsewhere*/
#ifndef DATA_TYPE
#define DATA_TYPE void *
#endif
#include <stdlib.h>
typedef struct stk_node {
    DATA_TYPE info;
    struct stk_node * next;
} stk_node;

typedef struct stk_stack { 
    stk_node * top;
    stk_node * tail;
} stk_stack ;

typedef struct rb_node {
    void* key;
    void* info;
    int red; /* red=0 -> node is black */
    struct rb_node* left;
    struct rb_node* right;
    struct rb_node* parent;
} rb_node;

typedef struct rb_tree {
    int (*compare)(const void* a, const void* b); 
    void (*destroy_key)(void* a);
    void (*destroy_info)(void* a);
    void (*print_key)(const void* a);
    void (*print_info)(void* a);
    size_t key_size;
    size_t info_size;
    /*  A place holder is used for root and for nil.  These placeholders are 
    created when rb_tree_create is called. root->left should always 
    point to the node which is the root of the tree.  nil points to a 
    node which should always be black but has aribtrary children and 
    parent and no key or info.  The point of using these sentinels is so 
    that the root and nil nodes do not require special cases in the code */
    rb_node* root;             
    rb_node* nil;              
} rb_tree;

/* stack functions, more detailed comments in rb_tree.c */
stk_stack * stack_join(stk_stack * stack1, stk_stack * stack2);
stk_stack * stack_create();
void stack_push(stk_stack * theStack, DATA_TYPE newInfoPointer);
void * stack_pop(stk_stack * theStack);
int stack_not_empty(stk_stack *);

/*rb tree functions  */
rb_tree* rb_tree_create(int(*comp_func)(const void*, const void*),
    void (*dest_func)(void*), 
    void (*info_dest_func)(void*), 
    void (*print_func)(const void*),
    void (*print_info)(void*),
    size_t key_size,
    size_t infot_size);
rb_node * rb_tree_insert(rb_tree*, void* key, void* info);
void rb_tree_print(rb_tree*);
void rb_delete(rb_tree*, rb_node*);
void rb_tree_destroy(rb_tree*);
rb_node* tree_predecessor(rb_tree*, rb_node*);
rb_node* tree_successor(rb_tree*, rb_node*);
rb_node* rb_lookup(rb_tree*, void*);
stk_stack * rb_numerate(rb_tree* tree, void* low, void* high);
void null_function(void*);
int rb_count(rb_tree*, rb_node*);
void rb_flatten(rb_tree* tree, rb_node* node, rb_node * arr[]);
void rb_clone_nodes(rb_tree* tree, rb_node* array1[], rb_node* array2[], int size);
rb_tree* rb_merge(rb_tree* tree1, rb_tree* tree2);
void rb_sorted_array_to_rb_tree(rb_tree* tree, rb_node* array[], int start, int end);