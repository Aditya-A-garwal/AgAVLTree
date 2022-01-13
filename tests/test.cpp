#include <gtest/gtest.h>

#include <string>
#include <cstdlib>

#define AVL_TEST_MODE   1   // to be able to access private members and add extra diagnostic info collection
#define CAP_TEST        0   // to run capacity test or not
#include  "../avl.h"

namespace balance_info = avl_tree::balance_info;

TEST( Smoke, smoke_test ) {

    avl_tree::AVL<int32_t> tree;

    constexpr int32_t lo    = -1'000'000;
    constexpr int32_t hi    = 1'000'000;

    for( int32_t v = lo; v <= (hi / 2); ++v ) {
        tree.insert( v );
        ASSERT_EQ( tree.size(), v - lo + 1 );
    }

    for( int32_t v = (hi / 2); v <= hi; ++ v) {
        tree.insert( v );
        ASSERT_EQ( tree.size(), v - lo + 1 );
    }

    ASSERT_EQ( tree.erase( hi + 1 ), 0 );
    ASSERT_EQ( tree.size(), hi - lo + 1 );
    ASSERT_EQ( tree.erase( hi ), 1 );
    ASSERT_EQ( tree.size(), hi - lo );

    tree.clear();
    ASSERT_EQ( tree.size(), 0 );
}

// Test insertion where simple left-left rotation takes place
TEST( Insert, insert_ll_basic ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 2 );
    tree.insert( 1 );
    tree.insert( 0 );

    ASSERT_EQ( balance_info::ll_count, 1 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 0 );
}

// Test insertion where simple left-right rotation takes place
TEST( Insert, insert_lr_basic ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 2 );
    tree.insert( 0 );
    tree.insert( 1 );

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 1 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 0 );
}

// Test insertion where simple right-left rotation takes place
TEST( Insert, insert_rl_basic ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 0 );
    tree.insert( 2 );
    tree.insert( 1 );

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 1 );
    ASSERT_EQ( balance_info::rr_count, 0 );
}

// Test insertion where simple right-right rotation takes place
TEST( Insert, insert_rr_basic ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 0 );
    tree.insert( 1 );
    tree.insert( 2 );

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 1 );
}

// Test insertion where left-left rotation takes place multiple times
TEST( Insert, insert_ll_compound ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    // first layer nodes
    tree.insert( 2 );
    tree.insert( 1 );
    tree.insert( 0 );

    // second LL balance called
    tree.insert( -1 );
    tree.insert( -2 );

    // third and fourth LL balance called
    tree.insert( -3 );
    tree.insert( -4 );

    ASSERT_EQ( balance_info::ll_count, 4 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 0 );
}

// Test insertion where left-left rotation takes place multiple times
TEST( Insert, insert_lr_compound ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    // first LR balance called
    tree.insert( 2 );
    tree.insert( 0 );
    tree.insert( 1 );

    // second LR balance called
    tree.insert( -2 );
    tree.insert( -1 );

    // third LR balance called
    // extra LL balance called at root
    tree.insert( -4 );
    tree.insert( -3 );

    ASSERT_EQ( balance_info::ll_count, 1 );
    ASSERT_EQ( balance_info::lr_count, 3 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 0 );;
}

// Test insertion where right-left rotation takes place multiple times
TEST( Insert, insert_rl_compound ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    // first RL balance called
    tree.insert( 0 );
    tree.insert( 2 );
    tree.insert( 1 );

    // second RL balance called
    tree.insert( 4 );
    tree.insert( 3 );

    // third RL balance called
    // extra RR balance called at root
    tree.insert( 6 );
    tree.insert( 5 );

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 3 );
    ASSERT_EQ( balance_info::rr_count, 1 );
}

