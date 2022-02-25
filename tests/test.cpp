#include <cstdlib>
#include <functional>

#include <gtest/gtest.h>

#define AVL_TEST_MODE                   // to be able to access private members and add extra diagnostic info collection

#include "../avl.h"

#define ASSERT_ROTATIONS(tree, a, b, c, d)       \
    ASSERT_EQ (tree.dbg_info.ll_count, a);      \
    ASSERT_EQ (tree.dbg_info.lr_count, b);      \
    ASSERT_EQ (tree.dbg_info.rl_count, c);      \
    ASSERT_EQ (tree.dbg_info.rr_count, d);      \

/**
 * @brief                       Inserts all given elements into a tree, in the same order
 * @note                        This is a utility function to improve readability where a lot of contiguous inserts are required
 *
 * @param pTree                 Reference to the tree in which to insert the values
 * @param pValues               The values to insert
 */
template <typename tree_t, typename T1, typename... T2>
void
insert (tree_t & pTree, T1 pValue, T2... pValues)
{
    pTree.insert (pValue);

    if constexpr (sizeof...(pValues) != 0) {
        insert (pTree, pValues...);
    }
}

/**
 * @brief                       Erases all given elements from a tree, in the given order
 * @note                        This is a utility function to improve readability where a lot of contiguous erases are required
 *
 * @param pTree                 Reference to the tree in which to insert the values
 * @param pValues               The values to erase
 */
template <typename tree_t, typename T1, typename... T2>
void
erase (tree_t & pTree, T1 pValue, T2... pValues)
{
    pTree.erase (pValue);

    if constexpr (sizeof...(pValues) != 0) {
        erase (pTree, pValues...);
    }
}

/**
 * @brief   Test basic functionality of tree
 *
 */
TEST (Smoke, smoke_test)
{
    AgAVLTree<int32_t> tree;

    constexpr int32_t      lo {-1'000'000};
    constexpr int32_t      hi {1'000'000};

    for (int32_t v = lo; v <= (hi / 2); ++v) {
        tree.insert (v);
        ASSERT_EQ (tree.size (), v - lo + 1);
    }

    for (int32_t v = (hi / 2); v <= hi; ++v) {
        tree.insert (v);
        ASSERT_EQ (tree.size (), v - lo + 1);
    }

    ASSERT_EQ (tree.erase (hi + 1), 0);
    ASSERT_EQ (tree.size (), hi - lo + 1);
    ASSERT_EQ (tree.erase (hi), 1);
    ASSERT_EQ (tree.size (), hi - lo);

    tree.clear ();
    ASSERT_EQ (tree.size (), 0);
}

/**
 * @brief   Test simple insertion with left-left rotation
 * @note    Produce the following tree
 *
 *                 2
 *                   1
 *                     0
 *
 *          left-left rotation takes place at the root to form the tree
 *
 *                 1
 *               0   2
 *
 */
TEST (Insert, insert_ll_simple)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 2, 1, 0);
    ASSERT_ROTATIONS (tree, 1, 0, 0, 0);
}

/**
 * @brief   Test simple insertion with left-right rotation
 * @note    Produce the following tree
 *
 *                 2
 *             0
 *               1
 *
 *          left-right rotation takes place at the root to form the tree
 *
 *                 1
 *               0   2
 *
 */
TEST (Insert, insert_lr_simple)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 2, 0, 1);
    ASSERT_ROTATIONS (tree, 0, 1, 0, 0);
}

/**
 * @brief   Test simple insertion with right-left rotation
 * @note    Produce the following tree
 *
 *                 0
 *                     2
 *                   1
 *
 *          right-left rotation takes place at the root to form the tree
 *
 *                 1
 *               0   2
 *
 */
TEST (Insert, insert_rl_simple)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 0, 2, 1);
    ASSERT_ROTATIONS (tree, 0, 0, 1, 0);
}

/**
 * @brief   Test simple insertion with right-left rotation
 * @note    Produce the following tree
 *
 *                 0
 *               1
 *             2
 *
 *          right-left rotation takes place at the root to form the tree
 *
 *                 1
 *               0   2
 *
 */
