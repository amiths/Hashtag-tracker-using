#include "fib_heap.h"
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <cmath>

using namespace std;

/*
 *   Constructor to initialize heap
 */
fib_class::fib_class()
{
	this->heap = new(fib_heap);
	this->heap->max = NULL;
}

/*
 *   Destructor to free the heap
 */
fib_class::~fib_class()
{
    delete this->heap;
}

/*
 *   Function to print the root nodes of the heap
 */
void fib_class::printHeap(fib_heap * H)
{
    heap_node * p = H->max;
    if (p == NULL)
        cout <<"Empty heap"<< endl;
    else
    {
        do
        {
        cout<< p->element;
        p = p->right;
        if (p != H->max)
        {
            cout<<"-->";
        }
        }
        while (p != H->max && p->right != NULL);
    cout<<endl;
    }
}

/*
 *   Insert node into heap and return the address of the node
 */
heap_node* fib_class::insert(fib_heap* heap, int element, string str) 
{
    heap_node* node = new (heap_node);
    node->element = element;
    node->str = str;
    node->right    = node;
    node->left     = node;
    node->child    = NULL;
    node->degree   = 0U;
    node->parent   = NULL;
    node->marked   = false;

    //Add node to the right of the maximum node
    if (heap->max)
    {
        node->left = heap->max;
        node->right = heap->max->right;
        heap->max->right = node;
        node->right->left = node;

        // If new node is greater than max, make this the max node
        if (element > heap->max->element) 
        {
            heap->max = node;
        }
    } 
    else 
    {
        heap->max = node;
    }
    count++;
    return node;
}

/*
 *   Add the new frequency to the old one
 */
void fib_class::increase_key(fib_heap* heap, heap_node * node, int element)
{
    heap_node* y;
    //Add frequencies
    node->element = node->element + element;
    y = node->parent;

    // If new frequency is greater than parent's, cut
    if (y && (node->element > y->element))
    {
        cut(heap, node, y);
        cascading_cut(heap, y);
    }

    if (node->element > heap->max->element)
    {
        heap->max = node;
    }

    return;
}

/*
 *   Make y node the child of x. Used by consolidation function
 */
void fib_class::combine(heap_node* y, heap_node* x)
{
    y->left->right = y->right;
    
    y->right->left = y->left;
    y->parent = x;

    if (!x->child)
    {
        x->child = y;
        y->right = y;
        y->left = y;
    }
    else
    {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right = y;
        y->right->left = y;
    }

    x->degree++;
    y->marked = false;
}

/*
 *   Remove the maximum node in the heap
 */
std::string fib_class::remove_max(fib_heap* heap)
{
    // Temporary variables
    int p_ret;
    size_t number_of_children;
    heap_node* max_ptr;
    heap_node* node_a;
    heap_node* tmp_right;
    heap_node* free_node;
    max_ptr = heap->max;

    number_of_children = max_ptr->degree;
    node_a = max_ptr->child;
    // Shift all the children of max to the root linked list
    while (number_of_children > 0) 
    {
        tmp_right = node_a->right;

        node_a->left->right = node_a->right;
        node_a->right->left = node_a->left;

        node_a->left = heap->max;
        node_a->right = heap->max->right;
        heap->max->right = node_a;
        node_a->right->left = node_a;

        node_a->parent = NULL;
        node_a = tmp_right;
        --number_of_children;
    }

    max_ptr->left->right = max_ptr->right;
    max_ptr->right->left = max_ptr->left;

    // Return the hashtag and frequency of the maximum node
    p_ret = heap->max->element;
    std::string str = heap->max->str + "," + to_string(heap->max->element);

    //Store max node in free node
    if (max_ptr == max_ptr->right)
    {
        free_node = heap->max;
        heap->max = NULL;
    }
    else 
    {
        free_node = heap->max;
        heap->max = max_ptr->right;
        // Combine the root nodes after remove max such that no two nodes have same degree
        consolidate(heap);
    }
    count--;
    delete(free_node);
    return str;
}

/*
 *   Consolidate : Combine the root nodes of same degree such that no two roots have same degree
 */
void fib_class::consolidate(fib_heap* heap)
{
    //Maximum degree in fib_heap is < log(total nodes)/log(magic ratio)
    int array_size = (int) (ceil(log(count))/log(1.618)) + 1;
    //Array to store the first nodes corresponding to each degree
	heap->node_array = new heap_node * [array_size];
    memset (heap->node_array, 0, array_size * sizeof(heap_node *));

    heap_node* node_a;
    heap_node* node_b;
    heap_node* tmp;
    heap_node* next;
    size_t     number_of_roots;
    size_t     degree;
    size_t     i;


    number_of_roots = 0;
    node_a = heap->max;

    // Obtain the number of root nodes present
    if (node_a) 
    {
        ++number_of_roots;
        node_a = node_a->right;

        while (node_a != heap->max)
        {
            ++number_of_roots;
            node_a = node_a->right;
        }
    }
    // Loop through each node to pairwise combine
    while (number_of_roots > 0) 
    {
        degree = node_a->degree;
        next = node_a->right;

        while(true)
        {
            // If there's another node of same degree, combine
            node_b = heap->node_array[degree];

            if (!node_b) break;

            if (node_a->element < node_b->element) 
            {
                tmp = node_b;
                node_b = node_a;
                node_a = tmp;
            }

            // Makes node b the child of node a
            combine(node_b, node_a);
            heap->node_array[degree] = NULL;
            ++degree;
        }
        //update degree array and number of roots
        heap->node_array[degree] = node_a;
        node_a = next;
        --number_of_roots;
    }

    heap->max = NULL;

    //Place each degree node next to max node
    for (i = 0; i < array_size; ++i) 
    {
        node_b = heap->node_array[i];

        if (!node_b)
        {
            continue;
        }
        if (heap->max) 
        {
            node_b->left->right = node_b->right;
            node_b->right->left = node_b->left;

            node_b->left = heap->max;
            node_b->right = heap->max->right;
            heap->max->right = node_b;
            node_b->right->left = node_b;
            // Update max node if necessary
            if (node_b->element > heap->max->element)
            {
                heap->max = node_b;
            }
        }
        // Make it max node if max was empty
        else
        {
            heap->max = node_b;
        }
    }
    //delete heap->node_array;
}
/*
 *   Used by increase_key to cut the children and make them root nodes
 */
void fib_class::cut(fib_heap* heap, heap_node* smaller, heap_node* bigger)
{
    smaller->left->right = smaller->right;
    smaller->right->left = smaller->left;
    bigger->degree--;

    if (bigger->child == smaller) 
    {
        bigger->child = smaller->right;
    }

    if (bigger->degree == 0) 
    {
        bigger->child = NULL;
    }

    smaller->left = heap->max;
    smaller->right = heap->max->right;
    heap->max->right = smaller;
    smaller->right->left = smaller;

    smaller->parent = NULL;
    smaller->marked = false;
}
/*
 *   Recursively cut parents if they are already marked (child_cut = true)
 */
void fib_class::cascading_cut(fib_heap* heap, heap_node* node)
{
    heap_node* z = node->parent;

    if (z)
    {
        if (node->marked)
        {
            cut(heap, node, z);
            cascading_cut(heap, z);
        }
        else 
        {
            node->marked = true;
        }
    }
}