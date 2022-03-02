#include <iostream>
#include <fstream>

#include <random>
#include <algorithm>

struct pair {
    int32_t     value;
    int32_t     position;

    friend bool
    operator< (const pair &pA, const pair &pB)
    {
        if (pA.position == pB.position) {
            return pA.value < pB.value;
        }
        return pA.position < pB.position;
    }
};

void
dnc (pair *pAr, int32_t pLo, int32_t pHi, int32_t pLvl = 0)
{
    if (pLo > pHi) {
        return;
    }

    int32_t mid         = pLo + (pHi - pLo + 1) / 2;

    pAr[mid].position   = pLvl;

    dnc (pAr, pLo, mid - 1, pLvl + 1);
    dnc (pAr, mid + 1, pHi, pLvl + 1);
}

int
main (void)
{
    // Records will be written to ../preorder_all.in
    std::ofstream           fout ("../preorder_all.in");

    // 20 million records
    int32_t                 n       = 20'000'000;

    pair                    *ar;

    // not possible to move ahead without opening file
    if (!fout) {
        std::cout << "Unable to open file\n";
        return 1;
    }
    fout.tie (NULL);
    std::cout << "Opened File\n";

    ar      = new (std::nothrow) pair[n];

    // not possible to move ahead without allocating array
    if (ar == nullptr) {
        std::cout << "Unable to allocate array\n";
        return 1;
    }
    std::cout << "Allocated Array\n";

    // initialize array with numbers from 0 to n-1
    for (int32_t i = 0; i < n; ++i) {
        ar[i].value     = i;
    }
    dnc (ar, 0, n - 1);
    std::sort (ar, ar + n);

    // print n (number of records) and print records thrice for insert, find and erase (re-shuffling each time)
    fout << n << '\n';
    for (int32_t _ = 0; _ < 3; ++_) {
        for (int32_t i = 0; i < n; ++i) {
            fout << ar[i].value << '\n';

            if ((i & 65'535) == 0) {
                std::cout << "\rDone " << ((i * 100) / (n * 3)) + (_ * 33) << "%  ";
            }
        }
    }
    std::cout << "\rDone 100%\n";

    return 0;
}