TEST (Insert, insert_rr_simple)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 0, 1, 2);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 1);
}

/**
 * @brief   Test insertion with multiple left-left rotations
 *
 *  @note   Multiple rotations are intended to take place after the final insertion
 *          This is intended
 */
TEST (Insert, insert_ll_compound)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 2, 1, 0);                           // first left-left rotation
    ASSERT_ROTATIONS (tree, 1, 0, 0, 0);                    //

    insert (tree, -1, -2);                            // second left-left rotation
    ASSERT_ROTATIONS (tree, 2, 0, 0, 0);                    //

    insert (tree, -3, -4);                            // third and fourth left-left rotations
    ASSERT_ROTATIONS (tree, 4, 0, 0, 0);                    //
}

/**
 * @brief   Test insertion with multiple left-right rotations
 *
 *  @note   Multiple rotations are intended to take place after the final insertion
 *          This is intended
 */
TEST (Insert, insert_lr_compound)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 2, 0, 1);                           // first left-right rotation
    ASSERT_ROTATIONS (tree, 0, 1, 0, 0);                    //

    insert (tree, -2, -1);                            // second left-right rotation
    ASSERT_ROTATIONS (tree, 0, 2, 0, 0);                    //

    insert (tree, -4, -3);                            // third left-right, first left-left rotations
    ASSERT_ROTATIONS (tree, 1, 3, 0, 0);                    //
}

/**
 * @brief   Test insertion with multiple right-left rotations
 *
 *  @note   Multiple rotations are intended to take place after the final insertion
 *          This is intended
 */
TEST (Insert, insert_rl_compound)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 0, 2, 1);                           // first right-left rotation
    ASSERT_ROTATIONS (tree, 0, 0, 1, 0);                    //

    insert (tree, 4, 3);                              // second right-left rotiation
    ASSERT_ROTATIONS (tree, 0, 0, 2, 0);                    //

    insert (tree, 6, 5);                              // third right-left, first right-right rotation
    ASSERT_ROTATIONS (tree, 0, 0, 3, 1);                    //
}

/**
 * @brief   Test insertion with multiple right-right rotations
 *
 *  @note   Multiple rotations are intended to take place after the final insertion
 *          This is intended
 */
TEST (Insert, insert_rr_compound)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 0, 1, 2);                           // first right-right rotation
    ASSERT_ROTATIONS (tree, 0, 0, 0, 1);                    //

    insert (tree, 3, 4);                              // second right-right rotation
    ASSERT_ROTATIONS (tree, 0, 0, 0, 2);                    //

    insert (tree, 5, 6);                    // third and fourth right-right rotation
    ASSERT_ROTATIONS (tree, 0, 0, 0, 4);
}

/**
 * @brief   Test erasing an ancestorless node without chidren (simple case)
 *
 */
TEST (Erase, no_child_simple)
{
    AgAVLTree<int32_t> tree;

    tree.insert (0);
    ASSERT_EQ (tree.erase (0), 1);
}

/**
 * @brief   Test erasing an ancestorless node with a right child only (simple case)
 *
 */
TEST (Erase, left_child_simple)
{
    AgAVLTree<int32_t> tree;

    tree.insert (0);
    tree.insert (-1);

    ASSERT_EQ (tree.erase (0), 1);
    ASSERT_EQ (tree.size (), 1);
}

/**
 * @brief   Test erasing an ancestorless node with a right child only (simple case)
 *
 */
TEST (Erase, right_child_simple)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 0, 1);

    ASSERT_EQ (tree.erase (0), 1);
    ASSERT_EQ (tree.size (), 1);
}

/**
 * @brief   Test erasing an ancestorless node with both children (simple case)
 *
 */
TEST (Erase, both_child_simple)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 0, -1, 1);

    ASSERT_EQ (tree.erase (0), 1);
    ASSERT_EQ (tree.size (), 2);
}

