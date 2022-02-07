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

    protected:
    struct tree_node_t {
        tree_node_t * lptr {nullptr};
        tree_node_t * rptr {nullptr};
        uint8_t       dep {0};
        val_t         val;
    };

    using node_t     = tree_node_t;
    using node_ptr_t = node_t *;
    using balance_t  = void (*) (node_ptr_t *);
    using comp_t     = bool (*) (const val_t &, const val_t &);

    public:
    struct iterator {

#ifndef AVL_TEST_MODE
        protected:
#endif

        using value_t    = val_t;
        using ptr_t      = node_ptr_t;
        using tree_ptr_t = const AVL<val_t> *;
        using ref_t      = const val_t &;

        ptr_t      ptr {nullptr};
        tree_ptr_t tree_ptr {nullptr};

        public:
        iterator (ptr_t _ptr, tree_ptr_t _tree_ptr);

        iterator () = default;

        ref_t    operator* () const;

        iterator operator++ ();

        iterator operator++ (int);

        iterator operator-- ();

        iterator operator-- (int);

        bool     operator== (const iterator & pOther) const;

        bool     operator!= (const iterator & pOther) const;
    };

    struct reverse_iterator {

#ifndef AVL_TEST_MODE
        protected:
#endif

        using value_t    = val_t;
        using ptr_t      = node_ptr_t;
        using tree_ptr_t = const AVL<val_t> *;
        using ref_t      = const val_t &;

        ptr_t      ptr {nullptr};
        tree_ptr_t tree_ptr {nullptr};

        public:
        reverse_iterator (ptr_t _ptr, tree_ptr_t _tree_ptr);

        reverse_iterator () = default;


        ref_t            operator* () const;

        reverse_iterator operator++ ();

        reverse_iterator operator++ (int);

        reverse_iterator operator-- ();

        reverse_iterator operator-- (int);

        bool             operator== (const reverse_iterator & pOther) const;

        bool             operator!= (const reverse_iterator & pOther) const;
    };

    AVL ();

    AVL (std::initializer_list<val_t> pVals);

    AVL (std::initializer_list<val_t> pVals, const comp_t pLtComp, const comp_t pEqComp);

    AVL (const comp_t pLtComp, const comp_t pEqComp);

    AVL (const AVL &) = delete;

    size_t           size () const;

    void             clear ();

    iterator         begin () const;

    iterator         end () const;

    reverse_iterator rbegin () const;

    reverse_iterator rend () const;

    bool             insert (const val_t val);

    bool             erase (const val_t val);

    iterator         find (const val_t val) const;

    iterator         first_greater_strict (const val_t val) const;

    iterator         first_greater_equals (const val_t val) const;

    iterator         last_smaller_strict (const val_t val) const;

    iterator         last_smaller_equals (const val_t val) const;

    bool             check_balance (node_ptr_t cur);

    bool             check_balance ();

    val_t            get_root_val ();

    private:
    node_ptr_t  m_root {nullptr};
    size_t      m_sz {0};

    comp_t      comp;
    comp_t      equals;

    static bool default_lessthan (const val_t & pA, const val_t & pB);

    static bool default_equals (const val_t & pA, const val_t & pB);

    template <typename arg_t>
    static arg_t max (const arg_t & pA, const arg_t & pB);

    template <typename arg_t>
    static arg_t min (const arg_t & pA, const arg_t & pB);

    static void  calc_depth (node_ptr_t pCur, uint8_t & pLdep, uint8_t & pRdep);

    static void  balance_ll (node_ptr_t * pRoot);

    static void  balance_lr (node_ptr_t * pRoot);

    static void  balance_rl (node_ptr_t * pRoot);

    static void  balance_rr (node_ptr_t * pRoot);

    node_ptr_t   find_min (node_ptr_t pRoot) const;

    node_ptr_t   find_max (node_ptr_t pRoot) const;

    node_ptr_t   find_min () const;

    node_ptr_t   find_max () const;

    bool         insert (node_ptr_t * cur, const val_t & val);

    bool         erase (node_ptr_t * cur, const val_t & val);

    node_ptr_t   find_min_move_up (node_ptr_t * cur);

    node_ptr_t   find_min_move_up ();

    node_ptr_t   find_max_move_up (node_ptr_t * cur);

    node_ptr_t   find_max_move_up ();

    node_ptr_t   find_ptr (const val_t & val) const;

    node_ptr_t   first_greater_strict_ptr (const val_t & val, node_ptr_t cur) const;

    node_ptr_t   first_greater_strict_ptr (const val_t & val) const;

    node_ptr_t   first_greater_equals_ptr (const val_t & val, node_ptr_t cur) const;

    node_ptr_t   first_greater_equals_ptr (const val_t & val) const;

    node_ptr_t   last_smaller_strict_ptr (const val_t & val, node_ptr_t cur) const;

    node_ptr_t   last_smaller_strict_ptr (const val_t & val) const;

    node_ptr_t   last_smaller_equals_ptr (const val_t & val, node_ptr_t cur) const;

    node_ptr_t   last_smaller_equals_ptr (const val_t & val) const;

    void         clear (node_ptr_t cur);
};
};                    // namespace avl_tree

