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

class Comparators {

    public:
    template <typename val_t>
    static bool
    lessthan (const val_t & pA, const val_t & pB)
    {
        return pA < pB;
    }

    // default equals comparator used if comparator not supplied
    template <typename val_t, bool (comp) (const val_t &, const val_t &) = lessthan<val_t>>
    static bool
    equals (const val_t & pA, const val_t & pB)
    {
        return !(comp (pA, pB)) and !(comp (pB, pA));
    }
};

template <typename val_t, bool (comp) (const val_t &, const val_t &) = Comparators::lessthan<val_t>,
          bool (equals) (const val_t &, const val_t &) = Comparators::equals<val_t>>
class AVL {

    private:
    struct tree_node_t {
        tree_node_t * cptr[2] {nullptr, nullptr};
        uint8_t       dep {0};
        val_t         val {};
    };

    using node_t    = tree_node_t;
    using balance_t = void (*) (node_t **);

    node_t *  m_root {nullptr};
    size_t    m_sz {0};
    balance_t m_balance[2][2] {{balance_ll, balance_lr}, {balance_rl, balance_rr}};

    /**
     * @brief           Utility function to return the maximum of two elements
     *
     * @param pA        First element to be compared
     * @param pB        Second element to be compared
     * @return val_t    The maximum of the two supplied arguments
     */
    static val_t
    max (const val_t & pA, const val_t & pB)
    {
        return (pA > pB) ? (pA) : (pB);
    }

    /**
     * @brief           Utility function to return the minimum of two elements
     *
     * @param pA        First element to be compared
     * @param pB        Second element to be compared
     * @return val_t    The maximum of the two supplied arguments
     */
    static val_t
    min (const val_t & pA, const val_t & pB)
    {
        return (pA < pB) ? (pA) : (pB);
    }

    /**
     * @brief           Utility function to calculate the depths of the left and right subtrees of this node
     *
     * @param pCur      Node whose subtrees's heights are to be calculated
     * @param pLdep     Reference to variable where left subtree's depth is kept
     * @param pRdep     Reference to variable where right subtree's depth is kept
     *
     * @note            The calculated depths are of the subtrees of the given node, i.e. they include the supplied node
     */
    static void
    calc_depth (node_t * pCur, uint8_t & pLdep, uint8_t & pRdep)
    {
        // if a child does not exist, return 0 (the corresponding subtree is non-existant)
        pLdep = (pCur->cptr[0] != nullptr) ? (1 + pCur->cptr[0]->dep) : (0);
        pRdep = (pCur->cptr[1] != nullptr) ? (1 + pCur->cptr[1]->dep) : (0);
    }

    /**
     * @brief           Function to balance a node which is left-left heavy
     *
     * @param pRoot     pointer to the edge between the pivot and it's parent
     */
    static void
    balance_ll (node_t ** pRoot)
    {

        uint8_t  ldep;
        uint8_t  rdep;

        node_t * top = *pRoot;
        node_t * bot = top->cptr[0];

        top->cptr[0] = bot->cptr[1];                    // right child of bot becomes left child of top
        bot->cptr[1] = top;                             // top becomes the right child of bot
        *pRoot       = bot;                             // pointer to top now points to bot

        // recalculate depths of shifted nodes
        calc_depth (top, ldep, rdep);
        top->dep = max (ldep, rdep);
        bot->dep = 1 + max (bot->cptr[0]->dep, bot->cptr[1]->dep);

#ifdef AVL_TEST_MODE
        balance_info::ll_count += 1;
#endif                    // #ifdef AVL_TEST_MODE
    }

