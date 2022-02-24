#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>

#include "formatInteger.h"
#include "scopedTimer.h"

#include "../avl.h"

int32_t     nInsert;
int32_t     nFind;
int32_t     nErase;

int32_t     *buffInsert;
int32_t     *buffFind;
int32_t     *buffErase;

void
read_buffers (const char *pFilepath)
{
    std::ifstream       fin (pFilepath);

    if (!fin) {
        std::cout << "No file with name \"" << pFilepath << "\" exists" << std::endl;
        std::exit (0);
    }

    fin.tie (NULL);

    fin >> nInsert >> nFind >> nErase;

    buffInsert          = new (std::nothrow) int[nInsert];
    buffFind            = new (std::nothrow) int[nFind];
    buffErase           = new (std::nothrow) int[nErase];

    if (buffInsert == nullptr or buffFind == nullptr or buffErase == nullptr) {
        std::cout << "Could not allocate buffers" << std::endl;
        std::exit (0);
    }

    for (int32_t i = 0; i < nInsert; ++i) {
        fin >> buffInsert[i];
    }

    for (int32_t i = 0; i < nFind; ++i) {
        fin >> buffFind[i];
    }

    for (int32_t i = 0; i < nErase; ++i) {
        fin >> buffErase[i];
    }
}

void
run_benchmark (int32_t pN)
{
    int32_t                         nInsert;
    int32_t                         nFind;
    int32_t                         nErase;

    AgAVLTree<int32_t>              tree1;
    AgAVLTree<int32_t>::iterator    it1;

    std::set<int32_t>               tree2;
    std::set<int32_t>::iterator     it2;

    int32_t                         cntr;

    nInsert     = pN;
    nFind       = pN;
    nErase      = pN;

    std::cout << "--------------------------------------------------\n";

    std::cout << format_integer (nInsert) << " Insertions\n";
    std::cout << format_integer (nFind) << " Finds\n";
    std::cout << format_integer (nErase) << " Erases\n";
    std::cout << '\n';

    {
        cntr    = 0;
        bool    flag;
        ScopedTimer timer ("std::set Insert");
        for (int32_t i = 0; i < nInsert; ++i) {
            std::tie (std::ignore, flag)    = tree2.insert (buffInsert[i]);
            cntr                            += flag;
        }
    }
    std::cout << "Successful Inserts: " << format_integer (cntr) << '\n';
    std::cout << '\n';

    {
        cntr    = 0;
        ScopedTimer timer ("AgAVLTree Insert");
        for (int32_t i = 0; i < nInsert; ++i) {
            cntr                            += (int32_t)tree1.insert (buffInsert[i]);
        }
    }
    std::cout << "Successful Inserts: " << format_integer (cntr) << '\n';
    std::cout << '\n';
    std::cout << '\n';

    {
        cntr    = 0;
        ScopedTimer timer ("std::set Find");
        for (int32_t i = 0; i < nFind; ++i) {
            it2                             = tree2.find (buffFind[i]);
            cntr                            += it2 != tree2.end ();
        }
    }
    std::cout << "Successful Finds: " << format_integer (cntr) << '\n';
    std::cout << '\n';

    {
        cntr    = 0;
        ScopedTimer timer ("AgAVLTree Find");
        for (int32_t i = 0; i < nFind; ++i) {
            it1                             = tree1.find (buffFind[i]);
            cntr                            += it1 != tree1.end ();
        }
    }
    std::cout << "Successful Finds: " << format_integer (cntr) << '\n';
    std::cout << '\n';
    std::cout << '\n';

    {
        cntr    = 0;
        ScopedTimer timer ("std::set Erase");
        for (int32_t i = 0; i < nErase; ++i) {
            cntr                            += tree2.erase (buffErase[i]);
        }
    }
    std::cout << "Successful Erases: " << format_integer (cntr) << '\n';

    {
        cntr    = 0;
        ScopedTimer timer ("AgAVLTree Erase");
        for (int32_t i = 0; i < nErase; ++i) {
            cntr                            += tree1.erase (buffErase[i]);
        }
    }
    std::cout << "Successful Erases: " << format_integer (cntr) << '\n';

    std::cout << "--------------------------------------------------\n";
    std::cout << '\n';
}

int
main (void)
{
    std::ios_base::sync_with_stdio (false);
    std::cout.tie (NULL);
    std::cin.tie (NULL);

    read_buffers ("../random_all2.in");

    run_benchmark (1'000'000);
    // run_benchmark (5'000'000);
    // run_benchmark (10'000'000);
    // run_benchmark (15'000'000);
    // run_benchmark (20'000'000);

    return 0;
}
