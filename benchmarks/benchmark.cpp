#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <tuple>
#include <chrono>
#include <set>
#include <vector>

#include "../avl.h"

struct Timer {

    private:

    std::chrono::high_resolution_clock::time_point      mStart;
    std::chrono::high_resolution_clock::time_point      mEnd;

    public:

    Timer ()
    {
        reset ();
    }

    int64_t
    elapsed ()
    {
        mEnd        = std::chrono::high_resolution_clock::now ();
        auto diff   = std::chrono::duration_cast<std::chrono::milliseconds> (mEnd - mStart).count ();

        return diff;
    }

    void
    reset ()
    {
        mStart      = std::chrono::high_resolution_clock::now ();
    }
};

struct table {

    private:

    std::vector<std::string>                mHeaders;
    std::vector<std::vector<std::string>>   mRows;

    public:

    table ()
    {}

    void
    add_headers (std::string pHeaders[], int pSz)
    {
        if (pSz == 0)
        {
            std::cout << "ZERO COLOUMNS NOT ALLOWED IN TABLE\n";
            std::exit (1);
        }
        mHeaders            = std::vector<std::string>(pHeaders, pHeaders + pSz);
    }

    void
    add_row (std::string pElems[], int32_t pSz)
    {
        if (pSz != (int32_t)mHeaders.size ())
        {
            std::cout << "NUMBER OF COLOUMNS IN ROW MUST MATCH NUMBER OF COLOUMNS IN HEADER\n";
            std::exit (1);
        }

        mRows.push_back (std::vector<std::string>(pElems, pElems + pSz));
    }

    friend std::ostream
    &operator<< (std::ostream &stream, const table &pOther)
    {
        int32_t                 cols    = pOther.mHeaders.size ();
        int32_t                 width   = 0;

        std::vector<int32_t>    sz (cols);

        for(int32_t col = 0; col < cols; ++col) {
            sz[col] = pOther.mHeaders[col].size ();
        }

        for (auto &e : pOther.mRows) {
            for (int32_t col = 0; col < cols; ++col) {

                sz[col] = std::max (sz[col], (int32_t)e[col].size ());
            }
        }

        for (auto &e : sz) {
            e       += 4;
            width   += e;
        }

        // print the headers
        for (int32_t i = 0; i < width; ++i) {
            stream << '-';
        }
        stream << '-' << '\n';
        for (int32_t i = 0; i < cols; ++i) {
            stream << "| ";
            stream << pOther.mHeaders[i];
            for (int32_t pad = pOther.mHeaders[i].size () + 2; pad < sz[i]; ++pad) {
                stream << ' ';
            }
        }
        stream << '|' << '\n';

        for (int32_t i = 0; i < width; ++i) {
            stream << '-';
        }
        stream << '-' << '\n';

        for (auto &row : pOther.mRows) {
            for (int32_t i = 0; i < cols; ++i) {
                stream << "| ";
                stream << row[i];
                for (int32_t pad = row[i].size () + 2; pad < sz[i]; ++pad) {
                    stream << ' ';
                }
            }
            stream << '|' << '\n';
        }

        if (pOther.mRows.size () == 0) {
            return stream;
        }

        for (int32_t i = 0; i < width; ++i) {
            stream << '-';
        }
        stream << '-' << '\n';

        return stream;
    }
};

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

int32_t     *buffInsert;
int32_t     *buffFind;
int32_t     *buffErase;

void
read_buffers (const char *pFilepath)
{
    std::ifstream       fin (pFilepath);
    int32_t             n;

    if (!fin) {
        std::cout << "No file with name \"" << pFilepath << "\" exists" << std::endl;
        std::exit (-1);
    }
    fin.tie (NULL);

    fin >> n;

    buffInsert          = new (std::nothrow) int[n];
    buffFind            = new (std::nothrow) int[n];
    buffErase           = new (std::nothrow) int[n];

    if (buffInsert == nullptr or buffFind == nullptr or buffErase == nullptr) {
        std::cout << "Could not allocate buffers" << std::endl;
        std::exit (1);
    }

    for (int32_t i = 0; i < n; ++i) {
        fin >> buffInsert[i];
    }

    for (int32_t i = 0; i < n; ++i) {
        fin >> buffFind[i];
    }

    for (int32_t i = 0; i < n; ++i) {
        fin >> buffErase[i];
    }
}

void
run_benchmark (int pN)
{
    std::set<int32_t>               tree1;
    std::set<int32_t>::iterator     it1;

    AgAVLTree<int32_t>              tree2;
    AgAVLTree<int32_t>::iterator    it2;

    Timer                           timer;
    int32_t                         measured;

    table results;
    std::string                     row[]   = {"Operation", "Class", "Successful", "Time (ms)"};

    int32_t                         flag;
    int32_t                         cntr;

    std::cout << '\n';
    std::cout << format_integer (pN) << " Insertions\n";
    std::cout << format_integer (pN) << " Finds\n";
    std::cout << format_integer (pN) << " Erases\n";
    std::cout << '\n';

    results.add_headers (row, 4);





    cntr = 0;
    timer.reset ();
    for (auto i = 0; i < pN; ++i) {
        tie (std::ignore, flag)     = tree1.insert (buffInsert[i]);
        cntr                        += flag;
    }
    measured    = timer.elapsed ();
    row[0]      = "Insertion";
    row[1]      = "std::set";
    row[2]      = format_integer (cntr);
    row[3]      = format_integer (measured);
    results.add_row (row, 4);

    cntr = 0;
    timer.reset ();
    for (auto i = 0; i < pN; ++i) {
        flag                        = tree2.insert (buffInsert[i]);
        cntr                        += flag;
    }
    measured = timer.elapsed ();
    row[0]      = "Insertion";
    row[1]      = "AgAVLTree";
    row[2]      = format_integer (cntr);
    row[3]      = format_integer (measured);
    results.add_row (row, 4);




    cntr = 0;
    timer.reset ();
    for (auto i = 0; i < pN; ++i) {
        it1                         = tree1.find (buffFind[i]);
        cntr                        += (int32_t)(it1 != tree1.end ());
    }
    measured    = timer.elapsed ();
    row[0]      = "Find";
    row[1]      = "std::set";
    row[2]      = format_integer (cntr);
    row[3]      = format_integer (measured);
    results.add_row (row, 4);

    cntr = 0;
    timer.reset ();
    for (auto i = 0; i < pN; ++i) {
        it2                         = tree2.find (buffFind[i]);
        cntr                        += (int32_t)(it2 != tree2.end ());
    }
    measured = timer.elapsed ();
    row[0]      = "Find";
    row[1]      = "AgAVLTree";
    row[2]      = format_integer (cntr);
    row[3]      = format_integer (measured);
    results.add_row (row, 4);




    cntr = 0;
    timer.reset ();
    for (auto i = 0; i < pN; ++i) {
        flag                        = tree1.erase (buffErase[i]);
        cntr                        += flag;
    }
    measured    = timer.elapsed ();
    row[0]      = "Erase";
    row[1]      = "std::set";
    row[2]      = format_integer (cntr);
    row[3]      = format_integer (measured);
    results.add_row (row, 4);

    cntr = 0;
    timer.reset ();
    for (auto i = 0; i < pN; ++i) {
        flag                        = tree2.erase (buffErase[i]);
        cntr                        += flag;
    }
    measured = timer.elapsed ();
    row[0]      = "Erase";
    row[1]      = "AgAVLTree";
    row[2]      = format_integer (cntr);
    row[3]      = format_integer (measured);
    results.add_row (row, 4);





    std::cout << results << std::endl;
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
}