/**
 * @brief   Test erasing a node with no children, but having an ancestor become left-left heavy
 *
 */
TEST (Erase, no_child_ll)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 7);
    insert (tree, 3, 11);
    insert (tree, 1, 5, 9, 13);
    insert (tree, 0, 2, 4, 6, 8, 10, 12, 14);
    ASSERT_EQ (tree.size (), 15);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    // Expected shape of tree after insertions (balanced)
    //                    7
    //
    //          3                    11
    //
    //    1          5          9        13
    //
    // 0     2    4    6    8    10   12   14

    erase (tree, 4, 6, 8, 10, 12, 14);
    ASSERT_EQ (tree.size (), 9);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    // Expected shape of tree after deletions (balanced)
    //                    7
    //
    //          3                    11
    //
    //    1          5          9        13
    //
    // 0     2

    erase (tree, 9, 13);
    ASSERT_EQ (tree.size (), 7);
    ASSERT_ROTATIONS (tree, 1, 0, 0, 0);

    // Expected shape of tree after deletions (left-left imbalance at node 7)
    //                    7
    //
    //          3                    11
    //
    //    1          5
    //
    // 0     2
}

/**
 * @brief   Test erasing a node with no children, but having an ancestor become left-right heavy
 *
 */
TEST (Erase, no_child_lr)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 7);
    insert (tree, 3, 11);
    insert (tree, 1, 5, 9, 13);
    insert (tree, 0, 2, 4, 6, 8, 10, 12, 14);
    ASSERT_EQ (tree.size (), 15);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    // Expected shape of tree after insertions (balanced)
    //                    7
    //
    //          3                    11
    //
    //    1          5          9        13
    //
    // 0     2    4    6    8    10   12   14

    erase (tree, 0, 2, 8, 10, 12, 14);
    ASSERT_EQ (tree.size (), 9);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    // Expected shape of tree after deletions (balanced)
    //                    7
    //
    //          3                    11
    //
    //    1          5          9        13
    //
    //            4    6

    erase (tree, 9, 13);
    ASSERT_EQ (tree.size (), 7);
    ASSERT_ROTATIONS (tree, 0, 1, 0, 0);

    // Expected shape of tree after deletions (left-right imbalance at node 7)
    //                    7
    //
    //          3                    11
    //
    //    1          5
    //
    //            4     6
}

/**
 * @brief   Test erasing a node with no children, but having an ancestor become right-left heavy
 *
 */
TEST (Erase, no_child_rl)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 7);
    insert (tree, 3, 11);
    insert (tree, 1, 5, 9, 13);
    insert (tree, 0, 2, 4, 6, 8, 10, 12, 14);
    ASSERT_EQ (tree.size (), 15);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    // Expected shape of tree after insertions (balanced)
    //                    7
    //
    //          3                    11
    //
    //    1          5          9        13
    //
    // 0     2    4    6    8    10   12   14

    erase (tree, 0, 2, 4, 6, 12, 14);
    ASSERT_EQ (tree.size (), 9);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    // Expected shape of tree after deletions (balanced)
    //                    7
    //
    //          3                    11
    //
    //    1          5          9        13
    //
    //                      8    10

    erase (tree, 1, 5);
    ASSERT_EQ (tree.size (), 7);
    ASSERT_ROTATIONS (tree, 0, 0, 1, 0);

    // Expected shape of tree after insertions (right-left imbalance at node 7)
    //                    7
    //
    //          3                    11
    //
    //                          9        13
    //
    //                      8    10
}

/**
 * @brief   Test erasing a node with no children, but having an ancestor become right-right heavy
 *
 */