// Test insertion where right-right rotation takes place multiple times
TEST( Insert, insert_rr_compound ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    // first RR balance called
    tree.insert( 0 );
    tree.insert( 1 );
    tree.insert( 2 );

    // second RR balance called
    tree.insert( 3 );
    tree.insert( 4 );

    // third and fourth RR balance called
    tree.insert( 5 );
    tree.insert( 6 );

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 4 );
}

// Test erasing a leaf node (with no children) with no ancestors
TEST( Erase, no_child_basic ) {

    avl_tree::AVL<int32_t> tree;

    tree.insert( 0 );
    ASSERT_EQ( tree.erase( 0 ), 1 );
}

// Test erasing a node with a right child, but no ancestors
TEST( Erase, right_child_basic ) {

    avl_tree::AVL<int32_t> tree;

    tree.insert( 0 );
    tree.insert( 1 );

    ASSERT_EQ( tree.erase( 0 ), 1 );
    ASSERT_EQ( tree.size(), 1 );
}

// Test erasing a node with a keft child, but no ancestors
TEST( Erase, left_child_basic ) {

    avl_tree::AVL<int32_t> tree;

    tree.insert( 0 );
    tree.insert( -1 );

    ASSERT_EQ( tree.erase( 0 ), 1 );
    ASSERT_EQ( tree.size(), 1 );
}

// Test erasing a node with both children, but no ancestors
TEST( Erase, both_child_basic ) {

    avl_tree::AVL<int32_t> tree;

    tree.insert( 0 );
    tree.insert( -1 );
    tree.insert( 1 );

    ASSERT_EQ( tree.erase( 0 ), 1 );
    ASSERT_EQ( tree.size(), 2 );
}

// Test erasing a node with no children, with ancestors, causing a left-left rotation
TEST( Erase, no_child_ll ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 7 );
    tree.insert( 3 );
    tree.insert( 11 );
    tree.insert( 1 );
    tree.insert( 5 );
    tree.insert( 9 );
    tree.insert( 13 );
    tree.insert( 0 );
    tree.insert( 2 );
    tree.insert( 4 );
    tree.insert( 6 );
    tree.insert( 8 );
    tree.insert( 10 );
    tree.insert( 12 );
    tree.insert( 14 );

    //                    7
    //
    //          3                    11
    //
    //    1          5          9        13
    //
    // 0     2    4    6    8    10   12   14


    tree.erase( 4 );
    tree.erase( 6 );
    tree.erase( 8 );
    tree.erase( 10 );
    tree.erase( 12 );
    tree.erase( 14 );

    tree.erase( 9 );
    tree.erase( 13 );

    //                    7
    //
    //          3                    11
    //
    //    1          5
    //
    // 0     2

    ASSERT_EQ( balance_info::ll_count, 1 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 0 );
}

// Test erasing a node with no children, with ancestors, causing a left-right rotation
TEST( Erase, no_child_lr ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 7 );
    tree.insert( 3 );
    tree.insert( 11 );
    tree.insert( 1 );
    tree.insert( 5 );
    tree.insert( 9 );
    tree.insert( 13 );
    tree.insert( 0 );
    tree.insert( 2 );
    tree.insert( 4 );
    tree.insert( 6 );
    tree.insert( 8 );
    tree.insert( 10 );
    tree.insert( 12 );
    tree.insert( 14 );

    //                    7
    //
    //          3                    11
    //
    //    1          5          9        13
    //
    // 0     2    4    6    8    10   12   14


    tree.erase( 0 );
    tree.erase( 2 );
    tree.erase( 8 );
    tree.erase( 10 );
    tree.erase( 12 );
    tree.erase( 14 );

    tree.erase( 9 );
    tree.erase( 13 );

    //                    7
    //
    //          3                    11
    //
    //    1          5
    //
    //            4     6

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 1 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 0 );
}

