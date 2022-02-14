//! VERY IMPORTANT IMPLEMENT DESTRUCTOR TO FIX MEMORY LEAK

#ifndef AVL_TREE_H
#define AVL_TREE_H

namespace avl_tree {

#ifdef AVL_TEST_MODE
namespace balance_info {

int32_t ll_count;
int32_t lr_count;
int32_t rl_count;
int32_t rr_count;

void
init ()
{
    ll_count = 0;
    lr_count = 0;
    rl_count = 0;
    rr_count = 0;
}
};                        // namespace balance_info
#endif                    // #ifdef AVL_TEST_MODE

template <typename val_t>
class AVL {

#ifdef AVL_TEST_MODE
    public:
#else
    protected:
#endif
    struct tree_node_t {
        tree_node_t * lptr {nullptr};
        tree_node_t * rptr {nullptr};
        uint8_t       height {0};
        val_t         val;
    };

    using node_t        = tree_node_t;
    using node_ptr_t    = node_t *;

    using link_t        = node_t *;
    using link_ptr_t    = link_t *;

    using balance_t     = void (*) (node_ptr_t *);
    using comparator_t  = bool (*) (const val_t &, const val_t &);

    public:

    //      terator types

    struct iterator {

#ifdef AVL_TEST_MODE
        public:
#else
        protected:
#endif

        using tree_ptr_t        = const AVL<val_t> *;
        using ref_t             = const val_t &;

        node_ptr_t mPtr         {nullptr};
        tree_ptr_t mTreePtr     {nullptr};

        public:

        iterator                (node_ptr_t pPtr, tree_ptr_t pTree_ptr);
        iterator                () = default;

        iterator operator++     ();
        iterator operator++     (int);
        iterator operator--     ();
        iterator operator--     (int);

        ref_t    operator*      () const;

        bool     operator==     (const iterator & pOther) const;
        bool     operator!=     (const iterator & pOther) const;
    };



    struct reverse_iterator {

#ifdef AVL_TEST_MODE
        public:
#else
        protected:
#endif

        using tree_ptr_t        = const AVL<val_t> *;
        using ref_t             = const val_t &;

        node_ptr_t mPtr         {nullptr};
        tree_ptr_t mTreePtr     {nullptr};

        public:

        reverse_iterator        (node_ptr_t pPtr, tree_ptr_t pTree_ptr);
        reverse_iterator        () = default;

        reverse_iterator operator++     ();
        reverse_iterator operator++     (int);
        reverse_iterator operator--     ();
        reverse_iterator operator--     (int);

        ref_t            operator*      () const;

        bool             operator==     (const reverse_iterator & pOther) const;
        bool             operator!=     (const reverse_iterator & pOther) const;
    };



    //      Constructors

    AVL                                         ();
    AVL                                         (const comparator_t pLtComp, const comparator_t pEqComp);
    AVL                                         (const AVL &) = delete;

    //      Destructor

    ~AVL                                        ();

    //      Getter for size

    size_t           size                       () const;

    //      Setter for clear

    void             clear                      ();

    //      Iterators

    iterator         begin                      () const;
    iterator         end                        () const;
    reverse_iterator rbegin                     () const;
    reverse_iterator rend                       () const;

    //      Modifiers

    bool             insert                     (const val_t pVal);
    bool             erase                      (const val_t pVal);

    //      Binary searches

    iterator         find                       (const val_t pVal) const;
    iterator         first_greater_strict       (const val_t pVal) const;
    iterator         first_greater_equals       (const val_t pVal) const;
    iterator         last_smaller_strict        (const val_t pVal) const;
    iterator         last_smaller_equals        (const val_t pVal) const;

    //      Utilities for testing

#ifdef AVL_TEST_MODE
    bool             check_balance              (node_ptr_t pCur);
    bool             check_balance              ();
    val_t            get_root_val               ();
#endif

#ifdef AVL_TEST_MODE
    public:
#else
    private:
#endif

    node_ptr_t   mRoot {nullptr};
    size_t       mSz {0};

    comparator_t mComp;
    comparator_t mEquals;

    //      Comparators

    static bool  default_lessthan               (const val_t & pA, const val_t & pB);
    static bool  default_equals                 (const val_t & pA, const val_t & pB);

    //      Aggregators

    template <typename arg_t>
    static arg_t max                            (const arg_t & pA, const arg_t & pB);
    template <typename arg_t>
    static arg_t min                            (const arg_t & pA, const arg_t & pB);

    //      Balance Utilities

    static void  calc_height                    (node_ptr_t pCur, uint8_t & pLdep, uint8_t & pRdep);
    static void  balance_ll                     (link_ptr_t pRoot);
    static void  balance_lr                     (link_ptr_t pRoot);
    static void  balance_rl                     (link_ptr_t pRoot);
    static void  balance_rr                     (link_ptr_t pRoot);

    //

    node_ptr_t   find_min                       (node_ptr_t pRoot) const;
    node_ptr_t   find_max                       (node_ptr_t pRoot) const;
    node_ptr_t   find_min                       () const;
    node_ptr_t   find_max                       () const;

    //      Modifiers

    bool         insert                         (node_ptr_t * pCur, const val_t & pVal);
    bool         erase                          (node_ptr_t * pCur, const val_t & pVal);

    //

