#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "rb_tree.h"
#include "string.h"
/*
This file includes code for red-black trees and stacks. 
*/

/*
NAME: safe_malloc
INPUTS: size to malloc
OUTPUT: pointer to memory if successful 
if mallloc fails it prints a statement and exits the program.
*/

void * safe_malloc(size_t size) {
    void * result;
    result = malloc(size);
    if (result != NULL) {
        return(result);
    } else {
        printf("malloc failed in safe_malloc.");
        printf("Terminating Program.\n");
        exit(1);
        return(0);
  }
}


/*  null_function just exists to be passed to rb_tree_create when no 
suitable function is neccassary */

void null_function(void * junk) { ; }


/*
NAME: stack_not_empty
INPUTS: stack
OUTPUT: top of stack if it exists and 0 if it does not
*/
int stack_not_empty(stk_stack * working_stack) {
  return( working_stack ? (int) working_stack->top : 0);
}


/*
NAME: stack_join
INPUTS: two stacks
OUTPUT: adjoins stack2 to stack1 and returns stack1
*/

stk_stack * stack_join(stk_stack * stack1, stk_stack * stack2) {
    if (!stack1->tail) {
        free(stack1);
        return(stack2);
    } else {
        stack1->tail->next = stack2->top;
        stack1->tail = stack2->tail;
        free(stack2);
    return(stack1);
    }
}


/*
NAME: stack_create
INPUTS: none
OUTPUT: a new stack
*/
stk_stack * stack_create() {
    stk_stack * new_stack;

    new_stack = (stk_stack *) safe_malloc(sizeof(stk_stack));
    new_stack->top = NULL;
    new_stack->tail = NULL;
    return(new_stack);
}


/*
NAMEL stack_push
INPUTS: stack and pointer to new element to push onto stack
OUTPUT: None
*/
void stack_push(stk_stack * working_stack, DATA_TYPE new_info_pointer) {
    stk_node * new_node;

    if(!working_stack->top) {
        new_node = (stk_node *) safe_malloc(sizeof(stk_node));
        new_node->info = new_info_pointer;
        new_node->next = working_stack->top;
        working_stack->top = new_node;
        working_stack->tail = new_node;
    } else {
        new_node = (stk_node *) safe_malloc(sizeof(stk_node));
        new_node->info = new_info_pointer;
        new_node->next = working_stack->top;
        working_stack->top = new_node;
    }
}


/* 
NAME: stack_pop
INPUTS: stack
OUTPUT: returns top of stack and removes that node from the stack
*/
DATA_TYPE stack_pop(stk_stack * working_stack) {
    DATA_TYPE pop_info;
    stk_node * old_node;

    if(working_stack->top) {
        pop_info = working_stack->top->info;
        old_node = working_stack->top;
        working_stack->top = working_stack->top->next;
        free(old_node);
    if (!working_stack->top) working_stack->tail = NULL;
    } else {
        pop_info = NULL;
    }
    return(pop_info);
}


/*
NAME: stack_destroy
INPUTS: stack and function to free/destroy information in each node
OUTPUT: None
*/
void stack_destroy(stk_stack * working_stack, void dest_func(void * a)) {
  stk_node * x = working_stack->top;
  stk_node * y;

  if(working_stack) {
    while(x) {
      y = x->next;
      dest_func(x->info);
      free(x);
      x = y;
    }
    free(working_stack);
  }
} 


/*
NAME: rb_tree_create
INPUTS: All of the inputs are functions. 
compare(a,b) should return 1 if *a > *b, -1 if *a < *b, and 0 otherwise 
destroy(a) takes a pointer and frees it accordingly 
the print functions are optional, if you choose not to use them use a 
null function can be used instead. Info and Key size are sizes in bytes.
OUTPUT: A pointer to the new red black tree
*/

