## Overview
A program for benchmarking the tree has also been included to measure the performance of the tree and compare it to std::set on the three operations- Insert, Find and Erase. The program requires a file to read records from, using which operations are performed.<br>
The record file contains a single number ```N``` on the first line, which is the number of operations to perform of each type. The next ```3N``` lines contain the records to be inserted, searched for and erased (```N``` each).<br>
 Given below is an example which inserts, searches for, and erases the numbers between 1 and 3 (inclusive of both endpoints) -
```
3
1
2
3
1
2
3
1
2
3
```
Three example generator programs (```random_gen.cpp```, ```preorder_gen.cpp``` and ```sequence_gen.cpp```) have also been given to generate record files. The programs require a directory called ```data``` to be created in the benchmarks directory, which will be used to store the files. **Note that the generated files are big, having 60 million records each (20 million of each type) and having a size of approximately 500 MB. They might take a few seconds to a few minutes to generate.** The size can be reduced by changing the value of the variable ```n``` in each of the files.<br>
The benchmark program must be invoked with the following arguments.
* Path to the record file
* Number of records of each type to use for running the benchmark (multiple values might be given, in which case each is run seperately)

Given below are the results of the benchmark program being run on my local machine for the file ```sequence_all.in``` with ```1,000,000``` and ```5,000,000``` operations (built on Ubuntu 20.04 using GCC).
```
$ ./benchmark ../data/sequence_all.in 1000000 5000000
Begin Reading File
Found 20,000,000 records each for Insert, Find and Erase
Done Reading File

1,000,000 Operations of each type

------------------------------------------------------
| Operation  | Class      | Successful  | Time (ms)  |
------------------------------------------------------
| Insertion  | std::set   | 1,000,000   | 249        |
| Insertion  | AgAVLTree  | 1,000,000   | 193        |
| Find       | std::set   | 1,000,000   | 130        |
| Find       | AgAVLTree  | 1,000,000   | 96         |
| Erase      | std::set   | 1,000,000   | 113        |
| Erase      | AgAVLTree  | 1,000,000   | 144        |
------------------------------------------------------


5,000,000 Operations of each type

------------------------------------------------------
| Operation  | Class      | Successful  | Time (ms)  |
------------------------------------------------------
| Insertion  | std::set   | 5,000,000   | 1,377      |
| Insertion  | AgAVLTree  | 5,000,000   | 1,098      |
| Find       | std::set   | 5,000,000   | 742        |
| Find       | AgAVLTree  | 5,000,000   | 674        |
| Erase      | std::set   | 5,000,000   | 704        |
| Erase      | AgAVLTree  | 5,000,000   | 991        |
------------------------------------------------------

Exiting
```