    node_ptr_t   find_min_move_up               (link_ptr_t pCur);
    node_ptr_t   find_min_move_up               ();
    node_ptr_t   find_max_move_up               (link_ptr_t pCur);
    node_ptr_t   find_max_move_up               ();

    //      Binary searches

    node_ptr_t   find_ptr                       (const val_t & pVal) const;
    node_ptr_t   first_greater_strict_ptr       (const val_t & pVal, node_ptr_t pVur) const;
    node_ptr_t   first_greater_strict_ptr       (const val_t & pVal) const;
    node_ptr_t   first_greater_equals_ptr       (const val_t & pVal, node_ptr_t pCur) const;
    node_ptr_t   first_greater_equals_ptr       (const val_t & pVal) const;
    node_ptr_t   last_smaller_strict_ptr        (const val_t & pVal, node_ptr_t pCur) const;
    node_ptr_t   last_smaller_strict_ptr        (const val_t & pVal) const;
    node_ptr_t   last_smaller_equals_ptr        (const val_t & pVal, node_ptr_t pCur) const;
    node_ptr_t   last_smaller_equals_ptr        (const val_t & pVal) const;

    //      Setter for clear

    void         clear                          (node_ptr_t pCur);
};
};                    // namespace avl_tree

/**
 * @brief                   Construct a new avl tree::AVL<val t>::AVL object
 *
 * @tparam val_t
 */
template <typename val_t>
avl_tree::AVL<val_t>::AVL () : mComp {default_lessthan}, mEquals {default_equals}
{}

/**
 * @brief                   Construct a new avl tree::AVL<val t>::AVL object
 *
 * @tparam val_t
 *
 * @param pLtComp
 * @param pEqComp
 */
template <typename val_t>
avl_tree::AVL<val_t>::AVL (const comparator_t pLtComp, const comparator_t pEqComp) : mComp {pLtComp}, mEquals {pEqComp}
{}

/**
 * @brief                   Destroy the avl tree::AVL<val t>::AVL object
 *
 * @tparam val_t            Type of data held by tree instance
 */
template <typename val_t>
avl_tree::AVL<val_t>::~AVL ()
{
    this->clear();
}

/**
 * @brief                   Returns the size of the tree (number of elements)
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @return size_t           Size of tree
 */
template <typename val_t>
size_t
avl_tree::AVL<val_t>::size () const
{
    return mSz;
}

/**
 * @brief                   Erases all elements from the tree (bringing size to 0)
 *
 * @tparam val_t            Type of data held by tree instance
 */
template <typename val_t>
void
avl_tree::AVL<val_t>::clear ()
{
    // if the root does not exist, return, else clear starting from the root
    if (mRoot == nullptr) {
        return;
    }

    clear (mRoot);
    mRoot   = nullptr;
    mSz     = 0;
}

/**
 * @brief                   Attempts to insert a value into the tree
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal              The value to be inserted into the tree
 *
 * @return true             If insertion was successful
 * @return false            If insertion failed
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::insert (val_t pVal)
{
    return insert (&mRoot, pVal);
}

/**
 * @brief                   Attempts to erase a value from the tree
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal              The value to be found and erased
 *
 * @return true             If value was successfuly erased
 * @return false            If value could not be successfuly erased (likely not found)
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::erase (val_t pVal)
{
    return erase (&mRoot, pVal);
}

/**
 * @brief                   Finds and returns an iterator to a given value (end() if no match exists in the tree)
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal              The value to be found
 *
 * @return avl_tree::AVL<val_t>::iterator Iterator to matching value in the tree (end() if no match found)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::find (val_t pVal) const
{
    node_ptr_t res  {find_ptr (pVal)};
    return iterator (res, this);
}

/**
 * @brief                   Finds and returns an iterator to the first value strictly greater than the given value (end() if no match exists)
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal              The value to be compared with
 *
 * @return avl_tree::AVL<val_t>::iterator Iterator to first strictly greater value in the tree (end() if no match found)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::first_greater_strict (val_t pVal) const
{
    node_ptr_t res  {first_greater_strict_ptr (pVal)};
    return iterator (res, this);
}

/**
 * @brief                   Finds and returns an iterator to the first value greater than or equal to the given value (end() if no match exists)
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal               The value to be compared with
 *
 * @return avl_tree::AVL<val_t>::iterator Iterator to first greater or equal value in the tree (end() if no match found)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::first_greater_equals (val_t pVal) const
{
    node_ptr_t res  {first_greater_equals_ptr (pVal)};
    return iterator (res, this);
}

/**
 * @brief                   Finds and returns an iterator to the last value strictly less than the given value (end() if no match exists)
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal               The value to be compared with
 *
 * @return avl_tree::AVL<val_t>::iterator Iterator to last strictly less value in the tree (end() if no match found)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::last_smaller_strict (val_t pVal) const
{
    node_ptr_t res  {last_smaller_strict_ptr (pVal)};
    return iterator (res, this);
}

/**
 * @brief                   Finds and returns an iterator to the last value less than or equal to the given value (end() if no match exists)
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal              The value to be compared with
 *
 * @return avl_tree::AVL<val_t>::iterator Iterator to last less or equal value in the tree (end() if no match found)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::last_smaller_equals (val_t pVal) const
{
    node_ptr_t res  {last_smaller_equals_ptr (pVal)};
    return iterator (res, this);
}

/**
 * @brief                   Returns an iterator to the beginning
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @return avl_tree::AVL<val_t>::iterator Iterator to the first element
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::begin () const
{
    return iterator (find_min (mRoot), this);
}

/**
 * @brief                   Returns an iterator to the element one after the last element
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @return avl_tree::AVL<val_t>::iterator Iterator to the element following the last element
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::end () const
{
    return iterator (nullptr, this);
}

/**
 * @brief                   Returns a reverse iterator to the beginning
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @return avl_tree::AVL<val_t>::reverse_iterator Reverse Iterator to the beginning
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::reverse_iterator
avl_tree::AVL<val_t>::rbegin () const
{
    return reverse_iterator (find_max (mRoot), this);
}

/**
 * @brief                   Returns a reverse iterator to the element one after the last element
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @return avl_tree::AVL<val_t>::reverse_iterato Iterator to the element following the last element
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::reverse_iterator
avl_tree::AVL<val_t>::rend () const
{
    return reverse_iterator (nullptr, this);
}

// PRIVATE

/**
 * @brief                   Default comparator function for less than comparison if none given by user (requires < operator to be implemented)
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pA                First element (element to be compared to)
 * @param pB                Second element (element to be compared)
 *
 * @return true             If pA is strictly less than pB
 * @return false            If pA is not stricly less than pB
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::default_lessthan (const val_t & pA, const val_t & pB)
{
    return pA < pB;
}

/**
 * @brief                   Default comparator function for mEquals comparison if none given by user (requires == operator to be implemented)
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pA                First element (element to be compared to)
 * @param pB                Second element (element to be compared)
 *
 * @return true             if pA is strictly equal to pB
 * @return false            if pA is not strictly equal to pB
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::default_equals (const val_t & pA, const val_t & pB)
{

    // requires experimental/type_traits

    // if constexpr (std::experimental::fundamentals_v2::is_detected<equals_t, val_t>::value) {
    // if constexpr (std::experimental::fundamentals_v2::is_detected<
    //                   decltype (std::declval<T> () == std::declval<T> ()), val_t>::value) {
    //     return pA == pB;
    // } else {
    //     return !(mComp (pA, pB)) and !(mComp (pB, pA));
    // }
    return pA == pB;
}

/**
 * @brief                   Returns the maximum of two elements (first if both compare equal)
 *
 * @tparam val_t            Type of data held by tree instance
 * @tparam arg_t            Type of values to be compared
 *
 * @param pA                First element to be compared
 * @param pB                Second element to be compared
 *
 * @return arg_t            The maximum of the two supplied arguments
 */