TEST (Erase, no_child_rr)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 7);
    insert (tree, 3, 11);
    insert (tree, 1, 5, 9, 13);
    insert (tree, 0, 2, 4, 6, 8, 10, 12, 14);
    ASSERT_EQ (tree.size (), 15);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    // Expected shape of tree after insertions (balanced)
    //                    7
    //
    //          3                    11
    //
    //    1          5          9        13
    //
    // 0     2    4    6    8    10   12   14

    erase (tree, 0, 2, 4, 6, 8, 10);
    ASSERT_EQ (tree.size (), 9);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    // Expected shape of tree after insertions (balanced)
    //                    7
    //
    //          3                    11
    //
    //    1          5          9        13
    //
    //                                12   14

    erase (tree, 1, 5);
    ASSERT_EQ (tree.size (), 7);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 1);

    // Expected shape of tree after deletions (right-right imbalance at node 7)
    //                    7
    //
    //          3                    11
    //
    //                          9        13
    //
    //                                12   14
}

/**
 * @brief   Test erasing a node with a left child, but having an ancestor become right-left heavy
 *
 */
TEST (Erase, left_child_rl)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 2, 1, 6);
    insert (tree, 0, 4, 8);
    insert (tree, 3, 5);
    ASSERT_EQ (tree.size (), 8);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    // expected shape of tree after insertions (balanced)
    //                       2
    //          1                       6
    //    0                       4           8
    //                         3     5

    tree.erase (0);
    ASSERT_EQ (tree.size (), 7);
    ASSERT_ROTATIONS (tree, 0, 0, 1, 0);

    // expected shape of tree after deletion (right-left imbalance at node 2)
    //                       2
    //          1                       6
    //                            4           8
    //                         3     5
}

/**
 * @brief   Test erasing a node with a left child, but having an ancestor become right-right heavy
 *
 */
TEST (Erase, left_child_rr)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 2, 1, 6);
    insert (tree, 0, 4, 8);
    insert (tree, 7, 9);
    ASSERT_EQ (tree.size (), 8);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    // Expected shape of tree after insertions (balanced)
    //                       2
    //          1                       6
    //    0                       4           8
    //                                     7     9

    tree.erase (0);
    ASSERT_EQ (tree.size (), 7);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 1);

    // Expected shape of tree after insertions (right-right imbalance at node 2)
    //                       2
    //          1                       6
    //    0                       4           8
    //                                     7     9
}

/**
 * @brief   Test erasing a node with a right child, but having an ancestor become left-left heavy
 *
 */
TEST (Erase, right_child_ll)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 7, 3, 8);
    insert (tree, 1, 5, 9);
    insert (tree, 0, 2);
    ASSERT_EQ (tree.size (), 8);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    // Expected shape of tree after insertions (balanced)
    //                       7
    //          3                       8
    //    1           3                       9
    // 0     2

    tree.erase (9);
    ASSERT_EQ (tree.size (), 7);
    ASSERT_ROTATIONS (tree, 1, 0, 0, 0);

    // Expected shape of tree after deletions (left-left imbalance at node 7)
    //                       7
    //          3                       8
    //    1           3
    // 0     2
}

/**
 * @brief   Test erasing a node with a right child, but having an ancestor become left-right heavy
 *
 */
TEST (Erase, right_child_lr)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 7, 3, 8);
    insert (tree, 1, 5, 9);
    insert (tree, 4, 6);
    ASSERT_EQ (tree.size (), 8);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    // Expected shape of tree after insertions (balanced)
    //                       7
    //          3                       8
    //    1           3                       9
    //             4     6

    tree.erase (9);
    ASSERT_EQ (tree.size (), 7);
    ASSERT_ROTATIONS (tree, 0, 1, 0, 0);

    // Expected shape of tree after deletions (left-right imbalance at node 7)
    //                       7
    //          3                       8
    //    1           3
    //             4     6
}

/**
 * @brief   Test erasing a node with both children, but not resulting in any rotations
 *
 */
TEST (Erase, both_child_find_min_basic)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 3);
    insert (tree, 1, 5);
    insert (tree, 0, 2, 4, 6);
    ASSERT_EQ (tree.size (), 7);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    tree.erase (3);
    ASSERT_EQ (tree.size (), 6);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
}

/**
 * @brief   Test erasing a node with both children, and resulting in a right-left rotation
 *
 */