// Test erasing a node with no children, with ancestors causing a right-left rotation
TEST( Erase, no_child_rl ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 7 );
    tree.insert( 3 );
    tree.insert( 11 );
    tree.insert( 1 );
    tree.insert( 5 );
    tree.insert( 9 );
    tree.insert( 13 );
    tree.insert( 0 );
    tree.insert( 2 );
    tree.insert( 4 );
    tree.insert( 6 );
    tree.insert( 8 );
    tree.insert( 10 );
    tree.insert( 12 );
    tree.insert( 14 );

    //                    7
    //
    //          3                    11
    //
    //    1          5          9        13
    //
    // 0     2    4    6    8    10   12   14


    tree.erase( 0 );
    tree.erase( 2 );
    tree.erase( 4 );
    tree.erase( 6 );
    tree.erase( 12 );
    tree.erase( 14 );

    tree.erase( 1 );
    tree.erase( 5 );

    //                    7
    //
    //          3                    11
    //
    //                          9        13
    //
    //                      8    10

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 1 );
    ASSERT_EQ( balance_info::rr_count, 0 );
}

// Test erasing a node with no children, with ancestors causing a right-right rotation
TEST( Erase, no_child_rr ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 7 );
    tree.insert( 3 );
    tree.insert( 11 );
    tree.insert( 1 );
    tree.insert( 5 );
    tree.insert( 9 );
    tree.insert( 13 );
    tree.insert( 0 );
    tree.insert( 2 );
    tree.insert( 4 );
    tree.insert( 6 );
    tree.insert( 8 );
    tree.insert( 10 );
    tree.insert( 12 );
    tree.insert( 14 );

    //                    7
    //
    //          3                    11
    //
    //    1          5          9        13
    //
    // 0     2    4    6    8    10   12   14


    tree.erase( 0 );
    tree.erase( 2 );
    tree.erase( 4 );
    tree.erase( 6 );
    tree.erase( 8 );
    tree.erase( 10 );

    tree.erase( 1 );
    tree.erase( 5 );

    //                    7
    //
    //          3                    11
    //
    //                          9        13
    //
    //                                12   14

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 1 );
}

// Test erasing a node with a left child, with ancestors causing a right-left rotation
TEST( Erase, left_child_rl ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 2 );
    tree.insert( 1 );
    tree.insert( 6 );

    tree.insert( 0 );
    tree.insert( 4 );
    tree.insert( 8 );

    tree.insert( 3 );
    tree.insert( 5 );

    //                       2
    //          1                       6
    //    0           x           4           8
    // x     x     x     x     3     5     x     x

    tree.erase( 0 );

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 1 );
    ASSERT_EQ( balance_info::rr_count, 0 );
}

// Test erasing a node with a left child, with ancestors causing a right-right rotation
TEST( Erase, left_child_rr ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 2 );
    tree.insert( 1 );
    tree.insert( 6 );

    tree.insert( 0 );
    tree.insert( 4 );
    tree.insert( 8 );

    tree.insert( 7 );
    tree.insert( 9 );

    //                       2
    //          1                       6
    //    0           x           4           8
    // x     x     x     x     x     x     7     9

    tree.erase( 0 );

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 1 );
}

// Test erasing a node with a left child, with ancestors causing a left-left rotation
TEST( Erase, right_child_ll ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 7 );
    tree.insert( 3 );
    tree.insert( 8 );

    tree.insert( 1 );
    tree.insert( 5 );
    tree.insert( 9 );

    tree.insert( 0 );
    tree.insert( 2 );

    //                       7
    //          3                       8
    //    1           3           x           9
    // 0     2     x     x     x     x     x     x

    tree.erase( 9 );

    ASSERT_EQ( balance_info::ll_count, 1 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 0 );
}

// Test erasing a node with a left child, with ancestors causing a left-right rotation
TEST( Erase, right_child_lr ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 7 );
    tree.insert( 3 );
    tree.insert( 8 );

    tree.insert( 1 );
    tree.insert( 5 );
    tree.insert( 9 );

    tree.insert( 4 );
    tree.insert( 6 );

    //                       7
    //          3                       8
    //    1           3           x           9
    // x     x     4     6     x     x     x     x

    tree.erase( 9 );

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 1 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 0 );
}

