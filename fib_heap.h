#ifndef FIB_HEAP_H
#define FIB_HEAP_H

#include <stdbool.h>
#include <stdlib.h>
#include <string>

#ifdef	__cplusplus
extern "C" {
#endif
    
/*
 *   Structure of each node in the fibonacci heap
 */    
struct heap_node
{
    int element;
    std::string str;
    heap_node* left;
    heap_node* right;
    int degree;
    heap_node* parent;
    heap_node* child;
    bool marked;
};

/*
 *   Structure that points to head (maximum node) of the fibonacci heap
 */ 
struct fib_heap {
    heap_node*     max;
    heap_node**    node_array;
};

/*
 *   Fibonacci class that containing all the fibonacci 
 *   operations and pointers to the heap
 */ 
class fib_class
{
    public:
        fib_heap * heap;
        int count = 0;
        fib_class();
        ~fib_class();
        heap_node * insert (fib_heap * H, int value, std::string str);
        std::string remove_max(fib_heap * H);
        void increase_key(fib_heap* heap, heap_node * x, int element);
        void cut(fib_heap* heap, heap_node* x, heap_node* y);
        void cascading_cut(fib_heap* heap, heap_node* y);
        void consolidate(fib_heap * H);
        void combine(heap_node * y, heap_node * x);
        void printHeap (fib_heap * H);
};


#ifdef	__cplusplus
}
#endif

#endif	/* fib_heap_H */
