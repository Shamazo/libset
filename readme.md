# Libset
## A basic set library written in c.
This is a implementation of sets using red-black trees. Please see example.c for usage examples and set.c for comments on function inputs and usage. To compile the example program with the supplied makefile run "make lib" in a shell. 
The complexity of various operations is described in the following table.

| Operation | Complexity|
| ----------|-----------|
|Insertion  | O(Log(n))  |
|Deletion 	|  O(Log(n))  |
|set intersection| O(n+m) |
|set union| O(n+m)  |
|set count | O(n)|   |
|set is member| O(log(n))  |

To do list:
insert via array of pointers
implement a function which applies a given function to every element in the set
and generates a new set. Similar to list maps in some functional langauges.
Set generating functions. Perhaps also implment an iter function.