TEST( Erase, both_child_find_min_basic ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 3 );

    tree.insert( 1 );
    tree.insert( 5 );

    tree.insert( 0 );
    tree.insert( 2 );
    tree.insert( 4 );
    tree.insert( 6 );

    tree.erase( 3 );

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 0 );
}

TEST( Erase, both_child_find_min_rl ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 3 );

    tree.insert( 1 );
    tree.insert( 5 );

    tree.insert( 0 );
    tree.insert( 2 );
    tree.insert( 4 );
    tree.insert( 7 );

    tree.insert( 6 );

    tree.erase( 3 );

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 1 );
    ASSERT_EQ( balance_info::rr_count, 0 );
}

TEST( Erase, both_child_find_min_rr ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 3 );

    tree.insert( 1 );
    tree.insert( 5 );

    tree.insert( 0 );
    tree.insert( 2 );
    tree.insert( 4 );
    tree.insert( 6 );

    tree.insert( 7 );

    tree.erase( 3 );

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 1 );
}

TEST( Erase, both_child_ll ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 5 );

    tree.insert( 2 );
    tree.insert( 7 );

    tree.insert( 1 );
    tree.insert( 4 );
    tree.insert( 6 );

    tree.insert( 0 );

    tree.erase( 5 );

    ASSERT_EQ( balance_info::ll_count, 1 );
    ASSERT_EQ( balance_info::lr_count, 0 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 0 );
}

TEST( Erase, both_child_lr ) {

    avl_tree::AVL<int32_t> tree;

    balance_info::init();

    tree.insert( 5 );

    tree.insert( 2 );
    tree.insert( 7 );

    tree.insert( 1 );
    tree.insert( 4 );
    tree.insert( 6 );

    tree.insert( 3 );

    tree.erase( 5 );

    ASSERT_EQ( balance_info::ll_count, 0 );
    ASSERT_EQ( balance_info::lr_count, 1 );
    ASSERT_EQ( balance_info::rl_count, 0 );
    ASSERT_EQ( balance_info::rr_count, 0 );
}

// Test simple forward iteration using forward iterators
TEST( Iteration, forward ) {

    avl_tree::AVL<int32_t> tree;

    constexpr int32_t lo      = 1;
    constexpr int32_t hi      = 1'000;

    auto end = tree.end();
    for( int32_t v = lo; v <= hi; ++v ) {
        tree.insert( v );
    }
    ASSERT_EQ( end, tree.end() );

    int32_t v = lo;
    for( auto it = tree.begin(); it != end; ++it, ++v ) {
        ASSERT_EQ( *it, v );
    }
}

// Test simple reverse iteration using reverse iterators
TEST( Iteration, reverse ) {

    avl_tree::AVL<int32_t> tree;

    constexpr int32_t lo      = 1;
    constexpr int32_t hi      = 1'000;

    auto end = tree.rend();
    for( int32_t v = hi; v >= lo; --v ) {
        tree.insert( v );
    }
    ASSERT_EQ( end, tree.rend() );

    int32_t v = hi;
    for( auto it = tree.rbegin(); it != end; ++it, --v ) {
        ASSERT_EQ( *it, v );
    }
}

// Test simple forward iteration using foreach loop
TEST( Iteration, for_each ) {

    avl_tree::AVL<int32_t> tree;

    constexpr int32_t lo      = 1;
    constexpr int32_t hi      = 1'000;

    for( int32_t v = lo; v <= hi; ++v ) {
        tree.insert( v );
    }

    int32_t v = lo;
    for( auto &e : tree ) {
        ASSERT_EQ( e, v );
        ++v;
    }
}