TEST (Erase, both_child_find_min_rl)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 3);
    insert (tree, 1, 5);
    insert (tree, 0, 2, 4, 7);
    insert (tree, 6);
    ASSERT_EQ (tree.size (), 8);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    tree.erase (3);
    ASSERT_EQ (tree.size (), 7);
    ASSERT_ROTATIONS (tree, 0, 0, 1, 0);
}

/**
 * @brief   Test erasing a node with both children, and resulting in a right-right rotation
 *
 */
TEST (Erase, both_child_find_min_rr)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 3);
    insert (tree, 1, 5);
    insert (tree, 0, 2, 4, 6);
    insert (tree, 7);
    ASSERT_EQ (tree.size (), 8);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    tree.erase (3);
    ASSERT_EQ (tree.size (), 7);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 1);
}

/**
 * @brief   Test erasing a node with both children, and resulting in a left-left rotation (at the position of the deleted node)
 *
 */
TEST (Erase, both_child_ll)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 4);
    insert (tree, 2, 6);
    insert (tree, 1, 3, 5);
    insert (tree, 0);
    ASSERT_EQ (tree.size (), 7);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    tree.erase (4);
    ASSERT_EQ (tree.size (), 6);
    ASSERT_ROTATIONS (tree, 1, 0, 0, 0);
}

/**
 * @brief   Test erasing a node with both children, and resulting in a left-right rotation (at the position of the deleted node)
 *
 */
TEST (Erase, both_child_lr)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 5);
    insert (tree, 2, 7);
    insert (tree, 1, 4, 6);
    insert (tree, 3);
    ASSERT_EQ (tree.size (), 7);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);

    tree.erase (5);
    ASSERT_EQ (tree.size (), 6);
    ASSERT_ROTATIONS (tree, 0, 1, 0, 0);
}

/**
 * @brief   Test simple forward iteration
 *
 */
TEST (Iteration, forward)
{
    constexpr int32_t       lo      {1};
    constexpr int32_t       hi      {1000};

    AgAVLTree<int32_t>      tree;

    auto                    end     = tree.end ();

    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }
    ASSERT_EQ (end, tree.end ());

    int32_t v = lo;
    for (auto it = tree.begin (); it != end; ++it, ++v) {
        ASSERT_EQ (*it, v);
    }
}

/**
 * @brief   Test simple reverse iteration
 *
 */
TEST (Iteration, reverse)
{

    AgAVLTree<int32_t> tree;

    constexpr int32_t      lo  = 1;
    constexpr int32_t      hi  = 1'000;

    auto                   end = tree.rend ();
    for (int32_t v = hi; v >= lo; --v) {
        tree.insert (v);
    }
    ASSERT_EQ (end, tree.rend ());

    int32_t v = hi;
    for (auto it = tree.rbegin (); it != end; ++it, --v) {
        ASSERT_EQ (*it, v);
    }
}

/**
 * @brief   Test C++ style for each loop
 *
 */
TEST (Iteration, for_each)
{
    constexpr int32_t      lo {1};
    constexpr int32_t      hi {1000};

    AgAVLTree<int32_t> tree;

    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }

    int32_t v = lo;
    for (auto &e : tree) {
        ASSERT_EQ (e, v);
        ++v;
    }
}

/**
 * @brief   Test for operations on end() iterator
 *
 */
TEST (Iteration, end_iterator_test)
{
    constexpr int32_t               lo      {1};
    constexpr int32_t               hi      {1000};

    AgAVLTree<int32_t>              tree;

    AgAVLTree<int32_t>::iterator    it;
    AgAVLTree<int32_t>::iterator    end     = tree.end ();

    // While Tree is empty
    it      = tree.end ();
    ++it;
    ASSERT_EQ (it, end);
    it++;
    ASSERT_EQ (it, end);
    --it;
    ASSERT_EQ (it, end);
    it--;
    ASSERT_EQ (it, end);

    tree.insert (lo);

    // While tree has one element
    it      = tree.end ();
    ++it;
    ASSERT_EQ (it, end);
    it++;
    ASSERT_EQ (it, end);
    --it;
    ASSERT_EQ (*it, lo);
    it      = tree.end ();
    it--;
    ASSERT_EQ (*it, lo);

    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }

    // While tree has many elements
    for (int32_t v = lo; v <= hi; ++v) {
        it  = tree.end ();
        ++it;
        ASSERT_EQ (it, end);
        it++;
        ASSERT_EQ (it, end);
        --it;
        ASSERT_EQ (*it, *(tree.rbegin ()));
        it = tree.end ();
        it--;
        ASSERT_EQ (*it, *(tree.rbegin ()));
    }
}

