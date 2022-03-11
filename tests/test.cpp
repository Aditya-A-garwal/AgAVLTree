#include <cstdlib>
#include <functional>

#include <gtest/gtest.h>

#define AVL_TEST_MODE                   // to be able to access private members and add extra diagnostic info collection

#include "AgAVLTree.h"

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
    ASSERT_EQ (pTree.insert (pValue), true);

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
    ASSERT_EQ (pTree.erase (pValue), true);

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
        ASSERT_EQ (tree.size (), (size_t)(v - lo + 1));
    }

    for (int32_t v = (hi / 2); v <= hi; ++v) {
        tree.insert (v);
        ASSERT_EQ (tree.size (), (size_t)(v - lo + 1));
    }

    ASSERT_EQ (tree.check_balance (), true);

    ASSERT_EQ (tree.erase (hi + 1), 0);
    ASSERT_EQ (tree.size (), (size_t)(hi - lo + 1));
    ASSERT_EQ (tree.erase (hi), 1);
    ASSERT_EQ (tree.size (), (size_t)(hi - lo));

    tree.clear ();
    ASSERT_EQ (tree.size (), (size_t)0);
}

/**
 * @brief   Test simple insertion with left-left rotation
 */
TEST (Insert, insert_ll_simple)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 2);
    insert (tree, 1, 0);
    ASSERT_ROTATIONS (tree, 1, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (right-right imbalance at node 2)
    //                    2
    //                      1
    //                        0

    // Expected shape of tree after rebalancing
    //                    1
    //                  0   2
}

/**
 * @brief   Test simple insertion with left-right rotation
 */
TEST (Insert, insert_lr_simple)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 2);
    insert (tree, 0, 1);
    ASSERT_ROTATIONS (tree, 0, 1, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (left-right imbalance at node 2)
    //                    2
    //               0
    //                 1

    // Expected shape of tree after rebalancing
    //                    1
    //                  0   2
}

/**
 * @brief   Test simple insertion with right-left rotation
 */
TEST (Insert, insert_rl_simple)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 0);
    insert (tree, 2, 1);
    ASSERT_ROTATIONS (tree, 0, 0, 1, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (right-left imbalance at node 2)
    //                    0
    //                         2
    //                       1

    // Expected shape of tree after rebalancing
    //                    1
    //                  0   2
}

/**
 * @brief   Test simple insertion with right-left rotation
 */
TEST (Insert, insert_rr_simple)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 0);
    insert (tree, 1, 2);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 1);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (right-right imbalance at node 0)
    //                    0
    //                      1
    //                        2

    // Expected shape of tree after rebalancing
    //                    1
    //                  0   2
}

/**
 * @brief   Test insertion with multiple left-left rotations
 */