rb_tree* rb_tree_create( int (*comp_func) (const void*, const void*),
                    void (*dest_func) (void*),
                    void (*info_dest_func) (void*),
                    void (*print_func) (const void*),
                    void (*print_info)(void*),
                    size_t key_size,
                    size_t info_size) {
    rb_tree* new_tree;
    rb_node* temp;

    new_tree = (rb_tree*) safe_malloc(sizeof(rb_tree));
    new_tree->compare = comp_func;
    new_tree->destroy_key = dest_func;
    new_tree->print_key = print_func;
    new_tree->print_info = print_info;
    new_tree->destroy_info = info_dest_func;
    new_tree->key_size=key_size;
    new_tree->info_size=info_size;
    /*  see the comment in the rb_tree structure in rb_tree.h */
    /*  for information on nil and root */
    new_tree->nil = temp = (rb_node*) safe_malloc(sizeof(rb_node));
    // temp;
    temp->parent=temp->left=temp->right=temp;
    /*
    temp->parent = temp;
    temp->left = temp;
    temp->right = temp; */
    temp->red = 0;
    temp->key = 0;
    temp = (rb_node*) safe_malloc(sizeof(rb_node));
    new_tree->root = temp;
    temp->parent = new_tree->nil;
    temp->left = new_tree->nil;
    temp->right = new_tree->nil;
    temp->key = 0;
    temp->red = 0;
    return(new_tree); 
}


/*
NAME: left_rotate
INPUTS: Tree and node to rotate around 
OUTPUT: None, modifies the input tree 
Rotates as described in Intro to Algorithms. 
            left rotate (T, x)
    x       -->             y
  /   \                   /   \ 
 a     y                 x     c
      / \               / \
     b   c             a   b
            <--
            right rotate (T,y)  
Where a,b and c are arbitrary subtrees. 
*/