template <typename val_t>
template <class arg_t>
arg_t
avl_tree::AVL<val_t>::max (const arg_t & pA, const arg_t & pB)
{
    return (pA > pB) ? (pA) : (pB);
}

/**
 * @brief                   Returns the minimum of two elements (first if both compare mEquals)
 *
 * @tparam val_t            Type of data held by tree instance
 * @tparam arg_t            Type of values to be compared
 *
 * @param pA                First element to be compared
 * @param pB                Second element to be compared
 *
 * @return arg_t            The maximum of the two supplied arguments
 */
template <typename val_t>
template <class arg_t>
arg_t
avl_tree::AVL<val_t>::min (const arg_t & pA, const arg_t & pB)
{
    return (pA < pB) ? (pA) : (pB);
}

/**
 * @brief                   Calculates the depths of the left and right subtrees of the given node
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pCur              Node whose subtrees's heights are to be calculated
 * @param pLdep             Reference to variable where left subtree's depth is kept
 * @param pRdep             Reference to variable where right subtree's depth is kept
 *
 * @note                    The calculated depths are of the subtrees of the given node, i.e. they include the supplied node
 */
template <typename val_t>
void
avl_tree::AVL<val_t>::calc_height (avl_tree::AVL<val_t>::node_ptr_t pCur, uint8_t & pLdep, uint8_t & pRdep)
{
    // if a child does not exist, return 0 (the corresponding subtree is non-existant)
    pLdep   = (pCur->lptr != nullptr) ? (1 + pCur->lptr->height) : (0);
    pRdep   = (pCur->rptr != nullptr) ? (1 + pCur->rptr->height) : (0);
}

/**
 * @brief                   Function to balance a node which is left-left heavy
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pRoot             Pointer to the edge between the pivot and it's parent
 */
template <typename val_t>
void
avl_tree::AVL<val_t>::balance_ll (avl_tree::AVL<val_t>::link_ptr_t pRoot)
{
    uint8_t    ldep;
    uint8_t    rdep;

    node_ptr_t top;
    node_ptr_t bot;

    top             = *pRoot;
    bot             = top->lptr;

    top->lptr       = bot->rptr;                    // right child of bot becomes left child of top
    bot->rptr       = top;                          // top becomes the right child of bot
    *pRoot          = bot;                          // pointer to top now points to bot

    // recalculate depths of shifted nodes
    calc_height (top, ldep, rdep);
    top->height     = max (ldep, rdep);

    // calc_height (bot, ldep, rdep);
    // bot->height = max (ldep, rdep);
    bot->height     = 1 + max (bot->lptr->height, bot->rptr->height);

#ifdef AVL_TEST_MODE
    balance_info::ll_count += 1;
#endif                    // #ifdef AVL_TEST_MODE
}