    /**
     * @brief           Function to balance a node which is left-right heavy
     *
     * @param pRoot     pointer to the edge between the pivot and it's parent
     */
    static void
    balance_lr (node_t ** pRoot)
    {

        node_t * top = *pRoot;
        node_t * mid = top->cptr[0];
        node_t * bot = mid->cptr[1];

        uint8_t  ldep;
        uint8_t  rdep;

        mid->cptr[1] = bot->cptr[0];                    // left child of bot becomes right child of mid
        bot->cptr[0] = mid;                             // mid becomes left child of bot
        top->cptr[0] = bot->cptr[1];                    // right child of bot becomes left child of top
        bot->cptr[1] = top;                             // top becomes right child of bot
        *pRoot       = bot;                             // pointer to top now points to bot

        // recalculate depths of shifted nodes
        calc_depth (top, ldep, rdep);
        top->dep = max (ldep, rdep);

        calc_depth (mid, ldep, rdep);
        mid->dep = max (ldep, rdep);

        bot->dep = 1 + max (bot->cptr[0]->dep, bot->cptr[1]->dep);

#ifdef AVL_TEST_MODE
        balance_info::lr_count += 1;
#endif                    // #ifdef AVL_TEST_MODE
    }

    /**
     * @brief           Function to balance a node which is right-left heavy
     *
     * @param pRoot     pointer to the edge between the pivot and it's parent
     */
    static void
    balance_rl (node_t ** pRoot)
    {

        node_t * top = *pRoot;
        node_t * mid = top->cptr[1];
        node_t * bot = mid->cptr[0];

        uint8_t  ldep;
        uint8_t  rdep;

        top->cptr[1] = bot->cptr[0];                    // left child of bot becomes right child of top
        bot->cptr[0] = top;                             // top becomes left child of bot
        mid->cptr[0] = bot->cptr[1];                    // right child of bot becomes left child of mid
        bot->cptr[1] = mid;                             // mid becomes right child of bot
        *pRoot       = bot;                             // pointer to top now points to bot

        // recalculate depths of shifted nodes
        calc_depth (top, ldep, rdep);
        top->dep = max (ldep, rdep);

        calc_depth (top, ldep, rdep);
        mid->dep = max (ldep, rdep);

        bot->dep = 1 + max (bot->cptr[0]->dep, bot->cptr[1]->dep);

#ifdef AVL_TEST_MODE
        balance_info::rl_count += 1;
#endif                    // #ifdef AVL_TEST_MODE
    }

    /**
     * @brief           Function to balance a node which is right-right heavy
     *
     * @param pRoot     pointer to the edge between the pivot and it's parent
     */
    static void
    balance_rr (node_t ** pRoot)
    {

        node_t * top = *pRoot;
        node_t * bot = top->cptr[1];

        uint8_t  ldep;
        uint8_t  rdep;

        top->cptr[1] = bot->cptr[0];                    // left child of bot becomes right child of top
        bot->cptr[0] = top;                             // top becomes the left child of bot
        *pRoot       = bot;                             // pointer to top now points to bot

        // recalculate depths of shifted nodes
        calc_depth (top, ldep, rdep);
        top->dep = max (ldep, rdep);
        bot->dep = 1 + max (bot->cptr[0]->dep, bot->cptr[1]->dep);

#ifdef AVL_TEST_MODE
        balance_info::rr_count += 1;
#endif                    // #ifdef AVL_TEST_MODE
    }

    /**
     * @brief           Find the minimum value node in a subtree without modifying it
     *
     * @param pRoot     Root of the subtree to be processed
     * @return node_t*  Pointer to minimum value node in the subtree of pRoot
     */
    node_t *
    find_min (node_t * pRoot) const
    {
        node_t * res {pRoot};

        if (res == nullptr) {
            return res;
        }

        while (res->cptr[0] != nullptr) {
            res = res->cptr[0];
        }

        return res;
    }

    /**
     * @brief           Find the maximum value node in a subtree without modifying it
     *
     * @param pRoot     Root of the subtree to be processed
     * @return node_t*  Pointer to maximum value node in the subtree of pRoot
     */
    node_t *
    find_max (node_t * pRoot) const
    {
        node_t * res {pRoot};

        if (res == nullptr) {
            return res;
        }

        while (res->cptr[1] != nullptr) {
            res = res->cptr[1];
        }

        return res;
    }