void left_rotate(rb_tree* tree, rb_node* x) {
    rb_node* y;
    rb_node* nil = tree->nil;

    y = x->right;
    x->right = y->left;

    if (y->left !=  nil) {
    y->left->parent = x;
    }
  
    y->parent = x->parent;   

    if( x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}


/*
NAME: left_rotate
INPUTS: Tree and node to rotate around 
OUTPUT: None, modifies the input tree 
Rotates as described in Intro to Algorithms. 
See diagram in comments of right_rotate 
*/

void right_rotate(rb_tree* tree, rb_node* y) {
    rb_node* x;
    rb_node* nil = tree->nil;

    x = y->left;
    y->left = x->right;

    if (nil !=  x->right){
        x->right->parent = y;
    } 

    x->parent = y->parent;
    if( y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

/*
NAME: tree_insert_help
INPUTS: tree to insert into and node z to inser
OUPUT: none
modifies both tree and z. This function inserts z into the free as though
it were a standard binary search tree. This is a helper function which should
only be called by rb_tree_insert, afterwards the tree must be fixed to ensure
it still satisfied the red-black properties. 
*/

void tree_insert_help(rb_tree* tree, rb_node* z) {
    /*  This function should only be called by InsertRBTree (see above) */
    rb_node* x;
    rb_node* y;
    rb_node* nil = tree->nil;

    z->left = z->right = nil;
    y = tree->root;
    x = tree->root->left;
    while( x !=  nil) {
        y = x;
        if (1 == tree->compare(x->key, z->key)) { /* x.key > z.key */
            x = x->left;
        } else { /* x,key < =  z.key */
            x = x->right;
        }
    }
    z->parent = y;
    if ( (y == tree->root) || (1 == tree->compare(y->key, z->key))) { /* y.key > z.key */
        y->left = z;
    } else {
        y->right = z;
    } 
}


/*
NAME: rb_tree_insert
INPUTS: tree is the tree to insert a new node which will have key pointed to
by key and info pointed to by info.
OUTPUT: Returns a pointer to the newly inserted node. This pointer will be 
valid until this node is deleted. Additionally, the tree is modifed and a 
new node is inserted into it. 
*/

rb_node * rb_tree_insert(rb_tree* tree, void* key, void* info) {
    rb_node * y;
    rb_node * x;
    rb_node * new_node;
    x = (rb_node*) safe_malloc(sizeof(rb_node));
    x->key = key;
    x->info = info;
    tree_insert_help(tree, x);
    new_node = x;
    x->red = 1;
    while(x->parent->red) { /* use sentinel instead of checking for root */
        if (x->parent == x->parent->parent->left) {
            y = x->parent->parent->right;
            if (y->red) {
                x->parent->red = 0;
                y->red = 0;
                x->parent->parent->red = 1;
                x = x->parent->parent;
            } else {
                if (x == x->parent->right) {
                    x = x->parent;
                    left_rotate(tree, x);
                }
            x->parent->red = 0;
            x->parent->parent->red = 1;
            right_rotate(tree, x->parent->parent);
            } 
        } else { /* case for x->parent == x->parent->parent->right */
            y = x->parent->parent->left;
            if (y->red) {
                x->parent->red = 0;
                y->red = 0;
                x->parent->parent->red = 1;
                x = x->parent->parent;
            } else {
                if (x == x->parent->left) {
                    x = x->parent;
                    right_rotate(tree, x);
                }
                x->parent->red = 0;
                x->parent->parent->red = 1;
                left_rotate(tree, x->parent->parent);
            } 
        }
    }
    tree->root->left->red = 0;
    return(new_node);
}


/*
NAME: tree_successor
INPUTS: tree and node x in the tree which we want the successor of
OUTPUT: the successor of x or NULL if no successor exists. 
*/

rb_node* tree_successor(rb_tree* tree, rb_node* x) { 
    rb_node* y;
    rb_node* nil = tree->nil;
    rb_node* root = tree->root;
    y = x->right;
    if (nil != y) {
        while(y->left != nil) { /* returns the minium of the right subtree of x */
            y = y->left;
        }
        return(y);
    } else {
        y = x->parent;
        while(x == y->right) { /* sentinel used instead of checking for nil */
            x = y;
            y = y->parent;
        }
        if (y == root) {
            return(nil);
        }
        return(y);
    }
}


/*
NAME: tree_predecessor
INPUTS: tree and node x which we wan the predecessor of
OUTPUT: returns the predecessor of x or NULL if no predecessor exists. 
*/

rb_node* tree_predecessor(rb_tree* tree, rb_node* x) {
    rb_node* y;
    rb_node* nil = tree->nil;
    rb_node* root = tree->root;
    y = x->left;
    if (nil != y) { /* assignment to y is intentional */
    while(y->right != nil) { /* returns the maximum of the left subtree of x */
        y = y->right;
    }
    return(y);
    } else {
        y = x->parent;
        while(x == y->left) { 
            if (y == root){
                return(nil); 
            } 
            x = y;
            y = y->parent;
        }
        return(y);
    }
}


/*
NAME: inorder_tree_print
INPUTS: tree to print and node x which is the current inorder node
OUTPUT: none
prints the tree inorder using the user supplied print_ket and print_info.
This function is not intended to be used by the user. It is called from
rb_tree_print
*/

void inorder_tree_print(rb_tree* tree, rb_node* x) {
    rb_node* nil = tree->nil;
    rb_node* root = tree->root;
    if (x !=  tree->nil) {
        inorder_tree_print(tree,x->left);
        printf("info = ");
        tree->print_info(x->info);
        printf("  key = "); 
        tree->print_key(x->key);
        printf("  l->key = ");
        if( x->left == nil){
            printf("NULL"); 
        } 
        else{
            tree->print_key(x->left->key);
        } 
        printf("  r->key = ");
        if( x->right == nil){
            printf("NULL"); 
        } 
        else{
            tree->print_key(x->right->key); 
        } 
        printf("  p->key = ");
        if( x->parent == root){
            printf("NULL"); 
        }
        else{
            tree->print_key(x->parent->key);   
        } 
        printf("  red = %i\n",x->red);
        inorder_tree_print(tree,x->right);
  }
}


/*
NAME: tree_dest_helper
INPUTS: tree to destroy and x which is the current node
OUTPUT: None
Recursively destroys the nodes using user supplied destroy_key and destroy_info
Should not be used by user. It is a helper function fro rb_tree_destroy
*/
void tree_dest_helper(rb_tree* tree, rb_node* x) {
    rb_node* nil = tree->nil;
    // I think this comparison is not working 
    if (x !=  nil) {
        tree_dest_helper(tree, x->left);
        tree_dest_helper(tree, x->right);
        tree->destroy_key(x->key);
        tree->destroy_info(x->info);
        free(x);
    }
}


/*
NAME: rb_tree_destroy
INPUTS: tree to destroy 
OUTPUT: none
destroys tree and frees it from memory. 
*/

void rb_tree_destroy(rb_tree* tree) {
    tree_dest_helper(tree, tree->root->left);
    free(tree->root);
    free(tree->nil);
    free(tree);
}


/*
NAME: rb_tree_print
INPUTS: tree to print
OUTPUT: none
prints the nodes of the tree using the user supplied print_key and print_info.
*/

void rb_tree_print(rb_tree* tree) {
  inorder_tree_print(tree, tree->root->left);
}

 
/*
NAME: rb_lookup
INPUTS: tree to look in, and a pointer to the key we are looking for
OUTPUT: returns the node with key equal to q. If there are multiple it
returns the one highest in the tree. returns 0 if not found. 
*/

rb_node* rb_lookup(rb_tree* tree, void* q) {
    rb_node* x = tree->root->left;
    rb_node* nil = tree->nil;
    int comp_val;
    if (x == nil){
        return(0);
    } 
    comp_val = tree->compare(x->key,(int*) q);
    while(0 !=  comp_val) {
        if (1 == comp_val) { /* x->key > q */
            x = x->left;
        } else {
            x = x->right;
        }
        if ( x == nil){
            return(0);
        } 
        comp_val = tree->compare(x->key,(int*) q);
    }
    return(x);
}


/*
NAME: rb_delete_fix_up
INPUTS: tree to be fixed and x which is the child of the deleted node. 
OUTPUT: None
restored red-black properties after node is deleted from the tree.
modifies the tree and x. 
*/

void rb_delete_fix_up(rb_tree* tree, rb_node* x) {
    rb_node* root = tree->root->left;
    rb_node* w;

    while((!x->red) && (root !=  x)) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->red) {
                w->red = 0;
                x->parent->red = 1;
                left_rotate(tree, x->parent);
                w = x->parent->right;
            }
            if ((!w->right->red) && (!w->left->red)) { 
                w->red = 1;
                x = x->parent;
            } else {
                if (!w->right->red) {
                    w->left->red = 0;
                    w->red = 1;
                    right_rotate(tree, w);
                    w = x->parent->right;
                }
                w->red = x->parent->red;
                x->parent->red = 0;
                w->right->red = 0;
                left_rotate(tree, x->parent);
                x = root; /* this is to exit while loop */
            }
        } else { /* the code below is has left and right switched from above */
            w = x->parent->left;
            if (w->red) {
                w->red = 0;
                x->parent->red = 1;
                right_rotate(tree,x->parent);
                w = x->parent->left;
            }
            if ((!w->right->red) && (!w->left->red)) { 
                w->red = 1;
                x = x->parent;
            } else {
                if (!w->left->red) {
                    w->right->red = 0;
                    w->red = 1;
                    left_rotate(tree, w);
                    w = x->parent->left;
                }
                w->red = x->parent->red;
                x->parent->red = 0;
                w->left->red = 0;
                right_rotate(tree, x->parent);
                x = root; /* this is to exit while loop */
            }
        }
    }
    x->red = 0;
}