#endif


//! PUBLIC

/**
 * @brief                   Construct a new avl tree::AVL<val t>::AVL object
 *
 * @tparam val_t
 */
template <typename val_t>
avl_tree::AVL<val_t>::AVL () : comp {default_lessthan}, equals {default_equals}
{}

/**
 * @brief                   Construct a new avl tree::AVL<val t>::AVL object
 *
 * @tparam val_t
 * @param pLtComp
 * @param pEqComp
 */
template <typename val_t>
avl_tree::AVL<val_t>::AVL (const comp_t pLtComp, const comp_t pEqComp) : comp {pLtComp}, equals {pEqComp}
{}

/**
 * @brief Construct a new avl tree::AVL<val t>::AVL object
 *
 * @tparam val_t
 * @param pVals
 */
template <typename val_t>
avl_tree::AVL<val_t>::AVL (std::initializer_list<val_t> pVals) : comp {default_lessthan}, equals {default_equals}
{
    for (auto & e : pVals) {
        insert (e);
    }
}

/**
 * @brief Construct a new avl tree::AVL<val t>::AVL object
 *
 * @tparam val_t
 * @param pVals
 * @param pLtComp
 * @param pEqComp
 */
template <typename val_t>
avl_tree::AVL<val_t>::AVL (std::initializer_list<val_t> pVals, const comp_t pLtComp, const comp_t pEqComp) :
    comp {pLtComp}, equals {pEqComp}
{
    for (auto & e : pVals) {
        insert (e);
    }
}

/**
 * @brief                   Returns the size of the tree (number of elements)
 *
 * @tparam val_t            Type of data held by tree instance
 * @return size_t           Size of tree
 */
template <typename val_t>
size_t
avl_tree::AVL<val_t>::size () const
{
    return m_sz;
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
    clear (m_root);
    m_root = nullptr;
    m_sz   = 0;
}

/**
 * @brief                   Attempts to insert a value into the tree
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               The value to be inserted into the tree
 * @return true             If insertion was successful
 * @return false            If insertion failed
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::insert (val_t val)
{
    return insert (&m_root, val);
}

/**
 * @brief                   Attempts to erase a value from the tree
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               The value to be found and erased
 * @return true             If value was successfuly erased
 * @return false            If value could not be successfuly erased (likely not found)
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::erase (val_t val)
{
    return erase (&m_root, val);
}

/**
 * @brief                   Finds and returns an iterator to a given value (end() if no match exists in the tree)
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               The value to be found
 * @return avl_tree::AVL<val_t>::iterator Iterator to matching value in the tree (end() if no match found)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::find (val_t val) const
{
    node_ptr_t res = find_ptr (val);
    return iterator (res, this);
}

/**
 * @brief                   Finds and returns an iterator to the first value strictly greater than the given value (end() if no match exists)
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               The value to be compared with
 * @return avl_tree::AVL<val_t>::iterator Iterator to first strictly greater value in the tree (end() if no match found)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::first_greater_strict (val_t val) const
{
    node_ptr_t res = first_greater_strict_ptr (val);
    return iterator (res, this);
}

/**
 * @brief                   Finds and returns an iterator to the first value greater than or equal to the given value (end() if no match exists)
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               The value to be compared with
 * @return avl_tree::AVL<val_t>::iterator Iterator to first greater or equal value in the tree (end() if no match found)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::first_greater_equals (val_t val) const
{
    node_ptr_t res = first_greater_equals_ptr (val);
    return iterator (res, this);
}

/**
 * @brief                   Finds and returns an iterator to the last value strictly less than the given value (end() if no match exists)
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               The value to be compared with
 * @return avl_tree::AVL<val_t>::iterator Iterator to last strictly less value in the tree (end() if no match found)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::last_smaller_strict (val_t val) const
{
    node_ptr_t res = last_smaller_strict_ptr (val);
    return iterator (res, this);
}

/**
 * @brief                   Finds and returns an iterator to the last value less than or equal to the given value (end() if no match exists)
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               The value to be compared with
 * @return avl_tree::AVL<val_t>::iterator Iterator to last less or equal value in the tree (end() if no match found)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::last_smaller_equals (val_t val) const
{
    node_ptr_t res = last_smaller_equals_ptr (val);
    return iterator (res, this);
}

/**
 * @brief                   Returns an iterator to the beginning
 *
 * @tparam val_t            Type of data held by tree instance
 * @return avl_tree::AVL<val_t>::iterator Iterator to the first element
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::begin () const
{
    return iterator (find_min (m_root), this);
}

/**
 * @brief                   Returns an iterator to the element one after the last element
 *
 * @tparam val_t            Type of data held by tree instance
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
 * @return avl_tree::AVL<val_t>::reverse_iterator Reverse Iterator to the beginning
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::reverse_iterator
avl_tree::AVL<val_t>::rbegin () const
{
    return reverse_iterator (find_max (m_root), this);
}

/**
 * @brief                   Returns a reverse iterator to the element one after the last element
 *
 * @tparam val_t            Type of data held by tree instance
 * @return avl_tree::AVL<val_t>::reverse_iterato Iterator to the element following the last element
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::reverse_iterator
avl_tree::AVL<val_t>::rend () const
{
    return reverse_iterator (nullptr, this);
}

//! PRIVATE

/**
 * @brief                   Default comparator function for less than comparison if none given by user (requires < operator to be implemented)
 *
 * @tparam val_t            Type of data held by tree instance
 * @param pA                First element (element to be compared to)
 * @param pB                Second element (element to be compared)
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
 * @brief                   Default comparator function for equals comparison if none given by user (requires == operator to be implemented)
 *
 * @tparam val_t            Type of data held by tree instance
 * @param pA                First element (element to be compared to)
 * @param pB                Second element (element to be compared)
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
    //     return !(comp (pA, pB)) and !(comp (pB, pA));
    // }
    return pA == pB;
}

/**
 * @brief                   Returns the maximum of two elements (first if both compare equal)
 *
 * @tparam val_t            Type of data held by tree instance
 * @tparam arg_t            Type of values to be compared
 * @param pA                First element to be compared
 * @param pB                Second element to be compared
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
 * @brief                   Returns the minimum of two elements (first if both compare equals)
 *
 * @tparam val_t            Type of data held by tree instance
 * @tparam arg_t            Type of values to be compared
 * @param pA                First element to be compared
 * @param pB                Second element to be compared
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
 * @param pCur              Node whose subtrees's heights are to be calculated
 * @param pLdep             Reference to variable where left subtree's depth is kept
 * @param pRdep             Reference to variable where right subtree's depth is kept
 *
 * @note                    The calculated depths are of the subtrees of the given node, i.e. they include the supplied node
 */
