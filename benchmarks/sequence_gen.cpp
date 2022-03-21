/**
 * @file                sequence_gen.cpp
 * @author              Aditya Agarwal (aditya.agarwal@dumblebots.com)
 * @brief               Program to generate benchmark record file, with records in ascending order
 */

#include <iostream>
#include <fstream>

#include <random>
#include <algorithm>

int
main (void)
{
    // Records will be written to ../sequence_all.in
    std::ofstream           fout (FILE_OUT);

    // 20 million records
    int32_t                 n       = 20'000'000;

    // not possible to move ahead without opening file
    if (!fout) {
        std::cout << "Unable to open file\n";
        std::cout << "Please check if the data directory exists in the benchmarks directory\n";
        return 1;
    }
    fout.tie (NULL);
    std::cout << "Opened File\n";

    // print n (number of records) and the records thrice for insert, find and erase
    fout << n << '\n';
    for (int32_t _ = 0; _ < 3; ++_) {
        for (int32_t i = 0; i < n; ++i) {
            fout << i << '\n';

            if ((i & 65'535) == 0) {
                std::cout << "\rDone " << ((i * 100) / (n * 3)) + (_ * 33) << "%  ";
            }
        }
    }
    std::cout << "\rDone 100%\n";

    return 0;
}