/**
 * @brief                   Function to balance a node which is left-right heavy
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pRoot             Pointer to the edge between the pivot and it's parent
 */
template <typename val_t>
void
avl_tree::AVL<val_t>::balance_lr (avl_tree::AVL<val_t>::link_ptr_t pRoot)
{
    uint8_t    ldep;
    uint8_t    rdep;

    node_ptr_t top;
    node_ptr_t mid;
    node_ptr_t bot;

    top             = *pRoot;
    mid             = top->lptr;
    bot             = mid->rptr;

    mid->rptr       = bot->lptr;                    // left child of bot becomes right child of mid
    bot->lptr       = mid;                          // mid becomes left child of bot
    top->lptr       = bot->rptr;                    // right child of bot becomes left child of top
    bot->rptr       = top;                          // top becomes right child of bot
    *pRoot          = bot;                          // pointer to top now points to bot

    // recalculate depths of shifted nodes
    calc_height (top, ldep, rdep);
    top->height     = max (ldep, rdep);

    calc_height (mid, ldep, rdep);
    mid->height     = max (ldep, rdep);

    // calc_height (bot, ldep, rdep);
    // bot->height = max (ldep, rdep);
    bot->height     = 1 + max (bot->lptr->height, bot->rptr->height);

#ifdef AVL_TEST_MODE
    balance_info::lr_count += 1;
#endif                    // #ifdef AVL_TEST_MODE
}

/**
 * @brief                   Function to balance a node which is right-left heavy
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pRoot             Pointer to the edge between the pivot and it's parent
 */
template <typename val_t>
void
avl_tree::AVL<val_t>::balance_rl (avl_tree::AVL<val_t>::link_ptr_t pRoot)
{
    uint8_t    ldep;
    uint8_t    rdep;

    node_ptr_t top;
    node_ptr_t mid;
    node_ptr_t bot;

    top             = *pRoot;
    mid             = top->rptr;
    bot             = mid->lptr;

    top->rptr       = bot->lptr;                    // left child of bot becomes right child of top
    bot->lptr       = top;                          // top becomes left child of bot
    mid->lptr       = bot->rptr;                    // right child of bot becomes left child of mid
    bot->rptr       = mid;                          // mid becomes right child of bot
    *pRoot          = bot;                          // pointer to top now points to bot

    // recalculate depths of shifted nodes
    calc_height (top, ldep, rdep);
    top->height     = max (ldep, rdep);

    calc_height (mid, ldep, rdep);
    mid->height     = max (ldep, rdep);

    // calc_height (bot, ldep, rdep);
    // bot->height = max (ldep, rdep);
    bot->height     = 1 + max (bot->lptr->height, bot->rptr->height);

#ifdef AVL_TEST_MODE
    balance_info::rl_count += 1;
#endif                    // #ifdef AVL_TEST_MODE
}

/**
 * @brief                   Function to balance a node which is right-right heavy
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pRoot             Pointer to the edge between the pivot and it's parent
 */
template <typename val_t>
void
avl_tree::AVL<val_t>::balance_rr (avl_tree::AVL<val_t>::link_ptr_t pRoot)
{
    uint8_t    ldep;
    uint8_t    rdep;

    node_ptr_t top;
    node_ptr_t bot;

    top             = *pRoot;
    bot             = top->rptr;

    top->rptr       = bot->lptr;                    // left child of bot becomes right child of top
    bot->lptr       = top;                          // top becomes the left child of bot
    *pRoot          = bot;                          // pointer to top now points to bot

    // recalculate depths of shifted nodes
    calc_height (top, ldep, rdep);
    top->height     = max (ldep, rdep);

    // calc_height (bot, ldep, rdep);
    // bot->height = max (ldep, rdep);

    bot->height     = 1 + max (bot->lptr->height, bot->rptr->height);

#ifdef AVL_TEST_MODE
    balance_info::rr_count += 1;
#endif                    // #ifdef AVL_TEST_MODE
}

