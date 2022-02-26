# AgAVLTree

## Overview
This is a high-performance, templatized C++ implementation of the AVL tree datastructure, which is a self balancing binary search tree. It supports fast insertion, deletion, binary searching and iteration. It can be used for both, learning about AVL trees, and in applications.<br>

## Uses
Since AVL trees are a kind of self balancing binary search trees, they can be used anywhere fast insertion, deletion and lookup of keys are required, Some examples of this are-
* Counting the number of distinct elements in arrays without sorting or modifying them
* Implementing the abstract set and map data structures (both require fast insertion, deletion and lookup of keys)
* Any application where data is constantly entering/leaving and fast lookup is required
## Operations

| Operation                                 | Asymptotic Complexity     |
| :---                                      | :----:                    |
| Insert                                    | O(logN)                   |
| Erase                                     | O(logN)                   |
| Find exact element                        | O(logN)                   |
| Find smallest element strictly greater    | O(logN)                   |
| Find smallest element greater or equal    | O(logN)                   |
| Find smallest element strictly less       | O(logN)                   |
| Find smallest element less or equal       | O(logN)                   |
## Dependencies
* Cmake (for build system)
* Google test library (for unit testing)

## How to Build
The tree uses Cmake as a build system generator. Create two build directories in the root ```AVLtree``` and ```tests``` directories. Then CD into ```AVLtree/build``` and run the following command-

    cmake -S .. -B .

The example program (more information given below) can be run by running the file ```build/main``` while the tests can be run by running the file ```tests/build/tests``` (both paths are relative to the root ```AVLtree``` directory)
## How to Use
**The file ```main.cpp``` contains a sample program to demonstrate the following features.**
<br>
To use the tree, include the file ```avl.h``` in your program and instantiate the ```AgAVLTree``` class. The data type of the data which the instance manages can be supplied as a template parameter. Additionally, custom comparators for less-than and equals comparisons can also be provided, which, if given, would be used over any overloaded < and == operators.
The class contains insert and erase methods to insert and erase nodes, which return true or false depending on whether the insertion/erasing was succesful.
The class also contains 5 binary search methods (as described above), which for a given element, return an iterator to-
* An exactly matching element (find method)
* The smallest strictly greater element (find_first_greater_strict method)
* The smallest greater or equal element (find_first_greater_equals method)
* The greatest strictly less element (find_last_less_strict method)
* The greatest less or equal element (find_last_less_equals method)

The class also supports the range-based for iteration introduced in C++ 11 via C++ compliant iterators and reverse iterators. Both the forward and reverse iterators are bidirectional iterators.

## Tests
A reasonable set of unit tests (intended to be exhaustive) are included, which can be used to test the features provided by the tree. The tests are based on the Google Test Framework. More information can be found in the ```tests``` directory.