// Test all operators and edge cases for end iterator
TEST( Iteration, end_iterator_test ) {

    avl_tree::AVL<int32_t> tree;

    constexpr int32_t lo      = 1;
    constexpr int32_t hi      = 1'000;

    avl_tree::AVL<int32_t>::iterator it;
    avl_tree::AVL<int32_t>::iterator end = tree.end();

    // While Tree is empty
    it = tree.end();
    ++it;   ASSERT_EQ( it, end );
    it++;   ASSERT_EQ( it, end );
    --it;   ASSERT_EQ( it, end );
    it--;   ASSERT_EQ( it, end );

    tree.insert( lo );

    // While tree has one element
    it = tree.end();
    ++it;   ASSERT_EQ( it, end );
    it++;   ASSERT_EQ( it, end );
    --it;   ASSERT_EQ( *it, lo );
    it = tree.end();
    it--;   ASSERT_EQ( *it, lo );

    for( int32_t v = lo; v <= hi; ++v ) {
        tree.insert( v );
    }

    // While tree has many elements
    for( int32_t v = lo; v <= hi; ++v ) {
        it = tree.end();
        ++it;   ASSERT_EQ( it, end );
        it++;   ASSERT_EQ( it, end );
        --it;   ASSERT_EQ( *it, *(tree.rbegin()) );
        it = tree.end();
        it--;   ASSERT_EQ( *it, *(tree.rbegin()) );
    }
}

// Test all operators and edge cases for rend iterator
TEST( Iteration, rend_iterator_test ) {

    avl_tree::AVL<int32_t> tree;

    constexpr int32_t lo      = 1;
    constexpr int32_t hi      = 1'000;

    avl_tree::AVL<int32_t>::reverse_iterator it;
    avl_tree::AVL<int32_t>::reverse_iterator end = tree.rend();

    // While Tree is empty
    it = tree.rend();
    ++it;   ASSERT_EQ( it, end );
    it++;   ASSERT_EQ( it, end );
    --it;   ASSERT_EQ( it, end );
    it--;   ASSERT_EQ( it, end );

    tree.insert( lo );

    // While tree has one element
    it = tree.rend();
    ++it;   ASSERT_EQ( it, end );
    it++;   ASSERT_EQ( it, end );
    --it;   ASSERT_EQ( *it, lo );
    it = tree.rend();
    it--;   ASSERT_EQ( *it, lo );

    for( int32_t v = lo; v <= hi; ++v ) {
        tree.insert( v );
    }

    for( int32_t v = lo; v <= hi; ++v ) {
        it = tree.rend();
        ++it;   ASSERT_EQ( it, end );
        it++;   ASSERT_EQ( it, end );
        --it;   ASSERT_EQ( *it, *(tree.begin()) );
        it = tree.rend();
        it--;   ASSERT_EQ( *it, *(tree.begin()) );
    }
}

// Test all operators and edge cases for begin iterator
TEST( Iteration, begin_iterator_test ) {

    avl_tree::AVL<int32_t> tree;

    constexpr int32_t lo      = 1;
    constexpr int32_t hi      = 1'000;

    avl_tree::AVL<int32_t>::iterator it;

    // While tree is empty
    it = tree.begin();
    --it;   ASSERT_EQ( it, tree.begin() );
    it--;   ASSERT_EQ( it, tree.begin() );

    // While tree has one element
    tree.insert( lo );
    it = tree.begin();
    --it;   ASSERT_EQ( it, tree.begin() );
    it--;   ASSERT_EQ( it, tree.begin() );
    ++it;   ASSERT_EQ( it, tree.end() );
    it = tree.begin();
    it++;   ASSERT_EQ( it, tree.end() );

    // While tree is not empty
    for( int32_t v = lo; v <= hi; ++v ) {
        tree.insert( v );
    }

    it = tree.begin();
    --it;   ASSERT_EQ( it, tree.begin() );
    it--;   ASSERT_EQ( it, tree.begin() );

    if constexpr( hi - lo + 1 >= 2 ) {

        ++it;   ASSERT_EQ( *it, lo + 1 );
        it = tree.begin();
        it++;   ASSERT_EQ( *it, lo + 1 );
    }
    else {

        ++it;   ASSERT_EQ( it, tree.end() );
        it = tree.begin();
        it++;   ASSERT_EQ( it, tree.end() );
    }
}