/*
NAME: rb_delete
INPUTS: tree and node z to delete from tree
OUTPUT: None
deletes z from tree and frees key/info from z using the user supplied 
destory_key and destroy_info. Calls rb_delete_fix_up to fix red-black 
properties
*/

void rb_delete(rb_tree* tree, rb_node* z){
    rb_node* y;
    rb_node* x;
    rb_node* nil = tree->nil;
    rb_node* root = tree->root;

    y =  ((z->left == nil) || (z->right == nil)) ? z : tree_successor(tree,z);
    x =  (y->left == nil) ? y->right : y->left;
    x->parent = y->parent;
    if (root == x->parent) {
        root->left = x;
    } else {
        if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
    }
    if (y !=  z) { /* y should not be nil in this case */
    /* y is the node to splice out and x is its child */
    if (!(y->red)){
        rb_delete_fix_up(tree,x);   
    } 

    tree->destroy_key(z->key);
    tree->destroy_info(z->info);
    y->left = z->left;
    y->right = z->right;
    y->parent = z->parent;
    y->red = z->red;
    z->left->parent = z->right->parent = y;
    if (z == z->parent->left) {
        z->parent->left = y; 
    } else {
        z->parent->right = y;
    }
    free(z); 
    } else {
        tree->destroy_key(y->key);
        tree->destroy_info(y->info);
        if (!(y->red)){
            rb_delete_fix_up(tree,x);
        }
        free(y);
    }
}

