/*
 * In this example, all the features of the tree are shown
*/

#include <iostream>

#include "AgAVLTree.h"

/**
 * @brief                   Structure for storing a point on the x-y plane
 *
 */
struct point2D {

    int x;
    int y;

    /**
     * @brief           Compares two points, first based on their x coordinate (breaking ties by y coordinate)
     *
     * @param pA        First point to compare with
     * @param pB        Second point to compare to
     *
     * @return true     If the x coordinate of the first point is strictly less than that of the second point,
     *                  or if they are equal and the y coordinate of the first point is strictly less than that of the second point
     * @return false    If the x coordinate of the first point is greater than that of the second point,
     *                  or if they are equal and the y coordinate of the second point is strictly less than that of the first point
     */
    friend bool
    operator< (const point2D &pA, const point2D &pB)
    {
        if (pA.x == pB.x) {
            return pA.y < pB.y;
        }
        return pA.x < pB.x;
    }

    /**
     * @brief           Checks for equality of two points
     *
     * @param pA        First point to compare with
     * @param pB        Second point to compare to
     * @return true     If the x and y coordinates of the first point are strictly equal to that of the second
     * @return false    If the x and y coordinates of the first point are not strictly equal to that of the second
     */
    friend bool
    operator== (const point2D &pA, const point2D &pB)
    {
        return (pA.x == pB.x) && (pA.y == pB.y);
    }
};

/**
 * @brief               Compares two points based on the sum of their x and y coordinates
 *
 * @note                This comparison is arbitrary and does not have any special geometrical significance,
 *                      it is only meant to highlight the features of the tree
 *
 * @param pA            First point to compare with
 * @param pB            Second point to compare to
 * @return true         If the sum of the x and y coodinates of the first point is strictly less than that of the second
 * @return false        If the sum of the x and y coordinates of the first point is not strictly less than that of the second
 */
bool
alternate_lt_comp (const point2D & pA, const point2D & pB)
{

    return pA.x + pA.y < pB.x + pB.y;
}

/**
 * @brief               Checks for equality of two points based on the sum of their x and y coordinates
 *
 * @note                This comparison is arbitrary and does not have any special geometrical significance,
 *                      it is only meant to highlight the features of the tree
 *
 * @param pA            First point to compare with
 * @param pB            Second point to compare to
 * @return true         If the sum of the x and y coordinates of both points are strictly equal
 * @return false        If the sum of the x and y coordinates of both points are not strictly equal
 */
bool
alternate_eq_comp (const point2D & pA, const point2D & pB)
{

    return pA.x + pA.y == pB.x + pB.y;
}

int
main (void)
{

    int                 v;

    // tree for simple int type
    AgAVLTree<int>      tree1;

    // insert numbers from 0 to 7 (inclusive of both endpoints)
    for (int i = 0; i < 8; ++i) {
        bool inserted = tree1.insert (i);
        if (inserted) {
            std::cout << "Succeeded in inserting " << i << " into the tree";
        } else {
            std::cout << "Failed in inserting " << i << " into the tree";
        }
        std::cout << std::endl;
    }

    // try to insert 7 into the tree again
    v             = 7;
    bool inserted = tree1.insert (v);
    if (inserted) {
        std::cout << "Succeeded in inserting " << v << " into the tree";
    } else {
        std::cout << "Failed in inserting " << v << " into the tree";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    // print the size of the tree (number of elements inside)
    std::cout << "Size of the tree: " << tree1.size () << std::endl;

    // print the elements inside the tree (forward iteration)
    std::cout << "Order of elements in tree: " << std::endl;
    for (auto & e : tree1) {
        std::cout << e << std::endl;
        ;
    }
    std::cout << std::endl;

    // search for an exact match of an element inside the tree
    v       = 2;
    auto it = tree1.find (v);
    if (it != tree1.end ()) {
        std::cout << "Found " << v;
    } else {
        std::cout << "Could not find " << v;
    }
    std::cout << std::endl;

    // search for an element not inside the tree
    v  = 20;
    it = tree1.find (v);
    if (it != tree1.end ()) {
        std::cout << "Found " << v;
    } else {
        std::cout << "Could not find " << v;
    }
    std::cout << std::endl;

    // search for the smallest element strictly greater than 2 and print it
    v  = 2;
    it = tree1.first_greater_strict (v);
    if (it != tree1.end ()) {
        std::cout << "Smallest element greater than " << v << " is " << *it;
    } else {
        std::cout << "There is no element greater than " << v;
    }
    std::cout << std::endl;

    // search for the smallest element greater than or equal to 2 and print it
    v  = 2;
    it = tree1.first_greater_equals (v);
    if (it != tree1.end ()) {
        std::cout << "Smallest element greater than or equal to " << v << " is " << *it;
    } else {
        std::cout << "There is no element greater than or equal to " << v;
    }
    std::cout << std::endl;

    // search for the greatest element strictly less than 2 and print it
    v  = 2;
    it = tree1.last_smaller_strict (v);
    if (it != tree1.end ()) {
        std::cout << "Greatest element less than " << v << " is " << *it;
    } else {
        std::cout << "There is no element less than " << v;
    }
    std::cout << std::endl;

    // search for the smallest element less than or equal to 2 and print it
    v  = 2;
    it = tree1.last_smaller_equals (v);
    if (it != tree1.end ()) {
        std::cout << "Greatest element less than or equal to " << v << " is " << *it;
    } else {
        std::cout << "There is no element less than or equal to " << v;
    }
    std::cout << std::endl;
    std::cout << std::endl;

    // tree for point2D type (defined above)
    AgAVLTree<point2D>      tree2;
    tree2.insert ({0, 0});
    tree2.insert ({1, 2});
    tree2.insert ({2, -1});

    std::cout << "Order of points in tree with default comparator (operator<)" << std::endl;
    for (auto & e : tree2) {
        std::cout << "{" << e.x << ", " << e.y << "}" << std::endl;
    }
    std::cout << std::endl;

    // tree for point2D type, with custom comparators
    AgAVLTree<point2D, alternate_lt_comp, alternate_eq_comp> tree3;
    tree3.insert ({0, 0});
    tree3.insert ({1, 2});
    tree3.insert ({2, -1});

    std::cout << "Order of points in tree with custom comparator" << std::endl;
    for (auto & e : tree3) {
        std::cout << "{" << e.x << ", " << e.y << "}" << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