/**
 * @brief                   Finds the minimum value node in a subtree without modifying it
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pRoot             Root of the subtree to be processed
 *
 * @return node_ptr_t       Pointer to minimum value node in the subtree of pRoot
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::find_min (avl_tree::AVL<val_t>::node_ptr_t pRoot) const
{
    node_ptr_t res {pRoot};

    // if the root does not exist (no nodes in tree), return null
    if (res == nullptr) {
        return nullptr;
    }

    // keep going left for as long as possible and return the leftmost (smallest) node
    while (res->lptr != nullptr) {
        res = res->lptr;
    }

    return res;
}

/**
 * @brief                   Finds the minimum value node in the entire tree without modifying it
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @return node_ptr_t       Pointer to the minimum value node in the entire tree
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::find_min () const
{
    return find_min (mRoot);
}

/**
 * @brief                   Finds the maximum value node in a subtree without modifying it
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pRoot             Root of the subtree to be processed
 *
 * @return node_ptr_t       Pointer to maximum value node in the subtree of pRoot
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::find_max (avl_tree::AVL<val_t>::node_ptr_t pRoot) const
{
    node_ptr_t res {pRoot};

    // if the root does not exist (no nodesi n tree), return null
    if (res == nullptr) {
        return nullptr;
    }

    // keep going right for as long as possible and return the rightmost (greatest) node
    while (res->rptr != nullptr) {
        res = res->rptr;
    }

    return res;
}

/**
 * @brief                   Finds the maximum value node in the entire tree without modifying it
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @return node_ptr_t       Pointer to the maximum value node in the entire tree
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::find_max () const
{
    return find_max (mRoot);
}

/**
 * @brief                   Attempts to insert a new node in the subtree of an existing node
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pCur              Pointer to node's link in whose subtree a new value must be inserted
 * @param pVal              Reference to value to be inserted
 *
 * @return true             If insertion was successful (new node created)
 * @return false            If insertion failed
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::insert (avl_tree::AVL<val_t>::link_ptr_t pCur, const val_t & pVal)
{
    // if the current pointet points to null, this is the correct location to insert a node_t
    if (*pCur == nullptr) {

        node_ptr_t  ins;
        ins     = new (std::nothrow) node_t {nullptr, nullptr, 0, pVal};

        if (ins == nullptr) {   // failed insertion
            return false;
        }

        *pCur   = ins;

        ++mSz;

        return true;
    }

    // found matching node, insert failed
    if (mEquals ((*pCur)->val, pVal)) {
        return false;
    }

    // if not equal, try to recursively insert
    // if successful, recalculate depths and mBalance
    // if( insert( &(*cur)->cptr[!mComp( val, (*cur)->val )], val ) ) {

    node_ptr_t  *whichPtr;
    whichPtr    = (mComp (pVal, (*pCur)->val)) ? (&(*pCur)->lptr) : (&(*pCur)->rptr);

    if (insert (whichPtr, pVal)) {

        uint8_t ldep;
        uint8_t rdep;

        calc_height (*pCur, ldep, rdep);

        if (ldep > (1 + rdep)) {

            (mComp (pVal, (*pCur)->lptr->val)) ? (balance_ll (pCur)) : (balance_lr (pCur));
            --ldep;
        } else if (rdep > (1 + ldep)) {

            (mComp (pVal, (*pCur)->rptr->val)) ? (balance_rl (pCur)) : (balance_rr (pCur));
            --rdep;
        }

        (*pCur)->height = max (ldep, rdep);

        return true;
    }

    return false;
}


/**
 * @brief                   Attempts to erase a node from the subtree of an existing node
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pCur              Pointer to node's link in whose subtree a new value must be inserted
 * @param pVal              Reference to value to be erased
 *
 * @return true             If erasing was successful (old node deleted)
 * @return false            If erasing failed
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::erase (avl_tree::AVL<val_t>::link_ptr_t pCur, const val_t & pVal)
{
    if (*pCur == nullptr) {
        return false;                                                           // could not find matching node; return failed insertion
    }

    if (mEquals ((*pCur)->val, pVal)) {                                         // found a matching node, try to remove it

        node_ptr_t nxt {nullptr};

        // todo use inorder predeccessor if more efficient
        if ((*pCur)->lptr != nullptr and (*pCur)->rptr != nullptr) {            // the the node to be deleted has both children, find its inorder successor to move up

            nxt       = find_min_move_up (&(*pCur)->rptr);

            nxt->lptr = (*pCur)->lptr;
            nxt->rptr = (*pCur)->rptr;
        }

        else if ((*pCur)->lptr != nullptr and (*pCur)->rptr == nullptr) {       // only left child exists, move it up and delete the current node
            nxt = (*pCur)->lptr;
        }

        else if ((*pCur)->lptr == nullptr and (*pCur)->rptr != nullptr) {       // only right child exists, make this the next node
            nxt = (*pCur)->rptr;
        }

        delete *pCur;                                                           // delete current node
        *pCur = nxt;                                                            // move up the next node

        --mSz;
    }

    else if (!erase ((mComp (pVal, (*pCur)->val)) ? (&(*pCur)->lptr) : (&(*pCur)->rptr), pVal)) {
        return false;                                                           // try to recursively erase (go left if current > supplied value else right), return false if failed
    }

    if (*pCur != nullptr) {                                                     // if a node was removed, recalculate heights of all shifted nodes and rebalance

        uint8_t ldep;                                                           // left depth of the current node
        uint8_t rdep;                                                           // right depth of the current node

        uint8_t lldep;                                                          // left depth of the heavier child's left child (grandchild of current node)
        uint8_t rrdep;                                                          // left depth of the heavier child's right child (grandchild of current node)

        calc_height (*pCur, ldep, rdep);                                        // since lower nodes were moved, re-calculate the height of this node

        if (ldep > (1 + rdep)) {

            calc_height ((*pCur)->lptr, lldep, rrdep);                          // in case the left child is too heavy, get heights of its children to check
            (lldep >= rrdep) ? (balance_ll (pCur)) : (balance_lr (pCur));
            --ldep;
        } else if (rdep > (1 + ldep)) {

            calc_height ((*pCur)->rptr, lldep, rrdep);                          // in case the right child is too heavy, get heights of its children to check
            (lldep >= rrdep) ? (balance_rl (pCur)) : (balance_rr (pCur));
            --rdep;
        }

        (*pCur)->height = max (ldep, rdep);
    }

    return true;
}

/**
 * @brief                   Finds inorder successor of a node and replace the node with it
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pCur              Pointer to node's link which is to be replaced
 *
 * @return avl_tree::AVL<val_t>::node_ptr_t Inorder succesor of given node (after it's right child has been moved up)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::find_min_move_up (avl_tree::AVL<val_t>::link_ptr_t pCur)
{
    node_ptr_t res;

    uint8_t ldep;                                                               // left depth of current node
    uint8_t rdep;                                                               // right depth of current node

    uint8_t lldep;                                                              // left depth of current node's right child
    uint8_t rrdep;                                                              // right depth of current node's right child

    if ((*pCur)->lptr != nullptr) {                                             // if the left child exists, then atleast one smaller node exists

        res = find_min_move_up (&(*pCur)->lptr);                                // recursively go left
        calc_height (*pCur, ldep, rdep);                                        // after left subtree is processed, recalculate height of current node

        if (rdep > (1 + ldep)) {

            // in case the right child is too heavy, get its childrens's heights and balance accordingly
            calc_height ((*pCur)->rptr, lldep, rrdep);
            (lldep > rrdep) ? (balance_rl (pCur)) : (balance_rr (pCur));
            --rdep;
        }

        (*pCur)->height = max (ldep, rdep);

    } else {                                                                    // since no left child exsists. this is the smallest node in the current subtree

        res  = *pCur;                                                           // make res point to the current node
        *pCur = res->rptr;                                                      // replace the current node with its right child
    }

    return res;
}

/**
 * @brief                   Finds inorder successor of the root
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @return avl_tree::AVL<val_t>::node_ptr_t Inorder succesor of the root
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::find_min_move_up ()
{
    return find_min_move_up (&mRoot);
}

/**
 * @brief                   Finds a node with value equal to the given value
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal              Value to find
 *
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to the node with equal value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::find_ptr (const val_t & pVal) const
{
    node_ptr_t cur {mRoot};

    // repeat while a valid node is being pointed to (not crossed a leaf)
    while (cur != nullptr) {

        // if a matching node was found, return it
        if (mEquals (pVal, cur->val)) {
            return cur;
        }

        // go left if current node is too big, else go right
        cur = (mComp (pVal, cur->val)) ? (cur->lptr) : (cur->rptr);
    }

    // if no match was found, return null
    return nullptr;
}

/**
 * @brief                   Finds a node with value strictly greater than the given value in an existing node's subtree
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal              Value to find
 * @param pCur              Pointer to node in whose subtree the search must take place
 *
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a strictly greater value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::first_greater_strict_ptr (const val_t & pVal, avl_tree::AVL<val_t>::node_ptr_t pCur) const
{
    // if reached beyond leaf (no valid node could be found in the current subtree), return null
    if (pCur == nullptr) {
        return nullptr;
    }

    // if the current node <= supplied value, go right (current node's value is too small)
    if (mComp (pCur->val, pVal) or mEquals (pCur->val, pVal)) {
        return first_greater_strict_ptr (pVal, pCur->rptr);
    }

    node_ptr_t res  {first_greater_strict_ptr (pVal, pCur->lptr)};              // try going right recursively to find a better match than the current node
    return (res != nullptr) ? (res) : (pCur);                                   // if a valid, larger-value node was found, use it otherwise use the current node
}

/**
 * @brief                   Finds a node with value strictly greater than the given value
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal              Value to find
 *
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a strictly greater value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::first_greater_strict_ptr (const val_t & pVal) const
{
    return first_greater_strict_ptr (pVal, mRoot);
}

/**
 * @brief                   Finds a node with value not less than the given value in an existing node's subtree
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal              Value to find
 * @param pCur              Pointer to node in whose subtree the search must take place
 *
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a greater or equal value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::first_greater_equals_ptr (const val_t & pVal, avl_tree::AVL<val_t>::node_ptr_t pCur) const
{
    // if reached beyond leaf (no valid node could be found in the current subtree), return null
    if (pCur == nullptr) {
        return nullptr;
    }

    // if the current node < supplied value, go right (current node's value is too small)
    if (mComp (pCur->val, pVal)) {
        return first_greater_equals_ptr (pVal, pCur->rptr);
    }

    node_ptr_t res  {first_greater_equals_ptr (pVal, pCur->lptr)};              // try going left recursively to find a better match than the current node
    return (res != nullptr) ? (res) : (pCur);                                   // if a valid, smaller-value node was found, use it otherwise use the current node
}

/**
 * @brief                   Finds a node with value not less than the given value
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal              Value to find
 *
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a greater or equal value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::first_greater_equals_ptr (const val_t & pVal) const
{
    return first_greater_equals_ptr (pVal, mRoot);
}

/**
 * @brief                   Finds a node with value strictly less than the given value in an existing node's subtree
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal              Value to find
 * @param pCur              Pointer to node in whose subtree the search must take place
 *
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a strictly less value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::last_smaller_strict_ptr (const val_t & pVal, avl_tree::AVL<val_t>::node_ptr_t pCur) const
{
    // if reached beyond leaf (no valid node could be found in the current subtree), return null
    if (pCur == nullptr) {
        return nullptr;
    }

    // if the supplied value <= current node, go left (current node's value is too big)
    if (mComp (pVal, pCur->val) or mEquals (pVal, pCur->val)) {
        return last_smaller_strict_ptr (pVal, pCur->lptr);
    }

    node_ptr_t res  {last_smaller_strict_ptr (pVal, pCur->rptr)};               // try going right recursively to find a better match than the current node
    return (res != nullptr) ? (res) : (pCur);                                   // if a valid, larger-value node was found, use it otherwise use the current node
}

/**
 * @brief                   Finds a node with value strictly less than the given value
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal              Value to find
 *
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a strictly less value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::last_smaller_strict_ptr (const val_t & pVal) const
{
    return last_smaller_strict_ptr (pVal, mRoot);
}

/**
 * @brief                   Finds a node with value not more than the given value in an existing node's subtree
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal              Value to find
 * @param pCur              Pointer to node in whose subtree the search must take place
 *
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a less or equal value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::last_smaller_equals_ptr (const val_t & pVal, avl_tree::AVL<val_t>::node_ptr_t pCur) const
{
    // if reached beyond leaf (no valid node could be found in current subtree), return null
    if (pCur == nullptr) {
        return nullptr;
    }

    // if the supplied value < current node, go left (current node's value is too big)
    if (mComp (pVal, pCur->val)) {
        return last_smaller_equals_ptr (pVal, pCur->lptr);
    }

    node_ptr_t res  {last_smaller_equals_ptr (pVal, pCur->rptr)};               // try going right recursively to find a better match than the current node
    return (res != nullptr) ? (res) : (pCur);                                   // if a valid, larger-value node was found, use it otherwise use the current node
}

/**
 * @brief                   Finds a node with value not more than the given value
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pVal              Value to find
 *
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a less or equal value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::last_smaller_equals_ptr (const val_t & pVal) const
{
    return last_smaller_equals_ptr (pVal, mRoot);
}

/**
 * @brief                   Clear the subtree of an entire node (including the node) by deleting
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pCur              Pointer to node whose subtree is to be deleted
 */