    /**
     * @brief           Find the minimum value node in the entire tree without modifying it
     *
     * @return node_t*  Pointer to the minimum value node in the entire tree
     */
    node_t *
    find_min () const
    {
        return find_min (m_root);
    }

    /**
     * @brief           Find the maximum value node in the entire tree without modifying it
     *
     * @return node_t*  Pointer to the maximum value node in the entire tree
     */
    node_t *
    find_max () const
    {
        return find_max (m_root);
    }

    bool
    insert (node_t ** cur, const val_t & val)
    {

        // if the current pointet points to null, this is the correct location to insert a node_t
        if (*cur == nullptr) {

            node_t * ins = new (std::nothrow) node_t {{nullptr, nullptr}, 0, val};
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
        if (!equals ((*cur)->val, val) and
            insert ((comp (val, (*cur)->val)) ? (&(*cur)->cptr[0]) : (&(*cur)->cptr[1]), val)) {

            uint8_t ldep;
            uint8_t rdep;

            calc_depth (*cur, ldep, rdep);

            if (ldep > (1 + rdep)) {

                if (comp (val, (*cur)->cptr[0]->val)) {
                    m_balance[0][0](cur);
                } else {
                    m_balance[0][1](cur);
                }

                --ldep;
            } else if (rdep > (1 + ldep)) {

                if (comp (val, (*cur)->cptr[1]->val)) {
                    m_balance[1][0](cur);
                } else {
                    m_balance[1][1](cur);
                }

                --rdep;
            }

            (*cur)->dep = max (ldep, rdep);

            return 1;
        }

        return 0;
    }

    bool
    erase (node_t ** cur, const val_t & val)
    {

        // could not find matching node_t, return 0 to indicate failed deletion
        if (*cur == nullptr)
            return 0;

        // found matching node_t, remove it
        if (equals ((*cur)->val, val)) {

            node_t * nxt {nullptr};

            // the node_t to be deleted has two children, find inorder successor and move it up
            // todo use inorder predeccessor if more efficient
            if ((*cur)->cptr[0] != nullptr and (*cur)->cptr[1] != nullptr) {

                nxt          = find_min_move_up (&(*cur)->cptr[1]);

                nxt->cptr[0] = (*cur)->cptr[0];
                nxt->cptr[1] = (*cur)->cptr[1];
            }

            // left child but not right child, move it up and delete
            else if ((*cur)->cptr[0] != nullptr and (*cur)->cptr[1] == nullptr) {
                nxt = (*cur)->cptr[0];
            }

            // right child but not left child, move it up and delete
            else if ((*cur)->cptr[1] != nullptr and (*cur)->cptr[0] == nullptr) {
                nxt = (*cur)->cptr[1];
            }

            delete *cur;
            *cur = nxt;

            --m_sz;
        }

        // try to recursively erase, if could not, return 0 to indicate failed deletion
        else if (!erase ((comp (val, (*cur)->val)) ? (&(*cur)->cptr[0]) : (&(*cur)->cptr[1]), val)) {
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

                calc_depth ((*cur)->cptr[0], lldep, rrdep);

                if (lldep >= rrdep) {
                    m_balance[0][0](cur);
                } else {
                    m_balance[0][1](cur);
                }

                --ldep;
            } else if (rdep > (1 + ldep)) {

                calc_depth ((*cur)->cptr[1], lldep, rrdep);

                if (lldep >= rrdep) {
                    m_balance[1][0](cur);
                } else {
                    m_balance[1][1](cur);
                }

                --rdep;
            }

            (*cur)->dep = max (ldep, rdep);
        }

        return 1;
    }