template <typename val_t>
void
avl_tree::AVL<val_t>::calc_depth (avl_tree::AVL<val_t>::node_ptr_t pCur, uint8_t & pLdep, uint8_t & pRdep)
{
    // if a child does not exist, return 0 (the corresponding subtree is non-existant)
    pLdep = (pCur->lptr != nullptr) ? (1 + pCur->lptr->dep) : (0);
    pRdep = (pCur->rptr != nullptr) ? (1 + pCur->rptr->dep) : (0);
}

/**
 * @brief                   Function to balance a node which is left-left heavy
 *
 * @tparam val_t            Type of data held by tree instance
 * @param pRoot             Pointer to the edge between the pivot and it's parent
 */
template <typename val_t>
void
avl_tree::AVL<val_t>::balance_ll (avl_tree::AVL<val_t>::node_ptr_t * pRoot)
{
    uint8_t    ldep;
    uint8_t    rdep;

    node_ptr_t top = *pRoot;
    node_ptr_t bot = top->lptr;

    top->lptr      = bot->rptr;                    // right child of bot becomes left child of top
    bot->rptr      = top;                          // top becomes the right child of bot
    *pRoot         = bot;                          // pointer to top now points to bot

    // recalculate depths of shifted nodes
    calc_depth (top, ldep, rdep);
    top->dep = max (ldep, rdep);

    // calc_depth (bot, ldep, rdep);
    // bot->dep = max (ldep, rdep);
    bot->dep = 1 + max (bot->lptr->dep, bot->rptr->dep);

#ifdef AVL_TEST_MODE
    balance_info::ll_count += 1;
#endif                    // #ifdef AVL_TEST_MODE
}

/**
 * @brief                   Function to balance a node which is left-right heavy
 *
 * @tparam val_t            Type of data held by tree instance
 * @param pRoot             Pointer to the edge between the pivot and it's parent
 */
