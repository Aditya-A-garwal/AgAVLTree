#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#include <set>
#include <algorithm>

#include "../avl.h"

template <typename T>
std::string
format_integer (T pNum)
{

    T           cpy {pNum};
    int32_t     len {};

    std::string res;

    if(pNum == 0) {
        return "0";
    }

    while (cpy) {
        ++len, cpy /= 10;
    }

    for (int32_t i = 0, d; i < len; ++i) {

        d = pNum % 10;
        pNum /= 10;

        res += (char) (d + '0');
        if (i % 3 == 2 and i != len - 1) {
            res += ',';
        }
    }

    std::reverse (res.begin (), res.end ());

    return res;
}

struct ScopedTimer {

    private:
    std::chrono::high_resolution_clock::time_point m_startPoint;
    std::chrono::high_resolution_clock::time_point m_endPoint;

    const char *                                   msg;

    void
    stop ()
    {

        m_endPoint = std::chrono::high_resolution_clock::now ();
        auto diff  = std::chrono::duration_cast<std::chrono::milliseconds> (m_endPoint - m_startPoint).count ();
        std::cout << "Time elapsed for (" << msg << "): " << format_integer (diff) << " ms" << std::endl;
    }

    public:
    ScopedTimer (const char * pMsg) : msg {pMsg}
    {

        std::cout << "Starting new scoped timer for (" << msg << ")" << std::endl;
        m_startPoint = std::chrono::high_resolution_clock::now ();
    }

    ~ScopedTimer ()
    {
        stop ();
    }
};

int32_t     *buffInsert;
int32_t     *buffFind;
int32_t     *buffErase;

void
read_buffers (const char *pFilepath)
{
    std::ifstream       fin (pFilepath);
    int32_t             nInsert;
    int32_t             nFind;
    int32_t             nErase;

    if (!fin) {
        std::cout << "No file with name \"" << pFilepath << "\" exists" << std::endl;
        std::exit (-1);
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
main (int argc, char *argv[])
{
    std::ios_base::sync_with_stdio (false);
    std::cout.tie (NULL);
    std::cin.tie (NULL);

    if (argc < 3) {
        std::cout << "TO FEW ARGUMENTS\n";
        return -1;
    }

    read_buffers (argv[1]);

    for (int32_t i = 2; i < argc; ++i) {
        if (strcmp (argv[i], "1e6") == 0) {
            run_benchmark (1'000'000);
        }
        else if (strcmp (argv[i], "5e6") == 0) {
            run_benchmark (5'000'000);
        }
        else if (strcmp (argv[i], "1e7") == 0) {
            run_benchmark (10'000'000);
        }
        else if (strcmp (argv[i], "1.5e7") == 0) {
            run_benchmark (15'000'000);
        }
        else if (strcmp (argv[i], "2e7") == 0) {
            run_benchmark (20'000'000);
        }
        else {
            std::cout << argv[i] << " does not match any value\n";
        }
    }

    return 0;
}