TEST (Insert, insert_ll_compound)
{
    AgAVLTree<int32_t> tree;

    // first left-left rotation
    insert (tree, 2);
    insert (tree, 1, 0);
    ASSERT_ROTATIONS (tree, 1, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (left-left imbalance at node 2)
    //                    2
    //                  1
    //                0

    // Expected shape of tree after rebalancing (balanced)
    //                    1
    //                  0   2

    // second left-left rotation
    insert (tree, -1, -2);
    ASSERT_ROTATIONS (tree, 2, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertion (left-left imbalance at node 0)
    //                               1
    //               0                               2
    //     -1
    //  -2

    // Expected shape of tree after balancing (balanced)
    //                               1
    //              -1                               2
    //      -2               0

    // third and fourth left-left rotations
    insert (tree, -3, -4);
    ASSERT_ROTATIONS (tree, 4, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (left-left imbalance at nodes -2, later at node 1)
    //                                  1
    //                 -1                              2
    //         -2               0
    //     -3
    //   -4

    //                               1
    //              -1                               2
    //      -2               0
    //  -3
    //-4

    // Expected shape of tree after rebalancing (balanced)
    //                               1
    //              -2                               1
    //      -3                               0               2
    //  -4
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

    // first left-right rotation
    insert (tree, 2);
    insert (tree, 0, 1);
    ASSERT_ROTATIONS (tree, 0, 1, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (left-right imbalance at node 2)
    //                    2
    //                0
    //                  1

    // Expected shape of tree after rebalancing (balanced)
    //                    1
    //                  0   2

    // second left-right rotation
    insert (tree, -2, -1);
    ASSERT_ROTATIONS (tree, 0, 2, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertion (left-right imbalance at node 0)
    //                               1
    //               0                               2
    //      -2
    //          -1

    // Expected shape of tree after rebalancing (balanced)
    //                               1
    //              -1                               2
    //      -2               0

    // third left-right, first left-left rotations
    insert (tree, -4, -3);
    ASSERT_ROTATIONS (tree, 1, 3, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               1
    //              -1                               2
    //      -2               0
    //  -4
    //    -3

    // Expected shape of tree after rebalancing (balanced)
    //                              -1
    //              -3                               1
    //      -4              -2               0               2
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

    // first right-left rotation
    insert (tree, 0);
    insert (tree, 2, 1);
    ASSERT_ROTATIONS (tree, 0, 0, 1, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (right-left imbalance at node 2)
    //                    0
    //                        2
    //                      1

    // Expected shape of tree after rebalancing (balanced)
    //                    1
    //                  0   2

    // second right-left rotiation
    insert (tree, 4, 3);
    ASSERT_ROTATIONS (tree, 0, 0, 2, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertion (right-left imbalance at node 2)
    //                               1
    //               0                               2
    //                                                       4
    //                                                   3

    // Expected shape of tree after rebalancing (balanced)
    //                               1
    //               0                               3
    //                                       2               4

    // third right-left, first right-right rotation
    insert (tree, 6, 5);
    ASSERT_ROTATIONS (tree, 0, 0, 3, 1);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (right-left imbalance at node 4, right-right later at node 1)
    //                               1
    //               0                               3
    //                                       2               4
    //                                                           6
    //                                                         5

    // Expected shape of tree rebalancing (balanced)
    //                               3
    //               1                               5
    //       0               2               4               6
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

    // first right-right rotation
    insert (tree, 0);
    insert (tree, 1, 2);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 1);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (right-right imbalance at node 0)
    //                    0
    //                      1
    //                        2

    // Expected shape of tree after rebalancing (balanced)
    //                    1
    //                  0   2

    // second right-right rotation
    insert (tree, 3, 4);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 2);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (right-right imbalance at node 2)
    //                               1
    //               0                               2
    //                                                       3
    //                                                           4

    // Expected shape of tree after rebalancing (balanced)
    //                               1
    //               0                               3
    //                                       2               4

    // third and fourth right-right rotation
    insert (tree, 5, 6);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 4);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (right-right imabalance at node 4, later at node 1)
    //                               1
    //               0                               3
    //                                       2               4
    //                                                           5
    //                                                             6

    // Expected shape of tree after rebalancing (balanced)
    //                               3
    //               1                               5
    //       0               2               4               6
}

/**
 * @brief   Test erasing an ancestorless node without chidren (simple case)
 *
 */
TEST (Erase, no_child_simple)
{
    AgAVLTree<int32_t> tree;

    // insert and erase single element = 0
    tree.insert (0);
    ASSERT_EQ (tree.erase (0), true);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);
}

/**
 * @brief   Test erasing an ancestorless node with a right child only (simple case)
 *
 */
TEST (Erase, left_child_simple)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 0 , -1);

    // Expected shape of tree after insertions (balanced)
    //                    0
    //                -1

    ASSERT_EQ (tree.erase (0), true);
    ASSERT_EQ (tree.size (), (size_t)1);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after erase (balanced)
    //                    -1
}

/**
 * @brief   Test erasing an ancestorless node with a right child only (simple case)
 *
 */
TEST (Erase, right_child_simple)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 0, 1);

    // Expected shape of tree after insertions (balanced)
    //                    0
    //                       1

    ASSERT_EQ (tree.erase (0), true);
    ASSERT_EQ (tree.size (), (size_t)1);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after erase (balanced)
    //                    1
}

/**
 * @brief   Test erasing an ancestorless node with both children (simple case)
 *
 */
