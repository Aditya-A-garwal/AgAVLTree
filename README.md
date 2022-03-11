# AgAVLTree

## Overview
This is a high-performance, templatized C++ implementation of the AVL tree data structure, which is a self balancing binary search tree. It supports fast insertion, deletion, binary searching and bidirectional iteration. It can be used for both, learning about AVL trees, and in applications.<br>
Along with the tree, the following have also been included-
* Unit Tests based on the Google Test framework (intended to be exhaustive)
* Program to benchmark the tree and measure its performance
* Examples demonstrating how to use the tree in practice

More information on each of the above can be found in their respective directories.

## Operations and Uses

| Operation                                 | Asymptotic Complexity     |
| :---                                      | :----:                    |
| Insert                                    | O(logN)                   |
| Erase                                     | O(logN)                   |
| Find exact element                        | O(logN)                   |
| Find smallest element strictly greater    | O(logN)                   |
| Find smallest element greater or equal    | O(logN)                   |
| Find smallest element strictly less       | O(logN)                   |
| Find smallest element less or equal       | O(logN)                   |

AVL trees can be used anywhere fast insertion, deletion and lookup of keys are required, Some examples of this are-
* Counting the number of distinct elements in an array without sorting or modifying it
* Implementing data structure such as sets, maps and priority queues
* Maintaining a sorted container of objects, with insertions and deletions happening at random

## How to Build
### Dependencies
* Cmake (version 3.16 or above)
* Build system such as Make or Ninja
* Google Test

**Note- Google test is automatically fetched and built as part of the project.**

The tree uses Cmake as a build system generator. Create a build directory in the root ```AgAVLTree``` directory and cd into it. Then run the following command to configure Cmake-

    cmake -S .. -B .

If the generator or compiler-toolchain needs to be changed, it can be done at this step. For example, to use NMake as a generator and GCC for compilation on windows (instead of VS proj and the MSVC compiler), the command can be modified as follows-

    cmake -S .. -B . -G "NMake Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++

**Note- In case a different compiler than the default is to be used, it is important to specify both the C and C++ compilers explicitly. This is because Google Test uses the C compiler as well.**
## How to Use
To use the tree, include the file ```AgAVLTree.h``` in your program and instantiate the ```AgAVLTree``` class. The type of data which the instance manages should be passed as a template argument. Additionally, custom comparators for less-than and equals comparisons can also be provided, which, if given, would be used over any overloaded < and == operators. If these are not provided, the type must have operator< and operator== implemented.<br>
The class contains insert and erase methods to insert and erase nodes, which return true or false depending on whether the insertion/erasing was succesful.
The class also contains 5 binary search methods (as described above), which for a given element, return an iterator to-
* An exactly matching element (find method)
* The smallest strictly greater element (find_first_greater_strict method)
* The smallest greater or equal element (find_first_greater_equals method)
* The greatest strictly less element (find_last_less_strict method)
* The greatest less or equal element (find_last_less_equals method)

The class also supports the range-based for iteration introduced in C++ 11 via C++ compliant iterators and reverse iterators. Both the forward and reverse iterators are bidirectional iterators. The implementations of forward and reverse iterators are in the ```AgAVLTree_iter.h``` file.