    // functions to get minimum and maximum node_t in a subtree and move their children up (used while erasing)
    node_t *
    find_min_move_up (node_t ** cur)
    {

        node_t * res;

        // since the left child is not null, we can find atleast one smaller node
        if ((*cur)->cptr[0] != nullptr) {

            res = find_min_move_up (&(*cur)->cptr[0]);

            uint8_t ldep;
            uint8_t rdep;

            uint8_t lldep;
            uint8_t rrdep;

            calc_depth (*cur, ldep, rdep);

            // if( ldep > ( 1 + rdep ) ) {

            //     calc_depth( (*cur)->cptr[0], lldep, rrdep );

            //     if( lldep >= rrdep ) {
            //         m_balance[0][0]( cur );
            //     }
            //     else {
            //         m_balance[0][1]( cur );
            //     }

            //     --ldep;
            // }

            if (rdep > (1 + ldep)) {

                calc_depth ((*cur)->cptr[1], lldep, rrdep);

                if (lldep >= rrdep) {
                    m_balance[1][0](cur);
                } else {
                    m_balance[1][1](cur);
                }

                --rdep;
            }

            (*cur)->dep = max (ldep, rdep);
        }

        // this is the smallest node_t, move its right child up and return it
        else {

            res  = *cur;
            *cur = res->cptr[1];
        }

        return res;
    }
    node_t *
    find_min_move_up ()
    {
        return find_min_move_up (&m_root);
    }

    node_t * find_max_move_up (node_t ** cur);
    node_t *
    find_max_move_up ()
    {
        return find_max_move_up (&m_root);
    }

    // returns pointer to node_t containing exact value of target
    node_t *
    find_ptr (const val_t val) const
    {

        node_t * cur {m_root};

        // break out if reached beyond a leaf node_t
        while (cur != nullptr) {

            // found node_t with same value
            if (equals (val, cur->val)) {
                return cur;
            }

            // if specified value is less than current node, go left, else go right
            cur = cur->cptr[!comp (val, cur->val)];
        }

        return nullptr;
    }
    // returns pointer to node_t containing least value strictly greater than target
    node_t *
    first_greater_strict_ptr (const val_t & val, node_t * cur) const
    {

        // reached beyond leaf, return garbage
        if (cur == nullptr)
            return nullptr;

        // current node_t has value <= requirement, go right
        if (comp (cur->val, val) or equals (cur->val, val))
            return first_greater_strict_ptr (val, cur->cptr[1]);

        node_t * res {first_greater_strict_ptr (val, cur->cptr[0])};

        return (res != nullptr) ? (res) : (cur);
    }
    node_t *
    first_greater_strict_ptr (const val_t & val) const
    {
        return first_greater_strict_ptr (val, m_root);
    }
    // returns pointer to node_t containing least value greater than or equal to target
    node_t *
    first_greater_equals_ptr (const val_t & val, node_t * cur) const
    {

        // reached beyond leaf, return garbage
        if (cur == nullptr)
            return nullptr;

        // current node_t has value < requirement, go right
        if (comp (cur->val, val))
            return first_greater_equals_ptr (val, cur->cptr[1]);

        node_t * res {first_greater_equals_ptr (val, cur->cptr[0])};

        return (res != nullptr) ? (res) : (cur);
    }
    node_t *
    first_greater_equals_ptr (const val_t & val) const
    {
        return first_greater_equals_ptr (val, m_root);
    }
    // returns pointer to node_t containing greatest value strictly less than target
    node_t *
    last_smaller_strict_ptr (const val_t & val, node_t * cur) const
    {

        // reached beyond leaf, return garbage
        if (cur == nullptr)
            return nullptr;

        // current node_t has value >= requirement, go left
        if (comp (val, cur->val) or equals (val, cur->val))
            return last_smaller_strict_ptr (val, cur->cptr[0]);

        node_t * res {last_smaller_strict_ptr (val, cur->cptr[1])};

        return (res != nullptr) ? (res) : (cur);
    }
    node_t *
    last_smaller_strict_ptr (const val_t & val) const
    {
        return last_smaller_strict_ptr (val, m_root);
    }
    // returns pointer to node_t containing greatest value less than or equal to target
    node_t *
    last_smaller_equals_ptr (const val_t & val, node_t * cur) const
    {

        // reached beyond leaf, return garbage
        if (cur == nullptr)
            return nullptr;

        // if specified value < current node's value, go left
        if (comp (val, cur->val))
            return last_smaller_equals_ptr (val, cur->cptr[0]);

        node_t * res {last_smaller_equals_ptr (val, cur->cptr[1])};

        return (res != nullptr) ? (res) : (cur);
    }
    node_t *
    last_smaller_equals_ptr (const val_t & val) const
    {
        return last_smaller_equals_ptr (val, m_root);
    }