/*
NAME: rb_enumerate
INPUTS: tree to lookup in, keys for high (<=) and low (>=) with respect
to user supplied compare function.
OUTPUT: stack with pointers to nodes in interval [low, high]
*/
stk_stack* rb_enumerate(rb_tree* tree, void* low, void* high) {
    stk_stack* enum_result_stack;
    rb_node* nil = tree->nil;
    rb_node* x = tree->root->left;
    rb_node* lastBest = nil;

    enum_result_stack = stack_create();
    while(nil !=  x) {
        if ( 1 == (tree->compare(x->key,high)) ) { /* x->key > high */
            x = x->left;
    } else {
    lastBest = x;
            x = x->right;
        }
    }
    while ( (lastBest !=  nil) && (1 !=  tree->compare(low,lastBest->key))) {
        stack_push(enum_result_stack,lastBest);
        lastBest = tree_predecessor(tree,lastBest);
    }
    return(enum_result_stack);
}

/*
NAME: rb_count
INPUTS: tree and node to start counting from
OUTPUT: size of subtree starting from and including x
*/
int rb_count(rb_tree* tree, rb_node* x){
    rb_node* nil = tree->nil;
    int c = 1;
    if(x == nil){
        return 0;
    } else {
        c += rb_count(tree, x->left);
        c += rb_count(tree, x->right);
        return c;
    }
}

/*
NAME: rb_flatten_helper
INPUTS: tree to flatten, node which is the root of the subtree we will flatten
and array to store elements and index 
OUPUT: None, but arr is modified. 
Should only be called from rb_flatten
*/

void rb_flatten_helper(rb_tree* tree, rb_node* node, rb_node* arr[], int *index_ptr){
    rb_node * nil = tree->nil;
    if (node == nil)
    return;
 
    /* first recur on left child */
    rb_flatten_helper(tree, node->left, arr, index_ptr);
    arr[*index_ptr] = node;
    (*index_ptr)++;  // increase index for next entry
    /* now recur on right child */
    rb_flatten_helper(tree, node->right, arr, index_ptr);
}

/*
NAME: rb_flatten
INPUTS: tree to flatten, node which is the root of the subtree we will flatten
and array to store elements
OUPUT: None, but arr is modified. 
*/

void rb_flatten (rb_tree* tree, rb_node* node, rb_node * arr[]){
    int i = 0;
    rb_flatten_helper(tree, node, arr, &i);
    int j = 0;
/*    while(j < i){
        //rb_node* temp = arr[j];
        rb_node * temp = arr[j];
        int x = *(int*)temp->key;
        printf("array print: %d \n", x);
        j++;
    } */
}


//need new nodes, I am modifying the old ones and super fucking them up in the old trees. 
// introduces problem of keys and info, should I create new keys and info in new memory when I merge two trees? probably. 
rb_node* rb_array_to_tree_helper(rb_tree* tree, rb_node* curr_node, rb_node* array[], int start, int end, int colour){
    if (start > end){
        return tree->nil;
    }
    int mid = (start + end)/2;
    rb_node* middle = array[mid];
    middle->red = colour;
    printf("PRINTING MIDDLE\n");
    tree->print_key(middle->key);
    middle->parent = curr_node;
    middle-> left = tree->nil;
    middle-> right = tree->nil;
    middle->left = rb_array_to_tree_helper(tree, middle, array, start, mid-1, (colour+1) %2);
    middle->right = rb_array_to_tree_helper(tree, middle, array, mid+1, end, (colour+1) %2);
    if(middle != tree->nil){
        return middle;
    }   
}