// Test all operators and edge cases for rbegin iterator
TEST( Iteration, rbegin_iterator_test ) {

    avl_tree::AVL<int32_t> tree;

    constexpr int32_t lo      = 1;
    constexpr int32_t hi      = 1'000;

    avl_tree::AVL<int32_t>::reverse_iterator it;

    // While tree is empty
    it = tree.rbegin();
    --it;   ASSERT_EQ( it, tree.rbegin() );
    it--;   ASSERT_EQ( it, tree.rbegin() );

    // While tree has one element
    tree.insert( lo );
    it = tree.rbegin();
    --it;   ASSERT_EQ( it, tree.rbegin() );
    it--;   ASSERT_EQ( it, tree.rbegin() );
    ++it;   ASSERT_EQ( it, tree.rend() );
    it = tree.rbegin();
    it++;   ASSERT_EQ( it, tree.rend() );

    // While tree is not empty
    for( int32_t v = lo; v <= hi; ++v ) {
        tree.insert( v );
    }

    it = tree.rbegin();
    --it;   ASSERT_EQ( it, tree.rbegin() );
    it--;   ASSERT_EQ( it, tree.rbegin() );

    if constexpr( hi - lo + 1 >= 2 ) {

        ++it;   ASSERT_EQ( *it, hi - 1 );
        it = tree.rbegin();
        it++;   ASSERT_EQ( *it, hi - 1 );
    }
    else {

        ++it;   ASSERT_EQ( it, tree.rend() );
        it = tree.rbegin();
        it++;   ASSERT_EQ( it, tree.rend() );
    }
}

// The find method returns an iterator an exact match of the given element (end() on failure)
TEST( BinarySearch, find_equal_strict_test ) {

    avl_tree::AVL<int32_t> tree;

    constexpr int32_t lo      = 1;
    constexpr int32_t hi      = 1'000;

    for( int32_t v = lo; v <= hi; ++v ) {
        ASSERT_EQ( tree.find( v ), tree.end() );
    }

    for( int32_t v = lo; v <= hi; ++v ) {
        tree.insert( v );
    }

    for( int32_t v = lo; v <= hi; ++v ) {
        ASSERT_EQ( *(tree.find( v )), v );
    }
}

// The first_greater_strict method returns an iterator to the smallest element strictly greater
// than the given element (end() on failure)
TEST( BinarySearch, find_greater_strict_test ) {

    avl_tree::AVL<int32_t> tree;

    constexpr int32_t lo      = 1;
    constexpr int32_t hi      = 1'000;

    for( int32_t v = lo; v <= hi; ++v ) {
        ASSERT_EQ( tree.first_greater_strict( v ), tree.end() );
    }

    for( int32_t v = lo; v <= hi; ++v ) {
        tree.insert( v );
    }

    for( int32_t v = lo; v < hi; ++v ) {
        ASSERT_EQ( *(tree.first_greater_strict( v )), v + 1 );
    }
    ASSERT_EQ( tree.first_greater_strict( hi ), tree.end() );
}

