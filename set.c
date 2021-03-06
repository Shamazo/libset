#include <stdio.h>   /* gets */
#include <stdlib.h>  /* atoi, malloc */
#include <string.h>  /* strcpy */
#include "set.h"

/*
NAME: set_create
INPUTS: comp_func, a function which takes in two elements (a,b ) and returns 
1 if a >. b, -1 if a < b and 0 if a == b. If your elements do not have 
a meaningful order this function can arbitrarily return -1 or 1 for a given
pair of inputs as long as it returns the same thing on the same inputs and 
returns 0 if they are the same. dest_func should free the input key. 
print_func is only required if you intend to use print_set. Otherwise
you can pass null_function. 
*/

set set_create(int (*comp_func)(const void*, const void*),
			    void (*dest_func)(void*),  
			    void (*print_func)(const void*),
                size_t key_size) {
	set new_set = rb_tree_create(comp_func, dest_func, null_function,
                                 print_func, null_function, key_size, 0);
	return(new_set);
}


/*
NMAEL set_print
INPUTS: set to print
OUPUT: None
prints the set using the user supplied print functions
*/
void set_print(set seta){
    void* nullp = NULL;
    printf("{");
    set_iter(seta, seta->print_key, nullp);
    printf("} \n");
}

/*
NAME: set_insert_element
INPUTS: Set to insert into and key to insert
OUPUTS: The element if sucessfully inserted and NULL if it is already
a member of the set
This is very similar to rb_merge, but it avoids duplicates. 
*/
element set_insert_element(set seta, void * key){
	if (rb_lookup(seta, key)){
        return(NULL);
    } else {
        void * info;
        return(rb_tree_insert(seta, key, info));
    }
}

/*
NAME: set_list_insert
INPUTS: set to insert into, array of pointers to keys and length of array
OUTPUT: 0 if every element inserted is not in the set
1 if one or more of the elements is already in the set, but still successfully
inserts all elements/
*/

int set_list_insert(set seta, void* array[], size_t length){
    element null_check;
    for (size_t i = 0; i < length; i++){
        null_check = set_insert_element(seta, array[i]);
    }
    if (null_check){
        return 1;
    } else {
        return 0;
    }
}


/*
NAME: delete_element
INPUTS: set to delete from and a pointer to the key value to delete
OUTPUT: None
*/
void set_delete_element(set seta, void * key){
    element temp = rb_lookup(seta, key);
    rb_delete(seta, temp);
}


/*
NAME: lookup_element
INPUTS: set to look in and pointer to key value to look for
OUTPUT: returns 1 if the key is in the set and 0 otherwise. 
*/
int set_is_member(set seta, void *key){
    element temp = rb_lookup(seta, key);
    if(temp){
        return 1;
    } else {
        return 0;
    }
}

/*
NAME: set_destroy
INPUTS: tree to destroy 
OUTPUT: none
destroys tree and frees it from memory. 
*/

void set_destroy(set seta){
	rb_tree_destroy(seta);
}

/*
NAME: set_count
INPUTS: set
OUPUT: size of set as an int
*/
int set_count(set seta){
    rb_count(seta, seta->root->left);
}


/*
NAME: set_union
INPUTS: two sets to take the union of
OUTPUT: new set containing every element in one or both of the sets.
Does not duplicate elements if they are in both sets. The new set is memory 
independent of the inputs. i.e you can destroy the input sets without effecting
the output set. 
*/

