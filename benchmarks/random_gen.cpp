/**
 * @file                random_gen.cpp
 * @author              Aditya Agarwal (aditya.agarwal@dumblebots.com)
 * @brief               Program to generate benchmark record file, with records in random order
 */

#include <iostream>
#include <fstream>

#include <random>
#include <algorithm>

int
main (void)
{
    // Records will be written to ../random_all.in
    std::ofstream           fout (FILE_OUT);

    // 20 million records, shuffling will be done in blocks of 1 million
    int32_t                 n       = 20'000'000;
    int32_t                 step    = 1'000'000;

    std::random_device      rd;

    int32_t                 *ar;

    // not possible to move ahead without opening file
    if (!fout) {
        std::cout << "Unable to open file\n";
        std::cout << "Please check if the data directory exists in the benchmarks directory\n";
        return 1;
    }
    fout.tie (NULL);
    std::cout << "Opened File\n";

    ar      = new (std::nothrow) int[n];

    // not possible to move ahead without allocating array
    if (ar == nullptr) {
        std::cout << "Unable to allocate array\n";
        return 1;
    }
    std::cout << "Allocated Array\n";

    // initialize array with numbers from 0 to n-1 and shuffle initially
    for (int32_t i = 0; i < n; ++i) {
        ar[i]   = i;
    }
    std::shuffle (ar, ar + n, std::mt19937(rd()));

    // print n (number of records) and print records thrice for insert, find and erase (re-shuffling each time)
    fout << n << '\n';
    for (int32_t _ = 0; _ < 3; ++_) {

        for (int32_t i = 0; i < n; i += step) {
            std::shuffle (ar + i, ar + i + step, std::mt19937(rd()));
        }
        for (int32_t i = 0; i < n; ++i) {
            fout << ar[i] << '\n';

            if ((i & 65'535) == 0) {
                std::cout << "\rDone " << ((i * 100) / (n * 3)) + (_ * 33) << "%  ";
            }
        }
    }
    std::cout << "\rDone 100%\n";

    return 0;
}