/**
 * @brief   Test for operations on rend() iterator
 *
 */
TEST (Iteration, rend_iterator_test)
{
    constexpr int32_t                       lo      {1};
    constexpr int32_t                       hi      {1000};

    AgAVLTree<int32_t>                      tree;

    AgAVLTree<int32_t>::reverse_iterator    it;
    AgAVLTree<int32_t>::reverse_iterator    end     = tree.rend ();

    // While Tree is empty
    it      = tree.rend ();
    ++it;
    ASSERT_EQ (it, end);
    it++;
    ASSERT_EQ (it, end);
    --it;
    ASSERT_EQ (it, end);
    it--;
    ASSERT_EQ (it, end);

    tree.insert (lo);

    // While tree has one element
    it      = tree.rend ();
    ++it;
    ASSERT_EQ (it, end);
    it++;
    ASSERT_EQ (it, end);
    --it;
    ASSERT_EQ (*it, lo);
    it      = tree.rend ();
    it--;
    ASSERT_EQ (*it, lo);

    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }

    for (int32_t v = lo; v <= hi; ++v) {
        it  = tree.rend ();
        ++it;
        ASSERT_EQ (it, end);
        it++;
        ASSERT_EQ (it, end);
        --it;
        ASSERT_EQ (*it, *(tree.begin ()));
        it  = tree.rend ();
        it--;
        ASSERT_EQ (*it, *(tree.begin ()));
    }
}

/**
 * @brief   Test all operation on begin() iterator
 *
 */
TEST (Iteration, begin_iterator_test)
{
    constexpr int32_t               lo      {1};
    constexpr int32_t               hi      {1000};

    AgAVLTree<int32_t>              tree;

    AgAVLTree<int32_t>::iterator    it;

    // While tree is empty
    it      = tree.begin ();
    --it;
    ASSERT_EQ (it, tree.begin ());
    it--;
    ASSERT_EQ (it, tree.begin ());

    // While tree has one element
    tree.insert (lo);
    it      = tree.begin ();
    --it;
    ASSERT_EQ (it, tree.begin ());
    it--;
    ASSERT_EQ (it, tree.begin ());
    ++it;
    ASSERT_EQ (it, tree.end ());
    it      = tree.begin ();
    it++;
    ASSERT_EQ (it, tree.end ());

    // While tree is not empty
    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }

    it      = tree.begin ();
    --it;
    ASSERT_EQ (it, tree.begin ());
    it--;
    ASSERT_EQ (it, tree.begin ());

    if constexpr (hi - lo + 1 >= 2) {

        ++it;
        ASSERT_EQ (*it, lo + 1);
        it  = tree.begin ();
        it++;
        ASSERT_EQ (*it, lo + 1);
    } else {

        ++it;
        ASSERT_EQ (it, tree.end ());
        it  = tree.begin ();
        it++;
        ASSERT_EQ (it, tree.end ());
    }
}

/**
 * @brief   Test all operation on rbegin() iterator
 *
 */