template <typename val_t>
void
avl_tree::AVL<val_t>::balance_lr (avl_tree::AVL<val_t>::node_ptr_t * pRoot)
{
    node_ptr_t top = *pRoot;
    node_ptr_t mid = top->lptr;
    node_ptr_t bot = mid->rptr;

    uint8_t    ldep;
    uint8_t    rdep;

    mid->rptr = bot->lptr;                    // left child of bot becomes right child of mid
    bot->lptr = mid;                          // mid becomes left child of bot
    top->lptr = bot->rptr;                    // right child of bot becomes left child of top
    bot->rptr = top;                          // top becomes right child of bot
    *pRoot    = bot;                          // pointer to top now points to bot

    // recalculate depths of shifted nodes
    calc_depth (top, ldep, rdep);
    top->dep = max (ldep, rdep);

    calc_depth (mid, ldep, rdep);
    mid->dep = max (ldep, rdep);

    // calc_depth (bot, ldep, rdep);
    // bot->dep = max (ldep, rdep);
    bot->dep = 1 + max (bot->lptr->dep, bot->rptr->dep);

#ifdef AVL_TEST_MODE
    balance_info::lr_count += 1;
#endif                    // #ifdef AVL_TEST_MODE
}

/**
 * @brief                   Function to balance a node which is right-left heavy
 *
 * @tparam val_t            Type of data held by tree instance
 * @param pRoot             Pointer to the edge between the pivot and it's parent
 */
template <typename val_t>
void
avl_tree::AVL<val_t>::balance_rl (avl_tree::AVL<val_t>::node_ptr_t * pRoot)
{
    node_ptr_t top = *pRoot;
    node_ptr_t mid = top->rptr;
    node_ptr_t bot = mid->lptr;

    uint8_t    ldep;
    uint8_t    rdep;

    top->rptr = bot->lptr;                    // left child of bot becomes right child of top
    bot->lptr = top;                          // top becomes left child of bot
    mid->lptr = bot->rptr;                    // right child of bot becomes left child of mid
    bot->rptr = mid;                          // mid becomes right child of bot
    *pRoot    = bot;                          // pointer to top now points to bot

    // recalculate depths of shifted nodes
    calc_depth (top, ldep, rdep);
    top->dep = max (ldep, rdep);

    calc_depth (mid, ldep, rdep);
    mid->dep = max (ldep, rdep);

    // calc_depth (bot, ldep, rdep);
    // bot->dep = max (ldep, rdep);
    bot->dep = 1 + max (bot->lptr->dep, bot->rptr->dep);

#ifdef AVL_TEST_MODE
    balance_info::rl_count += 1;
#endif                    // #ifdef AVL_TEST_MODE
}

/**
 * @brief                   Function to balance a node which is right-right heavy
 *
 * @tparam val_t            Type of data held by tree instance
 * @param pRoot             Pointer to the edge between the pivot and it's parent
 */
template <typename val_t>
void
avl_tree::AVL<val_t>::balance_rr (avl_tree::AVL<val_t>::node_ptr_t * pRoot)
{
    node_ptr_t top = *pRoot;
    node_ptr_t bot = top->rptr;

    uint8_t    ldep;
    uint8_t    rdep;

    top->rptr = bot->lptr;                    // left child of bot becomes right child of top
    bot->lptr = top;                          // top becomes the left child of bot
    *pRoot    = bot;                          // pointer to top now points to bot

    // recalculate depths of shifted nodes
    calc_depth (top, ldep, rdep);
    top->dep = max (ldep, rdep);

    // calc_depth (bot, ldep, rdep);
    // bot->dep = max (ldep, rdep);

    bot->dep = 1 + max (bot->lptr->dep, bot->rptr->dep);

#ifdef AVL_TEST_MODE
    balance_info::rr_count += 1;
#endif                    // #ifdef AVL_TEST_MODE
}

