/**
 * @file                    array_distinct.cpp
 * @author                  Aditya Agarwal (aditya,agarwal@dumblebots.com)
 *
 * In this example, given an array, we need to find the number of distinct elements inside it without
 * modifying it
 * All the elements from the array be iterated over and inserted into an AVL tree one by one
 * Since only one copy of each element will be succesfully inserted into the tree, the number of elements
 * in the tree after iterating over the array, is the answer
*/

#include <iostream>

#include "AgAVLTree.h"

int
main (void)
{
    // create, intialize array and find its size
    int             ar[]    = {4, 4, 4, 4, 2, 2, 3, 3, 3, 1};
    int             sz      = sizeof (ar) / sizeof (ar[0]);

    // create tree and also iterator (for later use)
    AgAVLTree<int>  tree;
    AgAVLTree<int>::iterator it;

    // insert elements into tree
    // each element will only be successfuly inserted once, thus removing repeats
    // the original array remains unmodified
    for (auto &e : ar) {
        tree.insert (e);
    }

    // print the array to the console
    std::cout << '\n';
    std::cout << "The given array " << '{';
    for (int i = 0; i < sz - 1; ++i) {
        std::cout << ar[i] << ", ";
    }
    std::cout << ar[sz - 1] << '}' << '\n';

    // print the number of disctinct elements in the array (= size of tree = number of elements in tree)
    std::cout << "Contains " << tree.size () << " distinct elements\n";

    // print each element of the array once by iterating over the tree
    std::cout << "Which are " << '{';
    it          = tree.begin ();
    for (int i = 0; i < (int)tree.size () - 1; ++i, ++it) {
        std::cout << *it << ", ";
    }
    std::cout << *it << '}' << '\n';
    std::cout << '\n';

    return 0;
}
