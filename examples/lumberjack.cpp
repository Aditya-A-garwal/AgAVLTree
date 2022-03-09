/*
 * In this example, we are given a set of logs and requests. Each log has a size and an ID. Each request
 * has a minimum size requirement
 * For each request, report the smallest log that meets the minimum size requirement of the request and
 * remove it from the set of available logs, repeating this for each all requests in order
*/

#include <iostream>

#include "AgAVLTree.h"

/**
 * @brief               Structure to store information about a log (ID and size)
 *
 */
struct log
{
    int     mId;
    int     mSize;

    /**
     * @brief           Compares two logs based on their size (breaking ties by their IDs)
     *
     * @param pA        First log to compare with
     * @param pB        Second log to compare to
     * @return true     If pA is smaller than pB (or if they are equal and pA's ID appears before that of pB)
     * @return false    If pA is larget than pb (or if they are equal and pA's ID appears after that of pB)
     */
    friend bool
    operator< (const log &pA, const log &pB)
    {
        if (pA.mSize == pB.mSize) {
            return pA.mId < pB.mId;
        }
        return pA.mSize < pB.mSize;
    }

    // two logs are equal if their sizes and IDs match exactly

    /**
     * @brief           Checks for equality of two logs
     *
     * @param pA        First log to compare with
     * @param pB        Second log to compare to
     * @return true     If pA and pB have the same size and ID
     * @return false    If Pa and pB do not have the same size and ID
     */
    friend bool
    operator== (const log &a, const log &b)
    {
        return (a.mSize == b.mSize) && (a.mSize == b.mSize);
    }
};

int
main (void)
{
    // initial set of logs (each having an ID and a size)
    log                 ar[]        {
        {0, 10},
        {1, 25},
        {2, 40},
        {3, 5},
        {4, 16},
        {5, 25},
        {6, 10},
        {7, 4},
        {8, 100},
        {9, 120},
        {10, 36},
        {11, 6}
    };

    // requests, containing the minimum size requirement
    int                 requests[]  {
        20,
        10,
        100,
        120,
        120,
        5,
        4,
        150,
        12,
        25,
    };

    int                 numRequests = sizeof (requests) / sizeof (requests[0]);

    AgAVLTree<log>      tree;

    // insert all log objects into the tree and print the final size of the tree
    for (auto &e : ar) {
        tree.insert (e);
    }
    std::cout << "Tree has " << tree.size () << " elements (logs)\n";

    for (int i = 0; i < numRequests; ++i) {

        // for each request, print the result, try to find the smallest log which meets the requirement
        std::cout << "For request #" << i << " (with size requirement " << requests[i] << "):\n\t";
        auto    it  = tree.first_greater_equals ({-1, requests[i]});

        // if no such log, could be found, report it
        if (it == tree.end ()) {
            std::cout << "No log could be found\n";
        }

        // if a log could be found, report its ID and size, then remove it from the tree
        else {
            std::cout << "Found log #" << (*it).mId << " with size " << (*it).mSize << '\n';
            tree.erase (*it);
        }
        std::cout << '\n';
    }

    return 0;
}