TEST (Erase, both_child_simple)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 0, -1, 1);

    // Expected shape of tree after insertions (balanced)
    //                    0
    //                -1     1

    ASSERT_EQ (tree.erase (0), true);
    ASSERT_EQ (tree.size (), (size_t)2);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after erase (balanced)
    //                    1
    //                -1
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
    ASSERT_EQ (tree.size (), (size_t)15);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               7
    //               3                              11
    //       1               5               9              13
    //   0       2       4       6       8      10      12      14

    erase (tree, 4, 6, 8, 10, 12, 14);
    ASSERT_EQ (tree.size (), (size_t)9);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after deletions (balanced)
    //                               7
    //               3                              11
    //       1               5               9              13
    //   0       2

    erase (tree, 9, 13);
    ASSERT_EQ (tree.size (), (size_t)7);
    ASSERT_ROTATIONS (tree, 1, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after deletions (left-left imbalance at node 7)
    //                               7
    //               3                              11
    //       1               5
    //   0       2

    // Expected shape of tree after rebalancing
    //                               3
    //               1                               7
    //       0               2               5              11
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
    ASSERT_EQ (tree.size (), (size_t)15);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               7
    //               3                              11
    //       1               5               9              13
    //   0       2       4       6       8      10      12      14

    erase (tree, 0, 2, 8, 10, 12, 14);
    ASSERT_EQ (tree.size (), (size_t)9);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after deletions (balanced)
    //                               7
    //               3                              11
    //       1               5               9              13
    //                   4       6

    erase (tree, 9, 13);
    ASSERT_EQ (tree.size (), (size_t)7);
    ASSERT_ROTATIONS (tree, 0, 1, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after deletions (left-right imbalance at node 7)
    //                               7
    //               3                              11
    //       1               5
    //                   4       6

    // Expected shape of tree after rebalancing
    //                               5
    //               3                               7
    //       1               4               6              11
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
    ASSERT_EQ (tree.size (), (size_t)15);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               7
    //               3                              11
    //       1               5               9              13
    //   0       2       4       6       8      10      12      14

    erase (tree, 0, 2, 4, 6, 12, 14);
    ASSERT_EQ (tree.size (), (size_t)9);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after deletions (balanced)
    //                               7
    //               3                              11
    //       1               5               9              13
    //                                   8      10

    erase (tree, 1, 5);
    ASSERT_EQ (tree.size (), (size_t)7);
    ASSERT_ROTATIONS (tree, 0, 0, 1, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (right-left imbalance at node 7)
    //                               7
    //               3                              11
    //                                       9              13
    //                                   8      10

    // Expected shape of tree after rebalancing
    //                               9
    //               7                              11
    //       3               8              10              13
    //
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
    ASSERT_EQ (tree.size (), (size_t)15);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               7
    //               3                              11
    //       1               5               9              13
    //   0       2       4       6       8      10      12      14

    erase (tree, 0, 2, 4, 6, 8, 10);
    ASSERT_EQ (tree.size (), (size_t)9);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               7
    //               3                              11
    //       1               5               9              13
    //                                                  12      14

    erase (tree, 1, 5);
    ASSERT_EQ (tree.size (), (size_t)7);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 1);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after deletions (right-right imbalance at node 7)
    //                               7
    //               3                              11
    //                                       9              13
    //                                                  12      14

    // Expected shape of tree after rebalancing
    //                              11
    //               7                              13
    //       3               9              12              14
}

/**
 * @brief   Test erasing a node with a left child, but having an ancestor become right-left heavy
 *
 */
TEST (Erase, left_child_rl)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 2);
    insert (tree, 1, 6);
    insert (tree, 0, 4, 8);
    insert (tree, 3, 5);
    ASSERT_EQ (tree.size (), (size_t)8);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               2
    //               1                               6
    //                                       4               8
    //                                   3       5

    tree.erase (0);
    ASSERT_EQ (tree.size (), (size_t)7);
    ASSERT_ROTATIONS (tree, 0, 0, 1, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after deletion (right-left imbalance at node 2)
    //                               2
    //               1                               6
    //                                       4               8
    //                                   3       5

    // Expected shape of tree after rebalancing
    //                               4
    //               2                               6
    //       1               3               5               8
}