/*
NAME: rb_sorted_array_to_rb_tree
INPUTS: tree to insert elements of array into, array of elemnts, the start 
and end indices of the elements in the array which we want to insert. 
OUTPUT: the next node for recursive purposes, 
This function will modify tree the contain all the elements of the array.
The idea is to pass a empty tree which contains the functions accociated with
this type of tree rather than pass the functions and construct a new tree. 
*/
// this does not work, probably need to do something with the coloring and add a iterations thingo, even = black odd = red or something 
void rb_sorted_array_to_rb_tree(rb_tree* tree, rb_node* array[], int start, int end){
    rb_node* root = rb_array_to_tree_helper(tree, tree->root, array, start, end, 0);
    tree->root->left = root;
}

/*
NAME: rb_clone_nodes
INPUTS: Tree of type which nodes come from, old_array of nodes to be copied
new_array to copy into and size which is the number of nodes.
OUPUT: None
This function will allocate new memoery and create new nodes which are clones
of the nodes in old_array.
*/

void rb_clone_nodes(rb_tree* tree, rb_node* old_array[], rb_node* new_array[], int size){
    rb_node* old_temp;
    rb_node* new_temp;
    void * temp_key;
    void * temp_info;
    for(int u = 0; u < size; u++){
        old_temp = old_array[u];
        temp_key = safe_malloc(tree->key_size);
        // if the user does not have info they should pass 0 for info size. 
        if(tree->info_size){
            temp_info = safe_malloc(tree->info_size);  
        }
        new_temp = safe_malloc(sizeof(rb_node));
        new_temp->key = memcpy(temp_key, (old_temp->key), tree->key_size);
        new_temp->info = memcpy(temp_info, (old_temp->info), tree->info_size);;
        new_array[u] = new_temp;
    }
}

/*
NAME: rb_merge
INPUTS: tree1 and tree2 to merge 
OUPUT: new tree or NULL if compare functions are different, or if key/info 
sizes differ.
both trees MUST use the same compare function. Since it is impossible to 
determine whether two programs compute the same function, the comparison will
be done on the pointers to the functions. 
*/

rb_tree* rb_merge(rb_tree* tree1, rb_tree* tree2){
    if(tree1->compare != tree2->compare || tree1->key_size != tree2->key_size
        || tree1->info_size != tree2->info_size){
        return NULL;
    }
    int size1 = rb_count(tree1, tree1->root->left);
    int size2 = rb_count(tree2, tree2->root->left);
    int new_size = size1 + size2;
    rb_node* arr1[size1];
    rb_node* arr2[size2];
    rb_node* arr_merge[new_size];

    rb_flatten(tree1, tree1->root->left, arr1);
    rb_flatten(tree2, tree2->root->left, arr2);

    int i = 0, j = 0, k = 0;
    // merge the two flattened trees  
    while (i < size1 && j < size2){
            // put the smallest element in arr_merge
            if (tree1->compare(arr1[i]->key, arr2[j]->key) == (-1)){
                arr_merge[k] = arr1[i];
                i++;
            } else {
                arr_merge[k] = arr2[j];
                j++;
            }
            k++;
        }
    // If there are more elements in first array
    while (i < size1){   
        arr_merge[k] = arr1[i];
        i++; k++;
    }
    // If there are more elements in second array
    while (j < size2){
        arr_merge[k] = arr2[j];
        j++; k++;
    }

    /*Each element in arr_merge is a pointer to an element in another tree. 
    In order to not mess those elements up we need to assign new memory
    for new elements with the same keys/info */
    rb_node* new_merge_arr[new_size];
    rb_clone_nodes(tree1, arr_merge, new_merge_arr, new_size);

/*
    int ab = 0;
    while (ab < k){
        //rb_node* temp = arr[j];
        rb_node * temp = new_merge_arr[ab];
        //tree1->print_key(temp->key);
        //int x = *(int*)temp->key;   
        printf("array print key ptr: %p \n", temp->key);
        ab++;
    } */

    rb_tree* out_tree = rb_tree_create(tree1->compare,
                                    tree1->destroy_key,
                                    tree1->destroy_info,
                                    tree1->print_key,
                                    tree1->print_info,
                                    tree1->key_size,
                                    tree1->info_size);
    rb_sorted_array_to_rb_tree(out_tree, new_merge_arr, 0, (new_size-1));
    return out_tree;
}