TEST (Iteration, rbegin_iterator_test)
{
    constexpr int32_t                       lo  {1};
    constexpr int32_t                       hi  {1000};

    AgAVLTree<int32_t>                      tree;

    AgAVLTree<int32_t>::reverse_iterator    it;

    // While tree is empty
    it      = tree.rbegin ();
    --it;
    ASSERT_EQ (it, tree.rbegin ());
    it--;
    ASSERT_EQ (it, tree.rbegin ());

    // While tree has one element
    tree.insert (lo);
    it      = tree.rbegin ();
    --it;
    ASSERT_EQ (it, tree.rbegin ());
    it--;
    ASSERT_EQ (it, tree.rbegin ());
    ++it;
    ASSERT_EQ (it, tree.rend ());
    it      = tree.rbegin ();
    it++;
    ASSERT_EQ (it, tree.rend ());

    // While tree is not empty
    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }

    it      = tree.rbegin ();
    --it;
    ASSERT_EQ (it, tree.rbegin ());
    it--;
    ASSERT_EQ (it, tree.rbegin ());

    if constexpr (hi - lo + 1 >= 2) {

        ++it;
        ASSERT_EQ (*it, hi - 1);
        it  = tree.rbegin ();
        it++;
        ASSERT_EQ (*it, hi - 1);
    }

    else {

        ++it;
        ASSERT_EQ (it, tree.rend ());
        it  = tree.rbegin ();
        it++;
        ASSERT_EQ (it, tree.rend ());
    }
}

/**
 * @brief   Test for equality comparisons on iterators
 *
 */
TEST (Iteration, equality_test)
{

    AgAVLTree<int32_t>  tree1;
    AgAVLTree<int32_t>  tree2;

    tree1.insert (0);
    tree1.insert (1);

    tree2.insert (0);
    tree2.insert (1);

    auto                it1         = tree1.begin ();
    auto                it1_cpy     = tree1.begin ();
    auto                it2         = tree2.begin ();
    auto                it2_cpy     = tree2.begin ();

    // same tree, same node
    ASSERT_EQ (it1, it1_cpy);
    ASSERT_EQ (it2, it2_cpy);

    // different tree same value node
    ASSERT_NE (it1, it2);

    // same tree, different node
    ++it1;
    it2++;
    ASSERT_NE (it1, it1_cpy);
    ASSERT_NE (it2, it2_cpy);

    // different tree, different value node
    ASSERT_NE (it1_cpy, it2);
}

/**
 * @brief   Test for equality comparisons on reverse iterators
 *
 */
TEST (Iteration, reverse_equality_test)
{

    AgAVLTree<int32_t>  tree1;
    AgAVLTree<int32_t>  tree2;

    tree1.insert (0);
    tree1.insert (1);

    tree2.insert (0);
    tree2.insert (1);

    auto                it1     = tree1.rbegin ();
    auto                it1_cpy = tree1.rbegin ();
    auto                it2     = tree2.rbegin ();
    auto                it2_cpy = tree2.rbegin ();

    // same tree, same node
    ASSERT_EQ (it1, it1_cpy);
    ASSERT_EQ (it2, it2_cpy);

    // different tree same value node
    ASSERT_NE (it1, it2);

    // same tree, different node
    ++it1;
    it2++;
    ASSERT_NE (it1, it1_cpy);
    ASSERT_NE (it2, it2_cpy);

    // different tree, different value node
    ASSERT_NE (it1_cpy, it2);
}

/**
 * @brief   Test all variations of find (strictly matching element)
 *
 */
TEST (Find, find_equal_strict_test)
{
    constexpr int32_t   lo      {1};
    constexpr int32_t   hi      {1000};

    AgAVLTree<int32_t>  tree;

    for (int32_t v = lo; v <= hi; ++v) {
        ASSERT_EQ (tree.find (v), tree.end ());
    }

    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }

    for (int32_t v = lo; v <= hi; ++v) {
        ASSERT_EQ (*(tree.find (v)), v);
    }
}

/**
 * @brief   Test all variations of first_greater_strict (smallest, strictly greater element)
 *
 */
TEST (Find, find_greater_strict_test)
{
    constexpr int32_t   lo      {1};
    constexpr int32_t   hi      {1000};

    AgAVLTree<int32_t>  tree;

    for (int32_t v = lo; v <= hi; ++v) {
        ASSERT_EQ (tree.first_greater_strict (v), tree.end ());
    }

    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }

    for (int32_t v = lo; v < hi; ++v) {
        ASSERT_EQ (*(tree.first_greater_strict (v)), v + 1);
    }
    ASSERT_EQ (tree.first_greater_strict (hi), tree.end ());
}

