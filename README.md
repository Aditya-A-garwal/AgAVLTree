# AgAVLTree

[![GitHub Workflow Status](https://img.shields.io/github/workflow/status/Aditya-A-garwal/AgAVLTree/build-test-ubuntu-gcc?label=build-test-ubuntu-gcc)](https://github.com/Aditya-A-garwal/AgAVLTree/actions/workflows/build-test-ubuntu-gcc.yaml)
[![GitHub Workflow Status](https://img.shields.io/github/workflow/status/Aditya-A-garwal/AgAVLTree/build-test-ubuntu-llvm?label=build-test-ubuntu-llvm)](https://github.com/Aditya-A-garwal/AgAVLTree/actions/workflows/build-test-ubuntu-llvm.yaml)
[![GitHub Workflow Status](https://img.shields.io/github/workflow/status/Aditya-A-garwal/AgAVLTree/build-test-windows-mingw?label=build-test-windows-mingw)](https://github.com/Aditya-A-garwal/AgAVLTree/actions/workflows/build-test-windows-mingw.yaml)
[![GitHub Workflow Status](https://img.shields.io/github/workflow/status/Aditya-A-garwal/AgAVLTree/build-test-windows-llvm?label=build-test-windows-llvm)](https://github.com/Aditya-A-garwal/AgAVLTree/actions/workflows/build-test-windows-llvm.yaml)
[![GitHub Workflow Status](https://img.shields.io/github/workflow/status/Aditya-A-garwal/AgAVLTree/build-test-windows-msvc?label=build-test-windows-msvc)](https://github.com/Aditya-A-garwal/AgAVLTree/actions/workflows/build-test-windows-msvc.yaml)

## Overview
This is a high-performance, templatized C++ implementation of the AVL tree data structure, which is a type of self balancing binary search tree. It supports fast insertion, deletion, binary searching and bidirectional iteration. It can be used for both, learning about AVL trees, and in applications.<br>
Along with the tree, the following have also been included -
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

Since AVL trees are a kind of binary search tree, they have a very wide domain in which they can be used, some examples of this are -
* Removing duplicate elements from an array
* Implementing Associative data structures like sets and maps, where a key and value pair must be kept together (ordered by the key)
* Implementing an index for a data base (in memory)

More specific examples (along with implementations) have been given in the ```examples``` directory.
## How to Build
### Dependencies
* CMake (version 3.16 or above)
* Build system such as Make or Ninja
* Google Test

**Note - Google Test is automatically fetched and built as part of the project.**

The tree uses CMake as a build system generator. Create a build directory in the root ```AgAVLTree``` directory and cd into it. Then run the following command to configure CMake -

    cmake -S .. -B .

Then, to build all the targets, the following command can be run (in the build directory itself) -

    cmake --build .

If the generator or compiler-toolchain needs to be changed, it can be done while configuring CMake. For example, to use NMake as a generator and GCC for compilation on Windows (instead of VS proj and the MSVC compiler), the command can be modified as follows -

    cmake -S .. -B . -G "NMake Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++

The above command also includes the benchmarks, tests and examples by default. Consequently, If any of them do not need to be built, then the command can be modified as follows -

    cmake -S .. -B . -DBUILD_BENCHMARKS=off -DBUILD_TESTS=off -DBUILD_EXAMPLES=off

For example, if only the tests do not have to be built, then the command may be modified to look like -

    cmake -S .. -B . -DBUILD_TESTS=off

**Note - In case a different compiler than the default is to be used, it is necessary to specify both the C and C++ compilers explicitly. This is because Google Test uses the C compiler as well.**
## How to Use
To use the tree, include the file ```AgAVLTree.h``` in your program and instantiate the ```AgAVLTree``` class. The type of data which the instance manages should be passed as a template argument. Additionally, custom comparators for less-than and equals comparisons can also be provided, which, if given, would be used over any overloaded < and == operators. If these are not provided, the type must have operator< and operator== implemented.<br>
The class contains insert and erase methods to insert and erase nodes, which return true or false depending on whether the insertion/erasing was succesful.
The class also contains 5 binary search methods (as described above), which for a given element, return an iterator to -
* An exactly matching element (find method)
* The smallest strictly greater element (find_first_greater_strict method)
* The smallest greater or equal element (find_first_greater_equals method)
* The greatest strictly less element (find_last_less_strict method)
* The greatest less or equal element (find_last_less_equals method)

The class also supports the range-based for iteration introduced in C++ 11 via C++ compliant iterators and reverse iterators. Both the forward and reverse iterators are bidirectional iterators. The implementations of forward and reverse iterators are in the ```AgAVLTree_iter.h``` file.