template <typename val_t>
void
avl_tree::AVL<val_t>::clear (avl_tree::AVL<val_t>::node_ptr_t pCur)
{
    // if left child exists, recursively clear its subtree
    if (pCur->lptr != nullptr) {
        clear (pCur->lptr);
    }

    // if right child exists, recursively clear its subtree
    if (pCur->rptr != nullptr) {
        clear (pCur->rptr);
    }

    delete pCur;
}


//! Iterators

/**
 * @brief                   Construct a new avl tree::AVL<val t>::iterator::iterator object
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pPtr              The pointer to encapsulate (points to node or nullptr)
 * @param PTreeptr          The point to the tree which contains the node
 */
template <typename val_t>
avl_tree::AVL<val_t>::iterator::iterator (avl_tree::AVL<val_t>::node_t * pPtr, const avl_tree::AVL<val_t> * pTreePtr) :
    mPtr {pPtr}, mTreePtr {pTreePtr}
{}

/**
 * @brief                   Dereferences and returns the value held by the encapsulated node
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @return avl_tree::AVL<val_t>::iterator::ref_t Data held by tree node the iterator points to
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator::ref_t
avl_tree::AVL<val_t>::iterator::operator* () const
{
    return mPtr->val;
}

/**
 * @brief                   Post increment operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 *
 * @return avl_tree::AVL<val_t>::iterator Incremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::iterator::operator++ ()
{
    // if not pointing to end(), then get the next greater node
    if (mPtr != nullptr) {
        mPtr = mTreePtr->first_greater_strict_ptr (mPtr->val);                  // if a valid node is pointed to (not end()), then get the next greater node
    }
    return *this;
}

/**
 * @brief                   Post increment operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 *
 * @return avl_tree::AVL<val_t>::iterator Incremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::iterator::operator++ (int)
{
    iterator cpy (this->mPtr, this->mTreePtr);                                  // create a copy of the current iterator,
    ++(*this);                                                                  // increment it,
    return cpy;                                                                 // and finally return the copy
}

/**
 * @brief                   Post decrement operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 *
 * @return avl_tree::AVL<val_t>::iterator Decremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::iterator::operator-- ()
{
    if (mPtr != nullptr) {                                                      // if the node being being pointed to is valid,
        node_ptr_t t    {mTreePtr->last_smaller_strict_ptr (mPtr->val)};        // try to get the next smaller node
        mPtr            = (t != nullptr) ? (t) : (mPtr);                        // if such a node exists, use it
    } else {                                                                    // else, if the current node is not valid (instance points to end())
        mPtr = mTreePtr->find_max ();                                           // then get the greateest (last) element of the tree
    }

    return *this;
}

/**
 * @brief                   Pre decrement operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 *
 * @return avl_tree::AVL<val_t>::iterator Decremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::iterator::operator-- (int)
{
    iterator cpy (this->mPtr, this->mTreePtr);                                  // create a copy of the current iterator,
    --(*this);                                                                  // decrement it,
    return cpy;                                                                 // and finally return the copy
}

/**
 * @brief                   Checks if the iterator points to the same node in the same tree
 *
 * @tparam val_t            Type of data held by the tree instance
 *
 * @param pOther            The iterator to compare to
 *
 * @return true             If both iterators point to the same node of the same tree
 * @return false            If both iterators dont point to the same node or belong to different trees
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::iterator::operator== (const avl_tree::AVL<val_t>::iterator & pOther) const
{
    return (mPtr == pOther.mPtr) and (mTreePtr == pOther.mTreePtr);
}

/**
 * @brief                   Checks if the iterator points to different nodes or belong to different trees
 *
 * @tparam val_t            Type of data held by the tree instance
 *
 * @param pOther            The iterator to compare to
 *
 * @return true             If both iterators point to the different nodes or belong to different trees
 * @return false            If both iterators point to the same node in the same tree
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::iterator::operator!= (const avl_tree::AVL<val_t>::iterator & pOther) const
{
    return (mPtr != pOther.mPtr) or (mTreePtr != pOther.mTreePtr);
}

/**
 * @brief Construct a new avl tree::AVL<val t>::reverse iterator::reverse iterator object
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param pPtr              The pointer to encapsulate (points to node or nullptr)
 * @param pTreePtr          The point to the tree which contains the node
 */
