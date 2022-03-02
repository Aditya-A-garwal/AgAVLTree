// file and std IO
#include <iostream>
#include <fstream>

// measuring time
#include <chrono>

// tie
#include <tuple>

// table printing
#include <vector>

// std::set
#include <set>

// AgAVLTree
#include "../AgAVLTree.h"

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
    add_headers (std::initializer_list<std::string> pHeaders)
    {
        if (pHeaders.size () == 0)
        {
            std::cout << "ZERO COLOUMNS NOT ALLOWED IN TABLE\n";
            std::exit (1);
        }
        mHeaders            = pHeaders;
    }

    void
    add_row (std::initializer_list<std::string> pElems)
    {
        if (pElems.size () != mHeaders.size ())
        {
            std::cout << "NUMBER OF COLOUMNS IN ROW MUST MATCH NUMBER OF COLOUMNS IN HEADER\n";
            std::exit (1);
        }

        mRows.push_back (pElems);
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

bool
streq (const char *pA, const char *pB)
{
    for (size_t i = 0; ; ++i) {

        if (pA[i] == 0 && pB[i] == 0)
            break;

        if (pA[i] == pB[i])
            continue;

        else
            return 0;
    }

    return true;
}

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
        if (i % 3 == 2 && i != len - 1) {
            res += ',';
        }
    }

    for (auto i = 0; i < (int32_t)(res.size () / 2); ++i) {
        std::swap (res[i], res[res.size () - i - 1]);
    }

    return res;
}

int32_t     *buffInsert;
int32_t     *buffFind;
int32_t     *buffErase;

int32_t     maxN;

void
read_buffers (const char *pFilepath)
{
    std::ifstream       fin (pFilepath);

    if (!fin) {
        std::cout << "No file with name \"" << pFilepath << "\" exists" << std::endl;
        std::exit (-1);
    }
    fin.tie (NULL);

    fin >> maxN;

    buffInsert          = new (std::nothrow) int[maxN];
    buffFind            = new (std::nothrow) int[maxN];
    buffErase           = new (std::nothrow) int[maxN];

    if (buffInsert == nullptr || buffFind == nullptr || buffErase == nullptr) {
        std::cout << "Could not allocate buffers" << std::endl;
        std::exit (1);
    }

    std::cout << "Begin Reading File\n";
    std::cout << "Found " << format_integer (maxN) << " records each for Insert, Find and Erase\n";

    for (int32_t i = 0; i < maxN; ++i) {
        fin >> buffInsert[i];

        if ((i & 65'535) == 0) {
            std::cout << "\rReading Insert " << ((100 * (int64_t)i) / maxN) << '%' << "  ";
        }
    }

    for (int32_t i = 0; i < maxN; ++i) {
        fin >> buffFind[i];

        if ((i & 65'535) == 0) {
            std::cout << "\rReading Find   " << ((100 * (int64_t)i) / maxN) << '%' << "  ";
        }
    }

    for (int32_t i = 0; i < maxN; ++i) {
        fin >> buffErase[i];

        if ((i & 65'535) == 0) {
            std::cout << "\rReading Erase  " << ((100 * (int64_t)i) / maxN) << '%' << "  ";
        }
    }

    std::cout << "\rDone Reading File    \n";
}

void
run_benchmark (int pN)
{
    std::set<int32_t>               tree1;
    std::set<int32_t>::iterator     it1;

    AgAVLTree<int32_t>              tree2;
    AgAVLTree<int32_t>::iterator    it2;

    Timer                           timer;
    int64_t                         measured;

    table results;

    int32_t                         flag;
    int32_t                         cntr;

    if (pN > maxN) {
        std::cout << "\nGiven " << format_integer (pN) << " operations exceeds the number of records supplied by the file\n";
        return;
    }

    std::cout << '\n';
    std::cout << format_integer (pN) << " Insertions\n";
    std::cout << format_integer (pN) << " Finds\n";
    std::cout << format_integer (pN) << " Erases\n";
    std::cout << '\n';

    results.add_headers ({"Operation", "Class", "Successful", "Time (ms)"});


    cntr = 0;
    timer.reset ();
    for (auto i = 0; i < pN; ++i) {
        std::tie(std::ignore, flag) = tree1.insert (buffInsert[i]);
        cntr                        += flag;
    }
    measured    = timer.elapsed ();
    results.add_row ({"Insertion", "std::set", format_integer (cntr), format_integer (measured)});

    cntr = 0;
    timer.reset ();
    for (auto i = 0; i < pN; ++i) {
        flag                        = tree2.insert (buffInsert[i]);
        cntr                        += flag;
    }
    measured = timer.elapsed ();
    results.add_row ({"Insertion", "AgAVLTree", format_integer (cntr), format_integer (measured)});


    cntr = 0;
    timer.reset ();
    for (auto i = 0; i < pN; ++i) {
        it1                         = tree1.find (buffFind[i]);
        cntr                        += (int32_t)(it1 != tree1.end ());
    }
    measured    = timer.elapsed ();
    results.add_row ({"Find", "std::set", format_integer (cntr), format_integer (measured)});

    cntr = 0;
    timer.reset ();
    for (auto i = 0; i < pN; ++i) {
        it2                         = tree2.find (buffFind[i]);
        cntr                        += (int32_t)(it2 != tree2.end ());
    }
    measured = timer.elapsed ();
    results.add_row ({"Find", "AgAVLTree", format_integer (cntr), format_integer (measured)});


    cntr = 0;
    timer.reset ();
    for (auto i = 0; i < pN; ++i) {
        flag                        = tree1.erase (buffErase[i]);
        cntr                        += flag;
    }
    measured    = timer.elapsed ();
    results.add_row ({"Erase", "std::set", format_integer (cntr), format_integer (measured)});

    cntr = 0;
    timer.reset ();
    for (auto i = 0; i < pN; ++i) {
        flag                        = tree2.erase (buffErase[i]);
        cntr                        += flag;
    }
    measured = timer.elapsed ();
    results.add_row ({"Erase", "AgAVLTree", format_integer (cntr), format_integer (measured)});;


    std::cout << results << std::endl;
}

int
main (int argc, char *argv[])
{
    if (argc < 3) {
        std::cout << "Usage: ";
        std::cout << argv[0] << " input_file op1 [op2...]\n";

        std::cout << "\nOptions:\n";
        std::cout << "  1e6\t\tperform 1 million operations of each type\n";
        std::cout << "  5e6\t\tperform 5 million operations of each type\n";
        std::cout << "  1e7\t\tperform 10 million operations of each type\n";
        std::cout << "  1.5e7\t\tperform 15 million operations of each type\n";
        std::cout << "  2e7\t\tperform 20 million operations of each type\n";

        std::cout << "\nExample: ";
        std::cout << argv[0] << " ../random_all.in 1e6 5e6\n";

        return 1;
    }

    read_buffers (argv[1]);

    for (int32_t i = 2; i < argc; ++i) {
        if (streq (argv[i], "1e6")) {
            run_benchmark (1'000'000);
        }
        else if (streq (argv[i], "5e6")) {
            run_benchmark (5'000'000);
        }
        else if (streq (argv[i], "1e7")) {
            run_benchmark (10'000'000);
        }
        else if (streq (argv[i], "1.5e7")) {
            run_benchmark (15'000'000);
        }
        else if (streq (argv[i], "2e7")) {
            run_benchmark (20'000'000);
        }
        else if (streq (argv[i], "4e7")) {
            run_benchmark (40'000'000);
        }
        else {
            std::cout << argv[i] << " does not match any value\n";
        }
    }
}