set set_union(set set1, set set2){
    if(set1->compare != set2->compare || set1->key_size != set2->key_size
        || set1->info_size != set2->info_size){
        return NULL;
    }
    int size1 = rb_count(set1, set1->root->left);
    int size2 = rb_count(set2, set2->root->left);
    rb_node* arr1[size1];
    rb_node* arr2[size2];
    int new_size = size1 + size2;
    rb_node* arr_merge[new_size];
    rb_flatten(set1, set1->root->left, arr1);
    rb_flatten(set2, set2->root->left, arr2);

    int i = 0, j = 0, k = 0;
    // merge the two flattened trees  avoiding duplicates 
    while (i < size1 && j < size2){
            // put the smallest element in arr_merge
            if (set1->compare(arr1[i]->key, arr2[j]->key) == (-1)){
                arr_merge[k] = arr1[i];
                i++;
            } else if(set1->compare(arr1[i]->key, arr2[j]->key) == (0))  {
                arr_merge[k] = arr2[j];
                j++;
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
    rb_node* new_merge_arr[k];
    rb_clone_nodes(set1, arr_merge, new_merge_arr, k);

    set out_set = rb_tree_create(set1->compare,
                                    set1->destroy_key,
                                    set1->destroy_info,
                                    set1->print_key,
                                    set1->print_info,
                                    set1->key_size,
                                    set1->info_size);
    rb_sorted_array_to_rb_tree(out_set, new_merge_arr, 0, (k-1));
    return out_set;
}


/*
NAME: set_intersection
INPUTS: two sets to take the intersection of.
OUTPUT: new set containing every element which is contained in both sets.
The new set is memory independent of the inputs. i.e you can destroy the 
input sets without affecting the output set. 
*/

set set_intersection(set set1, set set2){
    if(set1->compare != set2->compare || set1->key_size != set2->key_size
            || set1->info_size != set2->info_size){
        return NULL;
    }
    int size1 = rb_count(set1, set1->root->left);
    int size2 = rb_count(set2, set2->root->left);
    rb_node* arr1[size1];
    rb_node* arr2[size2];
    int new_size = size1 + size2;
    rb_node* arr_merge[new_size];
    rb_flatten(set1, set1->root->left, arr1);
    rb_flatten(set2, set2->root->left, arr2);

    int i = 0, j = 0, k = 0;
    // merge the two flattened trees only adding elements in common
    while (i < size1 && j < size2){
            // put the smallest element in arr_merge
            if (set1->compare(arr1[i]->key, arr2[j]->key) == 0){
                arr_merge[k] = arr1[i];
                i++;
                j++;
                k++;
            } else if(set1->compare(arr1[i]->key, arr2[j]->key) == (1)){
                j++;
            } else {
                i++;
            } 
        }
    /*Each element in arr_merge is a pointer to an element in another tree. 
    In order to not mess those elements up we need to assign new memory
    for new elements with the same keys/info */
    rb_node* new_merge_arr[k];
    rb_clone_nodes(set1, arr_merge, new_merge_arr, k);
    set out_set = rb_tree_create(set1->compare,
                                    set1->destroy_key,
                                    set1->destroy_info,
                                    set1->print_key,
                                    set1->print_info,
                                    set1->key_size,
                                    set1->info_size);
    rb_sorted_array_to_rb_tree(out_set, new_merge_arr, 0, (k-1));
    return out_set;
}

/*
NAME: set_iter_helper
INPUTS: set being iterated over, function being applied and current element
OUTPUT: None
This is a recursive helper function called by set_iter. 
*/

void set_iter_helper(set seta, void (*iter_func)(void* key, void* data), element curr_element, void* data){
    element nil = seta->nil;
    if (curr_element != nil){
        iter_func(curr_element->key, data);
        set_iter_helper(seta, iter_func, curr_element->right, data);
        set_iter_helper(seta, iter_func, curr_element->left, data);
    }
}


/*
NAME: set_iter
INPUTS: set to iterate over, function which takes in the keys in that set 
and a pointer to data and then finally the data pointer. The idea with
the data pointer is to allow the user to keep track of some information
while iterating over the set. See example of sum of a set in example.c
OUTPUT: None
Applyes the input function to every element in the set 
*/

void set_iter(set seta, void (*iter_func)(void* key, void* data), void* data){
    element root = seta->root->left;
    set_iter_helper(seta, iter_func, root, data);
} 