template <typename val_t>
avl_tree::AVL<val_t>::reverse_iterator::reverse_iterator (avl_tree::AVL<val_t>::node_t * pPtr,
                                                          const avl_tree::AVL<val_t> *   pTreePtr) :
    mPtr {pPtr}, mTreePtr {pTreePtr}
{}

/**
 * @brief                   Dereferences and returns the value held by the encapsulated node
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @return avl_tree::AVL<val_t>::reverse_iterator::ref_t Data held by tree node the iterator points to
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::reverse_iterator::ref_t
avl_tree::AVL<val_t>::reverse_iterator::operator* () const
{
    return mPtr->val;
}

/**
 * @brief                   Post increment operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 *
 * @return avl_tree::AVL<val_t>::reverse_iterator Incremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::reverse_iterator
avl_tree::AVL<val_t>::reverse_iterator::operator++ ()
{
    // if not pointing to rend(), then get the next greater node
    if (mPtr != nullptr) {
        mPtr = mTreePtr->last_smaller_strict_ptr (mPtr->val);
    }
    return *this;
}

/**
 * @brief                   Pre increment operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 *
 * @return avl_tree::AVL<val_t>::reverse_iterator Incremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::reverse_iterator
avl_tree::AVL<val_t>::reverse_iterator::operator++ (int)
{
    reverse_iterator cpy (this->mPtr, this->mTreePtr);                          // create a copy of the current iterator,
    ++(*this);                                                                  // increment it,
    return cpy;                                                                 // and finally return the copy
}

/**
 * @brief                   Post decrement operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 *
 * @return avl_tree::AVL<val_t>::iterator Decremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::reverse_iterator
avl_tree::AVL<val_t>::reverse_iterator::operator-- ()
{
    if (mPtr != nullptr) {                                                      // if the node being being pointed to is valid,
        node_ptr_t t    {mTreePtr->first_greater_strict_ptr (mPtr->val)};       // try to get the next greater node
        mPtr            = (t != nullptr) ? (t) : (mPtr);                        // if such a node exists, use it
    } else {                                                                    // else, if the current node is not valid (instance points to rend())
        mPtr = mTreePtr->find_min ();                                           // then get the smallest (first) element of the tree
    }
    return *this;
}

/**
 * @brief                   Pre decrement operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 *
 * @return avl_tree::AVL<val_t>::reverse_iterator Decremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::reverse_iterator
avl_tree::AVL<val_t>::reverse_iterator::operator-- (int)
{
    reverse_iterator cpy (this->mPtr, this->mTreePtr);                          // create a copy of the current iterator,
    --(*this);                                                                  // decrement it,
    return cpy;                                                                 // and finally return the copy
}

/**
 * @brief                   Checks if the iterator points to different nodes or belong to different trees
 *
 * @tparam val_t            Type of data held by the tree instance
 *
 * @param pOther            The iterator to compare to
 *
 * @return true             If both iterators point to the different nodes or belong to different trees
 * @return false            If both iterators point to the same node in the same tree
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::reverse_iterator::operator== (const avl_tree::AVL<val_t>::reverse_iterator & pOther) const
{
    return (mPtr == pOther.mPtr) and (mTreePtr == pOther.mTreePtr);
}

/**
 * @brief Construct a new avl tree::AVL<val t>::reverse iterator::reverse iterator object
 *
 * @tparam val_t            Type of data held by tree instance
 *
 * @param _ptr              The pointer to encapsulate (points to node or nullptr)
 * @param _tree_ptr         The point to the tree which contains the node
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::reverse_iterator::operator!= (const avl_tree::AVL<val_t>::reverse_iterator & pOther) const
{
    return (mPtr != pOther.mPtr) or (mTreePtr != pOther.mTreePtr);
}

#ifdef AVL_TEST_MODE
template <typename val_t>
bool
avl_tree::AVL<val_t>::check_balance (avl_tree::AVL<val_t>::node_ptr_t cur)
{

    uint8_t ldep {};
    uint8_t rdep {};
    bool    flag {1};

    if (cur->lptr != nullptr) {
        flag = check_balance (cur->lptr);
        ldep = 1 + cur->lptr->height;
    }

    if (cur->rptr != nullptr) {
        flag = check_balance (cur->rptr);
        rdep = 1 + cur->rptr->height;
    }

    if (ldep > 1 + rdep)
        return false;
    if (rdep > 1 + ldep)
        return false;
    return flag;
}

template <typename val_t>
bool
avl_tree::AVL<val_t>::check_balance ()
{
    if (mRoot != nullptr) {
        return check_balance (mRoot);
    }
    return 1;
}

#endif

#ifdef AVL_TEST_MODE
template <typename val_t>
val_t
avl_tree::AVL<val_t>::get_root_val ()
{
    return mRoot->val;
}
#endif

#endif                    // Header guard