/**
 * @brief   Test erasing a node with a left child, but having an ancestor become right-right heavy
 *
 */
TEST (Erase, left_child_rr)
{
    AgAVLTree<int32_t> tree;

    insert (tree, 2);
    insert (tree, 1, 6);
    insert (tree, 0, 4, 8);
    insert (tree, 7, 9);
    ASSERT_EQ (tree.size (), (size_t)8);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               2
    //               1                               6
    //       0                               4               8
    //                                                   7       9

    tree.erase (0);
    ASSERT_EQ (tree.size (), (size_t)7);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 1);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (right-right imbalance at node 2)
    //                               2
    //               1                               6
    //                                       4               8
    //                                                   7       9

    // Expected shape of tree after rebalancing
    //                               6
    //               2                               8
    //       1               4               7               9
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
    ASSERT_EQ (tree.size (), (size_t)8);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               7
    //               3                               8
    //       1               5                               9
    //   0       2

    tree.erase (9);
    ASSERT_EQ (tree.size (), (size_t)7);
    ASSERT_ROTATIONS (tree, 1, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after deletions (left-left imbalance at node 7)
    //                               7
    //               3                               8
    //       1               5
    //   0       2

    // Expected shape of tree after rebalancing
    //                               3
    //               1                               7
    //       0               2               5               8
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
    ASSERT_EQ (tree.size (), (size_t)8);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               7
    //               3                               8
    //       1               5
    //                   4       6

    tree.erase (9);
    ASSERT_EQ (tree.size (), (size_t)7);
    ASSERT_ROTATIONS (tree, 0, 1, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after deletions (left-right imbalance at node 7)
    //                               7
    //               3                               8
    //       1               5
    //                   4       6

    // Expected shape of tree after rebalancing
    //                               5
    //               3                               7
    //       1               4               6
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
    ASSERT_EQ (tree.size (), (size_t)7);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               3
    //               1                               5
    //       0               2               4               6

    tree.erase (3);
    ASSERT_EQ (tree.size (), (size_t)6);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after erase (balanced)
    //                               4
    //               1                               5
    //       0               2                               6
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
    ASSERT_EQ (tree.size (), (size_t)8);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               3
    //               1                               5
    //       0               2               4               7
    //                                                   6

    tree.erase (3);
    ASSERT_EQ (tree.size (), (size_t)7);
    ASSERT_ROTATIONS (tree, 0, 0, 1, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after erase (right-left imbalance at node 5)
    //                               4
    //               1                               5
    //       0               2                               7
    //                                                   6

    // Expected shape of tree after rebalancing
    //                               4
    //               1                               6
    //       0               2               5               7
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
    ASSERT_EQ (tree.size (), (size_t)8);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               3
    //               1                               5
    //       0               2               4               6
    //                                                           7

    tree.erase (3);
    ASSERT_EQ (tree.size (), (size_t)7);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 1);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after erase (right-right imbalance at node 5)
    //                               4
    //               1                               5
    //       0               2                               6
    //                                                           7

    // Expected shape of tree after rebalancing
    //                               4
    //               1                               6
    //       0               2               5               7
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
    ASSERT_EQ (tree.size (), (size_t)7);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               4
    //               2                               6
    //       1               3               5
    //   0

    tree.erase (4);
    ASSERT_EQ (tree.size (), (size_t)6);
    ASSERT_ROTATIONS (tree, 1, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after erase (left-left imbalance at node 5)
    //                               5
    //               2                               6
    //       1               3
    //   0

    // Expected shape of tree after rebalancing
    //                               2
    //               1                               5
    //       0                               3               6
    //                                   2
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
    ASSERT_EQ (tree.size (), (size_t)7);
    ASSERT_ROTATIONS (tree, 0, 0, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after insertions (balanced)
    //                               5
    //               2                               7
    //       1               4               6
    //                   3

    tree.erase (5);
    ASSERT_EQ (tree.size (), (size_t)6);
    ASSERT_ROTATIONS (tree, 0, 1, 0, 0);
    ASSERT_EQ (tree.check_balance (), true);

    // Expected shape of tree after erase (left-right imbalance at node 6)
    //                               6
    //               2                               7
    //       1               4
    //                   3

    // Expected shape of tree after rebalancing
    //                               4
    //               2                               7
    //       1               3               6
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

    // insert all elements between lo and hi (inclusive of both bounds)
    auto                    end     = tree.end ();
    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }
    // value of end() iterator should always be maintained (between any series of operations)
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

    // insert all elements between lo and hi (inclusive of both bounds)
    auto                   end = tree.rend ();
    for (int32_t v = hi; v >= lo; --v) {
        tree.insert (v);
    }
    // value of rend() iterator should always be maintained (between any series of operations)
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

    // insert all elements between lo and hi (inclusive of both bounds)
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

    // if the tree is empty, incrementing or decrementing the end() iterator should have no effect
    it      = tree.end ();
    ++it;
    ASSERT_EQ (it, end);
    it++;
    ASSERT_EQ (it, end);
    --it;
    ASSERT_EQ (it, end);
    it--;
    ASSERT_EQ (it, end);

    // insert a single element into the tree
    tree.insert (lo);

    // while the tree has one element, decrementing the end() iterator should have no effect
    it      = tree.end ();
    ++it;
    ASSERT_EQ (it, end);
    it++;
    ASSERT_EQ (it, end);

    // if the tree has one element, decrementing the end() iterator should make it point to the only node
    --it;
    ASSERT_EQ (*it, lo);
    it      = tree.end ();
    it--;
    ASSERT_EQ (*it, lo);

    // insert all elements between lo and hi (both bounds inclusive)
    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }

    // if tree has > 1 elements, incrementing the end() iterator should have no effect
    it  = tree.end ();
    ++it;
    ASSERT_EQ (it, end);
    it++;
    ASSERT_EQ (it, end);

    // if the tree has > 1 elements, decrementing the end() iterator should make it point to the greatest element
    --it;
    ASSERT_EQ (*it, *(tree.rbegin ()));
    it = tree.end ();
    it--;
    ASSERT_EQ (*it, *(tree.rbegin ()));
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

    // if the tree is empty, incrementing or decrementing the rend() iterator should have no effect
    it      = tree.rend ();
    ++it;
    ASSERT_EQ (it, end);
    it++;
    ASSERT_EQ (it, end);
    --it;
    ASSERT_EQ (it, end);
    it--;
    ASSERT_EQ (it, end);

    // insert a single element into the tree
    tree.insert (lo);

    // while the tree has one element, decrementing the rend() iterator should have no effect
    it      = tree.rend ();
    ++it;
    ASSERT_EQ (it, end);
    it++;
    ASSERT_EQ (it, end);

    // if the tree has one element, decrementing the rend() iterator should make it point to the only node
    --it;
    ASSERT_EQ (*it, lo);
    it      = tree.rend ();
    it--;
    ASSERT_EQ (*it, lo);

    // insert all elements between lo and hi (both bounds inclusive)
    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }

    // if tree has > 1 elements, incrementing the rend() iterator should have no effect
    it  = tree.rend ();
    ++it;
    ASSERT_EQ (it, end);
    it++;
    ASSERT_EQ (it, end);

    // if the tree has > 1 elements, decrementing the rend() iterator should make it point to the smallest element
    --it;
    ASSERT_EQ (*it, *(tree.begin ()));
    it  = tree.rend ();
    it--;
    ASSERT_EQ (*it, *(tree.begin ()));
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

    // decrementing the begin() iterator should have no effect if the tree is empty
    it      = tree.begin ();
    --it;
    ASSERT_EQ (it, tree.begin ());
    it--;
    ASSERT_EQ (it, tree.begin ());

    // decrementing the begin() iterator should have no effect if the tree has 1 element
    tree.insert (lo);
    it      = tree.begin ();
    --it;
    ASSERT_EQ (it, tree.begin ());
    it--;
    ASSERT_EQ (it, tree.begin ());

    // incrementing the begin() iterator should make it point to end() if the tree has 1 element
    ++it;
    ASSERT_EQ (it, tree.end ());
    it      = tree.begin ();
    it++;
    ASSERT_EQ (it, tree.end ());

    // insert all elements between lo and hi (both bounds inclusive)
    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }

    // decrementing the begin() iterator should have no effect if the tree has > 1 elements
    it      = tree.begin ();
    --it;
    ASSERT_EQ (it, tree.begin ());
    it--;
    ASSERT_EQ (it, tree.begin ());

    // if the tree has > 1 elements, incrementing the begin iterator should make it point to lo + 1 (second smallest element)
    if constexpr (hi - lo + 1 >= 2) {
        ++it;
        ASSERT_EQ (*it, lo + 1);
        it  = tree.begin ();
        it++;
        ASSERT_EQ (*it, lo + 1);
    }
    // if the tree has only 1 element, incrementing the begin iterator should make it point to end()
    else {
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
    constexpr int32_t                       lo      {1};
    constexpr int32_t                       hi      {1000};

    AgAVLTree<int32_t>                      tree;

    AgAVLTree<int32_t>::reverse_iterator    it;

    // decrementing the rbegin() iterator should have no effect if the tree is empty
    it      = tree.rbegin ();
    --it;
    ASSERT_EQ (it, tree.rbegin ());
    it--;
    ASSERT_EQ (it, tree.rbegin ());

    // decrementing the rbegin() iterator should have no effect if the tree has 1 element
    tree.insert (lo);
    it      = tree.rbegin ();
    --it;
    ASSERT_EQ (it, tree.rbegin ());
    it--;
    ASSERT_EQ (it, tree.rbegin ());

    // incrementing the rbegin() iterator should make it point to rend() if the tree has 1 element
    ++it;
    ASSERT_EQ (it, tree.rend ());
    it      = tree.rbegin ();
    it++;
    ASSERT_EQ (it, tree.rend ());

    // insert all elements between lo and hi (both bounds inclusive)
    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }

    // decrementing the rbegin() iterator should have no effect if the tree has > 1 elements
    it      = tree.rbegin ();
    --it;
    ASSERT_EQ (it, tree.rbegin ());
    it--;
    ASSERT_EQ (it, tree.rbegin ());

    // if the tree has > 1 elements, incrementing the rbegin iterator should make it point to hi - 1 (second largest element)
    if constexpr (hi - lo + 1 > 1) {
        ++it;
        ASSERT_EQ (*it, hi - 1);
        it  = tree.rbegin ();
        it++;
        ASSERT_EQ (*it, hi - 1);
    }
    // if the tree has only 1 element, incrementing the rbegin iterator should make it point to rend()
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

    // make both trees identical
    tree1.insert (0);
    tree1.insert (1);

    tree2.insert (0);
    tree2.insert (1);

    // get iterators to first element = 0
    auto                it1         = tree1.begin ();
    auto                it1_cpy     = tree1.begin ();
    auto                it2         = tree2.begin ();
    auto                it2_cpy     = tree2.begin ();

    // same tree, same node should be equal
    ASSERT_EQ (it1, it1_cpy);
    ASSERT_EQ (it2, it2_cpy);

    // different tree same value node not be equal
    ASSERT_NE (it1, it2);

    // increment first two iterators to point to 1
    // maintain values of other two
    ++it1;
    it2++;

    // same tree, different node should not be equal
    ASSERT_NE (it1, it1_cpy);
    ASSERT_NE (it2, it2_cpy);

    // different tree, different value node should not be equal
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

    // make both trees identical
    tree1.insert (0);
    tree1.insert (1);

    tree2.insert (0);
    tree2.insert (1);

    // get reverse iterators to last element = 1 (first element in reverse)
    auto                it1     = tree1.rbegin ();
    auto                it1_cpy = tree1.rbegin ();
    auto                it2     = tree2.rbegin ();
    auto                it2_cpy = tree2.rbegin ();

    // same tree, same node should be equal
    ASSERT_EQ (it1, it1_cpy);
    ASSERT_EQ (it2, it2_cpy);

    // different tree same value node should not be equal
    ASSERT_NE (it1, it2);

    // increment first two reverse iterators to point to 0
    // maintain values of other two
    ++it1;
    it2++;

    // same tree, different node should not be equal
    ASSERT_NE (it1, it1_cpy);
    ASSERT_NE (it2, it2_cpy);

    // different tree, different value node should not be equal
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

    // for an empty tree, all searches should result in failure (end())
    for (int32_t v = lo; v <= hi; ++v) {
        ASSERT_EQ (tree.find (v), tree.end ());
    }

    // insert all elements between lo and hi (inclusive of both bounds)
    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }

    // searching for all elements between lo and hi should be successful
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

    // for an empty tree, all searches should result in failure (end())
    for (int32_t v = lo; v <= hi; ++v) {
        ASSERT_EQ (tree.first_greater_strict (v), tree.end ());
    }

    // insert all odd elements between lo and hi (inclusive of both bounds)
    for (int32_t v = lo; v <= hi; ++v) {
        tree.insert (v);
    }

    // search for all elements between lo and hi
    for (int32_t v = lo; v < hi; ++v) {
        // for all keys, the element after the key should be returned
        ASSERT_EQ (*(tree.first_greater_strict (v)), v + 1);
    }
    // no key exists before lo, expect failed search
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

    // for an empty tree, all searches should result in failure (end())
    for (int32_t v = lo; v <= hi; ++v) {
        ASSERT_EQ (tree.first_greater_equals (v), tree.end ());
    }

    // insert all odd elements between lo and hi (inclusive of both bounds)
    for (int32_t v = lo; v <= hi; ++v) {
        if (v % 2)
            tree.insert (v);
    }

    // search for all elements between lo and hi
    for (int32_t v = lo; v < hi; ++v) {

        // for odd keys, the search should return the key itself
        if (v % 2) {
            ASSERT_EQ (*(tree.first_greater_equals (v)), v);
        }
        // for even keys, the search should return one after the key
        else {
            ASSERT_EQ (*(tree.first_greater_equals (v)), v + 1);
        }
    }

    // handle edge case for hi being even (one after hi does not exist in the tree)
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

    // for an empty tree, all searches should result in failure (end())
    for (int32_t v = hi; v >= lo; --v) {
        ASSERT_EQ (tree.last_smaller_strict (v), tree.end ());
    }

    // insert all odd elements between lo and hi (inclusive of both bounds)
    for (int32_t v = hi; v >= lo; --v) {
        tree.insert (v);
    }

    // search for all elements between lo and hi
    for (int32_t v = hi; v > lo; --v) {
        // for all keys, the element before the key should be returned
        ASSERT_EQ (*(tree.last_smaller_strict (v)), v - 1);
    }
    // no key exists before lo, expect failed search
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

    // for an empty tree, all searches should result in failure (end())
    for (int32_t v = hi; v >= lo; --v) {
        ASSERT_EQ (tree.last_smaller_equals (v), tree.end ());
    }

    // insert all odd elements between lo and hi (inclusive of both bounds)
    for (int32_t v = hi; v >= lo; --v) {
        if (v % 2)
            tree.insert (v);
    }

    // search for all elements between lo and hi
    for (int32_t v = hi; v > lo; --v) {

        // for odd keys, the search should return the key itself
        if (v % 2) {
            ASSERT_EQ (*(tree.last_smaller_equals (v)), v);
        }
        // for even keys, the search should return one before the key
        else {
            ASSERT_EQ (*(tree.last_smaller_equals (v)), v - 1);
        }
    }

    // handle edge case for lo being even (one before lo does not exist in the tree)
    if constexpr (lo % 2) {
        ASSERT_EQ (*(tree.last_smaller_equals (lo)), lo);
    }
    else {
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

    ASSERT_EQ (tree.insert ("AVL"), true);
    ASSERT_EQ (tree.insert ("Trees"), true);
    ASSERT_EQ (tree.insert ("are"), true);
    ASSERT_EQ (tree.insert ("very"), true);
    ASSERT_EQ (tree.insert ("useful"), true);

    // repeat insert should fail
    ASSERT_EQ (tree.insert (ar), false);

    ASSERT_EQ (tree.size (), (size_t)5);

    // each element should be strictly greater than the previous one
    for (auto it1 = tree.begin (), it2 = ++tree.begin (); it2 != tree.end (); ++it1, ++it2) {
        ASSERT_EQ (lt (*it1, *it2), true);
    }
}