/**
 * @brief                   Finds the minimum value node in a subtree without modifying it
 *
 * @tparam val_t            Type of data held by tree instance
 * @param pRoot             Root of the subtree to be processed
 * @return node_ptr_t       Pointer to minimum value node in the subtree of pRoot
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::find_min (avl_tree::AVL<val_t>::node_ptr_t pRoot) const
{
    node_ptr_t res {pRoot};

    if (res == nullptr) {
        return res;
    }

    while (res->lptr != nullptr) {
        res = res->lptr;
    }

    return res;
}

/**
 * @brief                   Finds the minimum value node in the entire tree without modifying it
 *
 * @tparam val_t            Type of data held by tree instance
 * @return node_ptr_t       Pointer to the minimum value node in the entire tree
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::find_min () const
{
    return find_min (m_root);
}

/**
 * @brief                   Finds the maximum value node in a subtree without modifying it
 *
 * @tparam val_t            Type of data held by tree instance
 * @param pRoot             Root of the subtree to be processed
 * @return node_ptr_t       Pointer to maximum value node in the subtree of pRoot
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::find_max (avl_tree::AVL<val_t>::node_ptr_t pRoot) const
{
    node_ptr_t res {pRoot};

    if (res == nullptr) {
        return res;
    }

    while (res->rptr != nullptr) {
        res = res->rptr;
    }

    return res;
}

/**
 * @brief                   Finds the maximum value node in the entire tree without modifying it
 *
 * @tparam val_t            Type of data held by tree instance
 * @return node_ptr_t       Pointer to the maximum value node in the entire tree
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::find_max () const
{
    return find_max (m_root);
}

/**
 * @brief                   Attempts to insert a new node in the subtree of an existing node
 *
 * @tparam val_t            Type of data held by tree instance
 * @param cur               Pointer to node's link in whose subtree a new value must be inserted
 * @param val               Reference to value to be inserted
 * @return true             If insertion was successful (new node created)
 * @return false            If insertion failed
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::insert (avl_tree::AVL<val_t>::node_ptr_t * cur, const val_t & val)
{

    // if the current pointet points to null, this is the correct location to insert a node_t
    if (*cur == nullptr) {

        node_ptr_t ins = new (std::nothrow) node_t {nullptr, nullptr, 0, val};
        // failed insertion
        if (ins == nullptr) {
            return 0;
        }

        *cur = ins;

        ++m_sz;

        return 1;
    }

    // if not equal, try to recursively insert
    // if successful, recalculate depths and m_balance
    // if( insert( &(*cur)->cptr[!comp( val, (*cur)->val )], val ) ) {
    if (!equals ((*cur)->val, val) and insert ((comp (val, (*cur)->val)) ? (&(*cur)->lptr) : (&(*cur)->rptr), val)) {

        uint8_t ldep;
        uint8_t rdep;

        calc_depth (*cur, ldep, rdep);

        if (ldep > (1 + rdep)) {

            if (comp (val, (*cur)->lptr->val)) {
                balance_ll (cur);
            } else {
                balance_lr (cur);
            }

            --ldep;
        } else if (rdep > (1 + ldep)) {

            if (comp (val, (*cur)->rptr->val)) {
                balance_rl (cur);
            } else {
                balance_rr (cur);
            }

            --rdep;
        }

        (*cur)->dep = max (ldep, rdep);

        return 1;
    }

    return 0;
}


/**
 * @brief                   Attempts to erase a node from the subtree of an existing node
 *
 * @tparam val_t            Type of data held by tree instance
 * @param cur               Pointer to node's link in whose subtree a new value must be inserted
 * @param val               Reference to value to be erased
 * @return true             If erasing was successful (old node deleted)
 * @return false            If erasing failed
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::erase (avl_tree::AVL<val_t>::node_ptr_t * cur, const val_t & val)
{

    // could not find matching node_t, return 0 to indicate failed deletion
    if (*cur == nullptr)
        return 0;

    // found matching node_t, remove it
    if (equals ((*cur)->val, val)) {

        node_ptr_t nxt {nullptr};

        // the node_t to be deleted has two children, find inorder successor and move it up
        // todo use inorder predeccessor if more efficient
        if ((*cur)->lptr != nullptr and (*cur)->rptr != nullptr) {

            nxt       = find_min_move_up (&(*cur)->rptr);

            nxt->lptr = (*cur)->lptr;
            nxt->rptr = (*cur)->rptr;
        }

        // left child but not right child, move it up and delete
        else if ((*cur)->lptr != nullptr and (*cur)->rptr == nullptr) {
            nxt = (*cur)->lptr;
        }

        // right child but not left child, move it up and delete
        else if ((*cur)->rptr != nullptr and (*cur)->lptr == nullptr) {
            nxt = (*cur)->rptr;
        }

        delete *cur;
        *cur = nxt;

        --m_sz;
    }

    // try to recursively erase, if could not, return 0 to indicate failed deletion
    else if (!erase ((comp (val, (*cur)->val)) ? (&(*cur)->lptr) : (&(*cur)->rptr), val)) {
        return 0;
    }

    // in case a node was removed, recalculate heights and rebalance if required
    if (*cur != nullptr) {

        uint8_t ldep;
        uint8_t rdep;

        uint8_t lldep;
        uint8_t rrdep;

        calc_depth (*cur, ldep, rdep);

        if (ldep > (1 + rdep)) {

            calc_depth ((*cur)->lptr, lldep, rrdep);

            if (lldep >= rrdep) {
                balance_ll (cur);
            } else {
                balance_lr (cur);
            }

            --ldep;
        } else if (rdep > (1 + ldep)) {

            calc_depth ((*cur)->rptr, lldep, rrdep);

            if (lldep >= rrdep) {
                balance_rl (cur);
            } else {
                balance_rr (cur);
            }

            --rdep;
        }

        (*cur)->dep = max (ldep, rdep);
    }

    return 1;
}

/**
 * @brief                   Finds inorder successor of a node and replace the node with it
 *
 * @tparam val_t            Type of data held by tree instance
 * @param cur               Pointer to node's link which is to be replaced
 * @return avl_tree::AVL<val_t>::node_ptr_t Inorder succesor of given node
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::find_min_move_up (avl_tree::AVL<val_t>::node_ptr_t * cur)
{

    node_ptr_t res;

    // since the left child is not null, we can find atleast one smaller node
    if ((*cur)->lptr != nullptr) {

        res = find_min_move_up (&(*cur)->lptr);

        uint8_t ldep;
        uint8_t rdep;

        uint8_t lldep;
        uint8_t rrdep;

        calc_depth (*cur, ldep, rdep);

        // if( ldep > ( 1 + rdep ) ) {

        //     calc_depth( (*cur)->lptr, lldep, rrdep );

        //     if( lldep >= rrdep ) {
        //         balance_ll( cur );
        //     }
        //     else {
        //         balance_lr( cur );
        //     }

        //     --ldep;
        // }

        if (rdep > (1 + ldep)) {

            calc_depth ((*cur)->rptr, lldep, rrdep);

            if (lldep > rrdep) {
                balance_rl (cur);
            } else {
                balance_rr (cur);
            }

            --rdep;
        }

        (*cur)->dep = max (ldep, rdep);
    }

    // this is the smallest node_t, move its right child up and return it
    else {

        res  = *cur;
        *cur = res->rptr;
    }

    return res;
}

/**
 * @brief                   Finds inorder successor of the root
 *
 * @tparam val_t            Type of data held by tree instance
 * @return avl_tree::AVL<val_t>::node_ptr_t Inorder succesor of the root
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::find_min_move_up ()
{
    return find_min_move_up (&m_root);
}

/**
 * @brief                   Finds a node with value equal to the given value
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               Value to find
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to the node with equal value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::find_ptr (const val_t & val) const
{

    node_ptr_t cur {m_root};

    // break out if reached beyond a leaf node_t
    while (cur != nullptr) {

        // found node_t with same value
        if (equals (val, cur->val)) {
            return cur;
        }

        // if specified value is less than current node, go left, else go right
        cur = (comp (val, cur->val)) ? (cur->lptr) : (cur->rptr);
        // cur = cur->cptr[!comp (val, cur->val)];
    }

    return nullptr;
}

/**
 * @brief                   Finds a node with value strictly greater than the given value in an existing node's subtree
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               Value to find
 * @param cur               Pointer to node in whose subtree the search must take place
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a strictly greater value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::first_greater_strict_ptr (const val_t & val, avl_tree::AVL<val_t>::node_ptr_t cur) const
{

    // reached beyond leaf, return garbage
    if (cur == nullptr)
        return nullptr;

    // current node_t has value <= requirement, go right
    if (comp (cur->val, val) or equals (cur->val, val))
        return first_greater_strict_ptr (val, cur->rptr);

    node_ptr_t res {first_greater_strict_ptr (val, cur->lptr)};

    return (res != nullptr) ? (res) : (cur);
}

/**
 * @brief                   Finds a node with value strictly greater than the given value
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               Value to find
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a strictly greater value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::first_greater_strict_ptr (const val_t & val) const
{
    return first_greater_strict_ptr (val, m_root);
}

/**
 * @brief                   Finds a node with value not less than the given value in an existing node's subtree
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               Value to find
 * @param cur               Pointer to node in whose subtree the search must take place
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a greater or equal value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::first_greater_equals_ptr (const val_t & val, avl_tree::AVL<val_t>::node_ptr_t cur) const
{

    // reached beyond leaf, return garbage
    if (cur == nullptr)
        return nullptr;

    // current node_t has value < requirement, go right
    if (comp (cur->val, val))
        return first_greater_equals_ptr (val, cur->rptr);

    node_ptr_t res {first_greater_equals_ptr (val, cur->lptr)};

    return (res != nullptr) ? (res) : (cur);
}

/**
 * @brief                   Finds a node with value not less than the given value
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               Value to find
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a greater or equal value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::first_greater_equals_ptr (const val_t & val) const
{
    return first_greater_equals_ptr (val, m_root);
}

/**
 * @brief                   Finds a node with value strictly less than the given value in an existing node's subtree
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               Value to find
 * @param cur               Pointer to node in whose subtree the search must take place
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a strictly less value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::last_smaller_strict_ptr (const val_t & val, avl_tree::AVL<val_t>::node_ptr_t cur) const
{

    // reached beyond leaf, return garbage
    if (cur == nullptr)
        return nullptr;

    // current node_t has value >= requirement, go left
    if (comp (val, cur->val) or equals (val, cur->val))
        return last_smaller_strict_ptr (val, cur->lptr);

    node_ptr_t res {last_smaller_strict_ptr (val, cur->rptr)};

    return (res != nullptr) ? (res) : (cur);
}

/**
 * @brief                   Finds a node with value strictly less than the given value
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               Value to find
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a strictly less value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::last_smaller_strict_ptr (const val_t & val) const
{
    return last_smaller_strict_ptr (val, m_root);
}

/**
 * @brief                   Finds a node with value not more than the given value in an existing node's subtree
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               Value to find
 * @param cur               Pointer to node in whose subtree the search must take place
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a less or equal value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::last_smaller_equals_ptr (const val_t & val, avl_tree::AVL<val_t>::node_ptr_t cur) const
{

    // reached beyond leaf, return garbage
    if (cur == nullptr)
        return nullptr;

    // if specified value < current node's value, go left
    if (comp (val, cur->val))
        return last_smaller_equals_ptr (val, cur->lptr);

    node_ptr_t res {last_smaller_equals_ptr (val, cur->rptr)};

    return (res != nullptr) ? (res) : (cur);
}

/**
 * @brief                   Finds a node with value not more than the given value
 *
 * @tparam val_t            Type of data held by tree instance
 * @param val               Value to find
 * @return avl_tree::AVL<val_t>::node_ptr_t Pointer to node with a less or equal value (or nullptr in the case of no match)
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::node_ptr_t
avl_tree::AVL<val_t>::last_smaller_equals_ptr (const val_t & val) const
{
    return last_smaller_equals_ptr (val, m_root);
}

/**
 * @brief                   Clear the subtree of an entire node (including the node) by deleting
 *
 * @tparam val_t            Type of data held by tree instance
 * @param cur               Pointer to node whose subtree is to be deleted
 */