// The first_greater_equals method returns an iterator to the smallest element greater than or
// equal to the given element (end() on failure)
TEST( BinarySearch, find_greater_equals_test ) {

    avl_tree::AVL<int32_t> tree;

    constexpr int32_t lo      = 1;
    constexpr int32_t hi      = 1'000;

    for( int32_t v = lo; v <= hi; ++v ) {
        ASSERT_EQ( tree.first_greater_equals( v ), tree.end() );
    }

    for( int32_t v = lo; v <= hi; ++v ) {
        if( v % 2 )
            tree.insert( v );
    }

    for( int32_t v = lo; v < hi; ++v ) {

        if( v % 2 ) {
            ASSERT_EQ( *(tree.first_greater_equals( v )), v );
        }
        else {
            ASSERT_EQ( *(tree.first_greater_equals( v )), v + 1 );
        }
    }

    if constexpr( hi % 2 ) {
        ASSERT_EQ( *(tree.first_greater_equals( hi )), hi );
    }
    else {
        ASSERT_EQ( tree.first_greater_equals( hi ), tree.end() );
    }
}

// The last_smaller_strict method returns an iterator to the largest element strictly less than the
// the given element (end() on failure)
TEST( BinarySearch, find_less_strict_test ) {

    avl_tree::AVL<int32_t> tree;

    constexpr int32_t lo      = 1;
    constexpr int32_t hi      = 1'000;

    for( int32_t v = hi; v >= lo; --v ) {
        ASSERT_EQ( tree.last_smaller_strict( v ), tree.end() );
    }

    for( int32_t v = hi; v >= lo; --v ) {
        tree.insert( v );
    }

    for( int32_t v = hi; v > lo; --v ) {
        ASSERT_EQ( *(tree.last_smaller_strict( v )), v - 1 );
    }
    ASSERT_EQ( tree.last_smaller_strict( lo ), tree.end() );
}

// The last_smaller_equals method returns an iterator to the largest element less than or equal to
// the given element (end() on failure)
TEST( BinarySearch, find_less_equals_test ) {

    avl_tree::AVL<int32_t> tree;

    constexpr int32_t lo      = 1;
    constexpr int32_t hi      = 1'000;

    for( int32_t v = hi; v >= lo; --v ) {
        ASSERT_EQ( tree.last_smaller_equals( v ), tree.end() );
    }

    for( int32_t v = hi; v >= lo; --v ) {
        if( v % 2 )
            tree.insert( v );
    }

    for( int32_t v = hi; v > lo; --v ) {

        if( v % 2 ) {
            ASSERT_EQ( *(tree.last_smaller_equals( v )), v );
        }
        else {
            ASSERT_EQ( *(tree.last_smaller_equals( v )), v - 1 );
        }
    }

    if constexpr( lo % 2 ) {
        ASSERT_EQ( *(tree.last_smaller_equals( lo )), lo );
    }
    else {
        ASSERT_EQ( tree.last_smaller_equals( lo ), tree.end() );
    }
}

// Test with std::string
TEST( DataTypes, cpp_string ) {

    avl_tree::AVL<std::string> tree;

    tree.insert( "AVL" );
    tree.insert( "Trees" );
    tree.insert( "are" );
    tree.insert( "very" );
    tree.insert( "useful!" );
    tree.insert( "useful!" );

    ASSERT_EQ( tree.size(), 5 );

    auto it1 = tree.begin();
    auto it2 = tree.begin(); ++it2;

    for( ; it2 != tree.end() ; ++it1, ++it2 ) {
        ASSERT_LT( *it1, *it2 );
    }
}

bool comp_lt( const char * const & a, const char * const & b ) {
    return strcmp( a, b ) < 0;
}

// Test with C-style string
TEST( DataTypes, c_string ) {

    avl_tree::AVL<const char *, comp_lt> tree;

    tree.insert( "AVL" );
    tree.insert( "Trees" );
    tree.insert( "are" );
    tree.insert( "very" );
    tree.insert( "useful!" );
    tree.insert( "useful!" );

    ASSERT_EQ( tree.size(), 5 );

    auto it1 = tree.begin();
    auto it2 = tree.begin(); ++it2;

    for( ; it2 != tree.end() ; ++it1, ++it2 ) {
        ASSERT_LT( strcmp( *it1, *it2 ), 0 );
    }
}