/**
 * @brief   Test all variations of first_greater_equals (smallest, greater or equal element)
 *
 */
TEST (Find, find_greater_equals_test)
{
    constexpr int32_t   lo      {1};
    constexpr int32_t   hi      {1000};

    AgAVLTree<int32_t>  tree;

    for (int32_t v = lo; v <= hi; ++v) {
        ASSERT_EQ (tree.first_greater_equals (v), tree.end ());
    }

    for (int32_t v = lo; v <= hi; ++v) {
        if (v % 2)
            tree.insert (v);
    }

    for (int32_t v = lo; v < hi; ++v) {

        if (v % 2) {
            ASSERT_EQ (*(tree.first_greater_equals (v)), v);
        } else {
            ASSERT_EQ (*(tree.first_greater_equals (v)), v + 1);
        }
    }

    if constexpr (hi % 2) {
        ASSERT_EQ (*(tree.first_greater_equals (hi)), hi);
    } else {
        ASSERT_EQ (tree.first_greater_equals (hi), tree.end ());
    }
}

/**
 * @brief   Test all variations of last_smaller_strict (largest, strictly smaller element)
 *
 */
TEST (Find, find_less_strict_test)
{
    constexpr int32_t   lo      {1};
    constexpr int32_t   hi      {1000};

    AgAVLTree<int32_t>  tree;

    for (int32_t v = hi; v >= lo; --v) {
        ASSERT_EQ (tree.last_smaller_strict (v), tree.end ());
    }

    for (int32_t v = hi; v >= lo; --v) {
        tree.insert (v);
    }

    for (int32_t v = hi; v > lo; --v) {
        ASSERT_EQ (*(tree.last_smaller_strict (v)), v - 1);
    }
    ASSERT_EQ (tree.last_smaller_strict (lo), tree.end ());
}

/**
 * @brief   Test all variations of last_smaller_equals (greatest, smaller or equal element)
 *
 */
TEST (Find, find_less_equals_test)
{
    constexpr int32_t   lo      {1};
    constexpr int32_t   hi      {1000};

    AgAVLTree<int32_t>  tree;

    for (int32_t v = hi; v >= lo; --v) {
        ASSERT_EQ (tree.last_smaller_equals (v), tree.end ());
    }

    for (int32_t v = hi; v >= lo; --v) {
        if (v % 2)
            tree.insert (v);
    }

    for (int32_t v = hi; v > lo; --v) {

        if (v % 2) {
            ASSERT_EQ (*(tree.last_smaller_equals (v)), v);
        } else {
            ASSERT_EQ (*(tree.last_smaller_equals (v)), v - 1);
        }
    }

    if constexpr (lo % 2) {
        ASSERT_EQ (*(tree.last_smaller_equals (lo)), lo);
    } else {
        ASSERT_EQ (tree.last_smaller_equals (lo), tree.end ());
    }
}

bool
lt (const char * const & a, const char * const & b)
{
    return strcmp (a, b) < 0;
}

bool
eq (const char * const & a, const char * const & b)
{
    return strcmp (a, b) == 0;
}

/**
 * @brief   Test if C style strings work (custom comparators)
 *
 */
TEST (CustomComparator, c_string_test)
{

    AgAVLTree<const char *, lt, eq> tree;

    char                            ar[]    = "useful";

    tree.insert ("AVL");
    tree.insert ("Trees");
    tree.insert ("are");
    tree.insert ("very");
    tree.insert ("useful");

    tree.insert (ar);

    ASSERT_EQ (tree.size (), 5);

    for (auto it1 = tree.begin (), it2 = ++tree.begin (); it2 != tree.end (); ++it1, ++it2) {
        ASSERT_EQ (lt (*it1, *it2), 1);
    }
}