template <typename val_t>
void
avl_tree::AVL<val_t>::clear (avl_tree::AVL<val_t>::node_ptr_t cur)
{

    if (cur->lptr != nullptr)
        clear (cur->lptr);
    if (cur->rptr != nullptr)
        clear (cur->rptr);
    delete cur;
}


//! Iterators

/**
 * @brief                   Construct a new avl tree::AVL<val t>::iterator::iterator object
 *
 * @tparam val_t            Type of data held by tree instance
 * @param _ptr              The pointer to encapsulate (points to node or nullptr)
 * @param _tree_ptr         The point to the tree which contains the node
 */
template <typename val_t>
avl_tree::AVL<val_t>::iterator::iterator (avl_tree::AVL<val_t>::node_t * _ptr, const avl_tree::AVL<val_t> * _tree_ptr) :
    ptr {_ptr}, tree_ptr {_tree_ptr}
{}

/**
 * @brief                   Dereferences and returns the value held by the encapsulated node
 *
 * @tparam val_t            Type of data held by tree instance
 * @return avl_tree::AVL<val_t>::iterator::ref_t Data held by tree node the iterator points to
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator::ref_t
avl_tree::AVL<val_t>::iterator::operator* () const
{
    return ptr->val;
}

/**
 * @brief                   Post increment operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 * @return avl_tree::AVL<val_t>::iterator Incremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::iterator::operator++ ()
{

    if (ptr != nullptr) {
        ptr = tree_ptr->first_greater_strict_ptr (ptr->val);
    }
    return *this;
}

/**
 * @brief                   Post increment operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 * @return avl_tree::AVL<val_t>::iterator Incremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::iterator::operator++ (int)
{

    iterator tmp = *this;
    ++(*this);
    return tmp;
}

/**
 * @brief                   Post decrement operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 * @return avl_tree::AVL<val_t>::iterator Decremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::iterator::operator-- ()
{

    if (ptr != nullptr) {

        ptr_t t = tree_ptr->last_smaller_strict_ptr (ptr->val);

        if (t != nullptr) {
            ptr = t;
        }

        // this points to beginning of container
        else {
        }
    } else {
        ptr = tree_ptr->find_max ();
    }

    return *this;
}

/**
 * @brief                   Pre decrement operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 * @return avl_tree::AVL<val_t>::iterator Decremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::iterator
avl_tree::AVL<val_t>::iterator::operator-- (int)
{

    iterator tmp = *this;
    --(*this);
    return tmp;
}

/**
 * @brief                   Checks if the iterator points to the same node in the same tree
 *
 * @tparam val_t            Type of data held by the tree instance
 * @param pOther            The iterator to compare to
 * @return true             If both iterators point to the same node of the same tree
 * @return false            If both iterators dont point to the same node or belong to different trees
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::iterator::operator== (const avl_tree::AVL<val_t>::iterator & pOther) const
{
    return ptr == pOther.ptr and tree_ptr == pOther.tree_ptr;
}

/**
 * @brief                   Checks if the iterator points to different nodes or belong to different trees
 *
 * @tparam val_t            Type of data held by the tree instance
 * @param pOther            The iterator to compare to
 * @return true             If both iterators point to the different nodes or belong to different trees
 * @return false            If both iterators point to the same node in the same tree
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::iterator::operator!= (const avl_tree::AVL<val_t>::iterator & pOther) const
{
    return ptr != pOther.ptr or tree_ptr != pOther.tree_ptr;
}

/**
 * @brief Construct a new avl tree::AVL<val t>::reverse iterator::reverse iterator object
 *
 * @tparam val_t            Type of data held by tree instance
 * @param _ptr              The pointer to encapsulate (points to node or nullptr)
 * @param _tree_ptr         The point to the tree which contains the node
 */