    void
    clear (node_t * cur)
    {

        if (cur->cptr[0] != nullptr)
            clear (cur->cptr[0]);
        if (cur->cptr[1] != nullptr)
            clear (cur->cptr[1]);
        delete cur;
    }

    public:
    struct iterator {

        using value_t    = val_t;
        using ptr_t      = node_t *;
        using tree_ptr_t = const AVL<val_t, comp, equals> *;
        using ref_t      = const val_t &;

#ifndef AVL_TEST_MODE
        private:
#endif
        ptr_t      ptr {nullptr};
        tree_ptr_t tree_ptr {nullptr};

        public:
        iterator (ptr_t _ptr, tree_ptr_t _tree_ptr) : ptr {_ptr}, tree_ptr {_tree_ptr}
        {}

        iterator () = default;

        ref_t
        operator* () const
        {
            return ptr->val;
        }

        iterator
        operator++ ()
        {

            if (ptr != nullptr) {
                ptr = tree_ptr->first_greater_strict_ptr (ptr->val);
            }
            return *this;
        }

        iterator
        operator++ (int)
        {

            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator
        operator-- ()
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

        iterator
        operator-- (int)
        {

            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        friend bool
        operator== (const iterator & a, const iterator & b)
        {
            return a.ptr == b.ptr and a.tree_ptr == b.tree_ptr;
        }
        friend bool
        operator!= (const iterator & a, const iterator & b)
        {
            return a.ptr != b.ptr or a.tree_ptr != b.tree_ptr;
        }
    };

    struct reverse_iterator {

        using value_t    = val_t;
        using ptr_t      = node_t *;
        using tree_ptr_t = const AVL<val_t, comp, equals> *;
        using ref_t      = const val_t &;

#ifndef AVL_TEST_MODE
        private:
#endif

        ptr_t      ptr {nullptr};
        tree_ptr_t tree_ptr {nullptr};

        public:
        reverse_iterator (ptr_t _ptr, tree_ptr_t _tree_ptr) : ptr {_ptr}, tree_ptr {_tree_ptr}
        {}

        reverse_iterator () = default;


        ref_t
        operator* () const
        {
            return ptr->val;
        }

        reverse_iterator
        operator++ ()
        {

            if (ptr != nullptr) {
                ptr = tree_ptr->last_smaller_strict_ptr (ptr->val);
            }
            return *this;
        }

        reverse_iterator
        operator++ (int)
        {

            reverse_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        reverse_iterator
        operator-- ()
        {

            if (ptr != nullptr) {

                ptr_t t = tree_ptr->first_greater_strict_ptr (ptr->val);

                if (t != nullptr) {
                    ptr = t;
                } else {
                }
            } else {
                ptr = tree_ptr->find_min ();
            }
            return *this;
        }

        reverse_iterator
        operator-- (int)
        {

            reverse_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        friend bool
        operator== (const reverse_iterator & a, const reverse_iterator & b)
        {
            return a.ptr == b.ptr and a.tree_ptr == b.tree_ptr;
        }
        friend bool
        operator!= (const reverse_iterator & a, const reverse_iterator & b)
        {
            return a.ptr != b.ptr or a.tree_ptr != b.tree_ptr;
        }
    };

    //! Constructors and iterators (PUBLIC)

    AVL ()            = default;

    AVL (const AVL &) = delete;

    // functions for getting iterators to end and beginning
    iterator
    begin () const
    {
        return iterator (find_min (m_root), this);
    };

    iterator
    end () const
    {
        return iterator (nullptr, this);
    };

    reverse_iterator
    rbegin () const
    {
        return reverse_iterator (find_max (m_root), this);
    }

    reverse_iterator
    rend () const
    {
        return reverse_iterator (nullptr, this);
    }

    bool
    insert (const val_t val)
    {
        return insert (&m_root, val);
    }

    bool
    erase (const val_t val)
    {
        return erase (&m_root, val);
    }

    iterator
    find (const val_t val) const
    {
        node_t * res = find_ptr (val);
        return iterator (res, this);
    }

    iterator
    first_greater_strict (const val_t val) const
    {
        node_t * res = first_greater_strict_ptr (val);
        return iterator (res, this);
    }

    iterator
    first_greater_equals (const val_t val) const
    {
        node_t * res = first_greater_equals_ptr (val);
        return iterator (res, this);
    }

    iterator
    last_smaller_strict (const val_t val) const
    {
        node_t * res = last_smaller_strict_ptr (val);
        return iterator (res, this);
    }

    iterator
    last_smaller_equals (const val_t val) const
    {
        node_t * res = last_smaller_equals_ptr (val);
        return iterator (res, this);
    }

    size_t
    size () const
    {
        return m_sz;
    }

    void
    clear ()
    {
        clear (m_root);
        m_root = nullptr;
        m_sz   = 0;
    }

    //! helper functions to print tree (TO REMOVE SOON)

    void
    printPreorder (node_t * cur)
    {

        if (cur == nullptr)
            return;

        printPreorder (cur->cptr[0]);
        printPreorder (cur->cptr[1]);

        std::cout << '(';
        if (cur->cptr[0] != nullptr)
            std::cout << cur->cptr[0]->val;
        else
            std::cout << 'x';

        std::cout << ", " << cur->val << ", ";

        if (cur->cptr[1] != nullptr)
            std::cout << cur->cptr[1]->val;
        else
            std::cout << 'x';
        std::cout << ')';

        std::cout << ' ' << cur->dep;

        std::cout << std::endl;
    }

    void
    printPreorder ()
    {
        printPreorder (m_root);
    }

    void
    printPostorder (node_t * cur)
    {

        if (cur == nullptr)
            return;

        std::cout << '(';
        if (cur->cptr[0] != nullptr)
            std::cout << cur->cptr[0]->val;
        else
            std::cout << 'x';

        std::cout << ", " << cur->val << ", ";

        if (cur->cptr[1] != nullptr)
            std::cout << cur->cptr[1]->val;
        else
            std::cout << 'x';
        std::cout << ')';

        std::cout << ' ' << cur->dep;

        std::cout << std::endl;

        printPostorder (cur->cptr[0]);
        printPostorder (cur->cptr[1]);
    }

    void
    printPostorder ()
    {
        printPostorder (m_root);
    }

    bool
    checkBalance (node_t * cur)
    {

        uint8_t dep[2] {0, 0};
        bool    flag {1};

#pragma unroll
        for (int32_t which = 0; which < 2; ++which) {

            if (cur->cptr[which] != nullptr) {
                flag       = checkBalance (cur->cptr[which]);
                dep[which] = 1 + cur->cptr[which]->dep;
            }
        }

        if (!(dep[1] <= (1 + dep[0]) or dep[0] <= (1 + dep[1]))) {
            return false;
        }

        return flag;
    }

    bool
    checkBalance ()
    {
        return checkBalance (m_root);
    }
};
};                    // namespace avl_tree

#endif
