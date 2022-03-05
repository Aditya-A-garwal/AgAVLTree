#include <iostream>

#include "../AgAVLTree.h"

struct point2D {

    int x;
    int y;

    bool
    operator< (const point2D & other) const
    {
        if (x == other.x)
            return y < other.y;
        return x < other.x;
    }

    bool
    operator== (const point2D & other) const
    {
        return x == other.x && y == other.y;
    }
};

bool
alternate_lt_comp (const point2D & a, const point2D & b)
{

    return a.x + a.y < b.x + b.y;
}

bool
alternate_eq_comp (const point2D & a, const point2D & b)
{

    return a.x + a.y == b.x + b.y;
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