template <typename val_t>
avl_tree::AVL<val_t>::reverse_iterator::reverse_iterator (avl_tree::AVL<val_t>::node_t * _ptr,
                                                          const avl_tree::AVL<val_t> *   _tree_ptr) :
    ptr {_ptr}, tree_ptr {_tree_ptr}
{}

/**
 * @brief                   Dereferences and returns the value held by the encapsulated node
 *
 * @tparam val_t            Type of data held by tree instance
 * @return avl_tree::AVL<val_t>::reverse_iterator::ref_t Data held by tree node the iterator points to
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::reverse_iterator::ref_t
avl_tree::AVL<val_t>::reverse_iterator::operator* () const
{
    return ptr->val;
}

/**
 * @brief                   Post increment operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 * @return avl_tree::AVL<val_t>::reverse_iterator Incremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::reverse_iterator
avl_tree::AVL<val_t>::reverse_iterator::operator++ ()
{

    if (ptr != nullptr) {
        ptr = tree_ptr->last_smaller_strict_ptr (ptr->val);
    }
    return *this;
}

/**
 * @brief                   Pre increment operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 * @return avl_tree::AVL<val_t>::reverse_iterator Incremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::reverse_iterator
avl_tree::AVL<val_t>::reverse_iterator::operator++ (int)
{

    reverse_iterator tmp = *this;
    ++(*this);
    return tmp;
}

/**
 * @brief                   Post decrement operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 * @return avl_tree::AVL<val_t>::iterator Decremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::reverse_iterator
avl_tree::AVL<val_t>::reverse_iterator::operator-- ()
{

    if (ptr != nullptr) {

        ptr_t t = tree_ptr->first_greater_strict_ptr (ptr->val);

        if (t != nullptr) {
            ptr = t;
        }
    } else {
        ptr = tree_ptr->find_min ();
    }
    return *this;
}

/**
 * @brief                   Pre decrement operator (incremements the pointer to the next inorder node)
 *
 * @tparam val_t            Type of data held by the tree instance
 * @return avl_tree::AVL<val_t>::reverse_iterator Decremented Iterator
 */
template <typename val_t>
typename avl_tree::AVL<val_t>::reverse_iterator
avl_tree::AVL<val_t>::reverse_iterator::operator-- (int)
{

    reverse_iterator tmp = *this;
    --(*this);
    return tmp;
}

/**
 * @brief                   Checks if the iterator points to different nodes or belong to different trees
 *
 * @tparam val_t            Type of data held by the tree instance
 * @param pOther            The iterator to compare to
 * @return true             If both iterators point to the different nodes or belong to different trees
 * @return false            If both iterators point to the same node in the same tree
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::reverse_iterator::operator== (const avl_tree::AVL<val_t>::reverse_iterator & pOther) const
{
    return ptr == pOther.ptr and tree_ptr == pOther.tree_ptr;
}

/**
 * @brief Construct a new avl tree::AVL<val t>::reverse iterator::reverse iterator object
 *
 * @tparam val_t            Type of data held by tree instance
 * @param _ptr              The pointer to encapsulate (points to node or nullptr)
 * @param _tree_ptr         The point to the tree which contains the node
 */
template <typename val_t>
bool
avl_tree::AVL<val_t>::reverse_iterator::operator!= (const avl_tree::AVL<val_t>::reverse_iterator & pOther) const
{
    return ptr != pOther.ptr or tree_ptr != pOther.tree_ptr;
}

template <typename val_t>
bool
avl_tree::AVL<val_t>::check_balance (avl_tree::AVL<val_t>::node_ptr_t cur)
{

    uint8_t ldep {};
    uint8_t rdep {};
    bool    flag {1};

    if (cur->lptr != nullptr) {
        flag = check_balance (cur->lptr);
        ldep = 1 + cur->lptr->dep;
    }

    if (cur->rptr != nullptr) {
        flag = check_balance (cur->rptr);
        rdep = 1 + cur->rptr->dep;
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
    if (m_root != nullptr) {
        return check_balance (m_root);
    }
    return 1;
}

#ifdef AVL_TEST_MODE
template <typename val_t>
val_t
avl_tree::AVL<val_t>::get_root_val ()
{
    return m_root->val;
}
#endif