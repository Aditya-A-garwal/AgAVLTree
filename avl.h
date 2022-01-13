#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <functional>

//! MODIFYING THE TREE USING INSERT/ERASE WHILE ITERATING CAUSES UNDEFINED BEHAVIOUR

namespace avl_tree {

    namespace {

    #ifdef AVL_TEST_MODE

    namespace balance_info {

        int32_t ll_count;
        int32_t lr_count;
        int32_t rl_count;
        int32_t rr_count;

        void init() {
            ll_count    = 0;
            lr_count    = 0;
            rl_count    = 0;
            rr_count    = 0;
        }
    };

    #endif // #ifdef AVL_TEST_MODE

    // utility function to return maximum of two elements (ONLY USED FOR COMPARING int_t, not val_t)
    template<   typename val_t >
    val_t max( const val_t &pA, const val_t &pB ) { return ( pA > pB )?(pA):(pB); }

    // utility function to return minimum of two elements (ONLY USED FOR COMPARING int_t, not val_t)
    template<   typename val_t >
    val_t min( const val_t &pA, const val_t &pB ) { return ( pA < pB )?(pA):(pB); }

    // default lessthan comparator used if comparator not supplied
    template<   typename val_t >
    bool default_lessthan( const val_t &pA, const val_t &pB ) { return pA < pB; }

    // default equals comparator used if comparator not supplied
    template< typename val_t, bool ( comp )( const val_t &, const val_t & ) = default_lessthan<val_t> >
    bool default_equals( const val_t &pA, const val_t &pB ) { return !( comp( pA, pB ) ) and !( comp( pB, pA ) ); }

    // structure representing a node_t in the AVL tree (NOT TO BE EXPLICITLY INSTANTIATED)
    // val_t: The type of value held by the tree
    // dep_t: The integral type used to store the depth of the node_t (max of left height + 1 and right height + 1)
    template<   typename val_t,
                typename dep_t >
    struct tree_node_t {

        // val holds the actual value used at the node_t
        val_t           val{};
        // pair of pointers to left and right child (0 is left and 1 is right)
        tree_node_t     *cptr[2]{ nullptr, nullptr };
        // depth of the subtree of this node_t
        dep_t           dep{ 0 };
    };

    // utility function to calculate the left and right heights of a node_t
    // val_t: The type of value held by the tree
    // dep_t: The integral type used to store the depth of the node_t
    // node_t: Abbreviated form of the templatized tree_node_t
    template<   typename val_t,
                typename dep_t,
                typename node_t = tree_node_t<val_t, dep_t> >
    void calc_depth( node_t *pCur, dep_t &pLdep, dep_t &pRdep ) {

        // pCur: pointer to node_t whose left and right child is to be calculated
        // pLdep: Reference to object which holds the left height
        // pRdep: Reference to object which holds the right height

        // if corresponding child exists, get 1 + height of that child, else 0

        pLdep        = ( pCur->cptr[0] != nullptr )?( 1 + pCur->cptr[0]->dep ):( 0 );
        pRdep        = ( pCur->cptr[1] != nullptr )?( 1 + pCur->cptr[1]->dep ):( 0 );
    }

    // utility function to balance the tree if the imbalance is due to a left-left node_t
    // val_t: THe type of value held by the tree
    // dep_t: The Integral type used to store the depth of the node_t
    // node_t: Abbreviated form of the templatized tree_node_t
    template<   typename val_t,
                typename dep_t,
                typename node_t = tree_node_t<val_t, dep_t> >
    void balance_ll( node_t **A ) {

        node_t    *a    = *A;
        node_t    *b    = a->cptr[0];

        dep_t   ldep;
        dep_t   rdep;

        // right child of 'b' becomes left child of 'a'
        a->cptr[0]      = b->cptr[1];

        // 'a' becomes the right child of 'b'
        b->cptr[1]      = a;

        // pointer to 'a' now points to 'b'
        *A              = b;

        // recalculate depths of shifted nodes
        calc_depth<val_t, dep_t>( a, ldep, rdep );
        a->dep          = max( ldep, rdep );
        b->dep          = 1 + max( b->cptr[0]->dep, b->cptr[1]->dep );

        #ifdef AVL_TEST_MODE
        balance_info::ll_count += 1;
        #endif // #ifdef AVL_TEST_MODE
    }

    // utility function to balance the tree if the imbalance is due to a left-right node_t
    // val_t: THe type of value held by the tree
    // dep_t: The Integral type used to store the depth of the node_t
    // node_t: Abbreviated form of the templatized tree_node_t
    template<   typename val_t,
                typename dep_t,
                typename node_t   = tree_node_t<val_t, dep_t> >
    void balance_lr( node_t **A ) {

        node_t    *a    = *A;
        node_t    *b    = a->cptr[0];
        node_t    *c    = b->cptr[1];

        dep_t   ldep;
        dep_t   rdep;

        // left child of 'c' becomes right child of 'b'
        b->cptr[1]      = c->cptr[0];
        // b becomes left child of 'c'
        c->cptr[0]      = b;

        // right child of 'c' becomes left child of 'a'
        a->cptr[0]      = c->cptr[1];
        // a becomes right child of 'c'
        c->cptr[1]      = a;

        // pointer to 'a' now points to 'c'
        *A              = c;

        // recalculate depths of shifted nodes
        calc_depth<val_t, dep_t>( a, ldep, rdep );
        a->dep          = max( ldep, rdep );

        calc_depth<val_t, dep_t>( b, ldep, rdep );
        b->dep          = max( ldep, rdep );

        c->dep          = 1 + max( c->cptr[0]->dep, c->cptr[1]->dep );

        #ifdef AVL_TEST_MODE
        balance_info::lr_count += 1;
        #endif // #ifdef AVL_TEST_MODE
    }

    // utility function to balance the tree if the imbalance is due to a right-left node_t
    // val_t: THe type of value held by the tree
    // dep_t: The Integral type used to store the depth of the node_t
    // node_t: Abbreviated form of the templatized tree_node_t
    template<   typename val_t,
                typename dep_t,
                typename node_t   = tree_node_t<val_t, dep_t> >
    void balance_rl( node_t **A ) {

        node_t    *a    = *A;
        node_t    *b    = a->cptr[1];
        node_t    *c    = b->cptr[0];

        dep_t   ldep;
        dep_t   rdep;

        // left child of 'c' becomes right child of 'a'
        a->cptr[1]      = c->cptr[0];
        // a becomes left child of 'c'
        c->cptr[0]      = a;

        // right child of 'c' becomes left child of 'b'
        b->cptr[0]      = c->cptr[1];
        // b becomes right child of 'c'
        c->cptr[1]      = b;

        // pointer to 'a' now points to 'c'
        *A              = c;

        // recalculate depths of shifted nodes
        calc_depth<val_t, dep_t>( a, ldep, rdep );
        a->dep      = max( ldep, rdep );

        calc_depth<val_t, dep_t>( a, ldep, rdep );
        b->dep      = max( ldep, rdep );

        c->dep      = 1 + max( c->cptr[0]->dep, c->cptr[1]->dep );

        #ifdef AVL_TEST_MODE
        balance_info::rl_count += 1;
        #endif // #ifdef AVL_TEST_MODE
    }

    // utility function to balance the tree if the imbalance is due to a right-right node_t
    // val_t: THe type of value held by the tree
    // dep_t: The Integral type used to store the depth of the node_t
    // node_t: Abbreviated form of the templatized tree_node_t
    template<   typename val_t,
                typename dep_t,
                typename node_t   = tree_node_t<val_t, dep_t> >
    void balance_rr( node_t **A ) {

        node_t  *a      = *A;
        node_t  *b      = a->cptr[1];

        dep_t   ldep;
        dep_t   rdep;

        // left child of 'b' becomes right child of 'a'
        a->cptr[1]      = b->cptr[0];

        // 'a' becomes the left child of 'b'
        b->cptr[0]      = a;

        // pointer to 'a' now points to 'b'
        *A              = b;

        // recalculate depths of shifted nodes
        calc_depth<val_t, dep_t>( a, ldep, rdep );
        a->dep      = max( ldep, rdep );
        b->dep      = 1 + max( b->cptr[0]->dep, b->cptr[1]->dep );

        #ifdef AVL_TEST_MODE
        balance_info::rr_count += 1;
        #endif // #ifdef AVL_TEST_MODE
    }

    };

    // class to store the AVL tree
    // val_t: THe type of value held by the tree
    // comp: The lessthan comparator to be used
    // equals: The equals comparator to be used
    // dep_t: The Integral type used to store the depth of the node_t
    // node_t: Abbreviated form of the templatized tree_node_t
    template<   typename val_t,
                bool ( comp )( const val_t &, const val_t & )       = default_lessthan<val_t>,
                bool ( equals )( const val_t &, const  val_t & )    = default_equals<val_t>,
                typename dep_t                                      = uint16_t,
                typename node_t                                     = tree_node_t<val_t, dep_t> >
    struct AVL {

private:

        node_t          *m_root{ nullptr };
        size_t          m_sz{ 0 };

        using           balance_t   = void ( * )( node_t ** );

        balance_t       m_balance[2][2]{    { balance_ll<val_t, dep_t>, balance_lr<val_t, dep_t> },
                                            { balance_rl<val_t, dep_t>, balance_rr<val_t, dep_t> }
                                        };

        // function to get minimum and maximum nodes in a subtree without changing the tree
        node_t              *find_min( node_t *cur ) const {

            node_t    *res{ cur };
            if( res == nullptr )
                return res;

            while( res->cptr[0] != nullptr )
                res = res->cptr[0];

            return res;
        }
        node_t              *find_max( node_t *cur ) const {

            node_t    *res{ cur };
            if( res == nullptr )
                return res;

            while( res->cptr[1] != nullptr )
                res = res->cptr[1];

            return res;
        }

        node_t              *find_min() const { return find_min( m_root ); }
        node_t              *find_max() const { return find_max( m_root ); }


        // methods for inserting a value into the tree
        bool                insert( node_t **cur, const val_t &val ) {

            // if the current pointet points to null, this is the correct location to insert a node_t
            if( *cur == nullptr ) {

                node_t    *ins      = new (std::nothrow) node_t{ val, {nullptr, nullptr}, 0 };
                // failed insertion
                if( ins == nullptr ) {
                    return 0;
                }

                *cur                = ins;

                ++m_sz;

                return 1;
            }

            // if not equal, try to recursively insert
            // if successful, recalculate depths and m_balance
            // if( insert( &(*cur)->cptr[!comp( val, (*cur)->val )], val ) ) {
            if( !equals( (*cur)->val, val ) and insert( ( comp( val, (*cur)->val ) )?( &(*cur)->cptr[0] ):( &(*cur)->cptr[1]), val ) ) {

                dep_t   ldep;
                dep_t   rdep;

                calc_depth<val_t, dep_t>( *cur, ldep, rdep );

                if( ldep > ( 1 + rdep ) ) {

                    if( comp( val, (*cur)->cptr[0]->val ) ) {
                        m_balance[0][0]( cur );
                    }
                    else {
                        m_balance[0][1]( cur );
                    }

                    --ldep;
                }
                else if( rdep > ( 1 + ldep ) ) {

                    if( comp( val, (*cur)->cptr[1]->val ) ) {
                        m_balance[1][0]( cur );
                    }
                    else {
                        m_balance[1][1]( cur );
                    }

                    --rdep;
                }

                (*cur)->dep = max( ldep, rdep );

                return 1;
            }

            return 0;
        }
        bool                erase( node_t **cur, const val_t &val ) {

            // could not find matching node_t, return 0 to indicate failed deletion
            if( *cur == nullptr )
                return 0;

            // found matching node_t, remove it
            if( equals( (*cur)->val, val ) ) {

                node_t    *nxt{ nullptr };

                // the node_t to be deleted has two children, find inorder successor and move it up
                // todo use inorder predeccessor if more efficient
                if( (*cur)->cptr[0] != nullptr and (*cur)->cptr[1] != nullptr ) {

                    nxt             = find_min_move_up( &(*cur)->cptr[1] );

                    nxt->cptr[0]       = (*cur)->cptr[0];
                    nxt->cptr[1]       = (*cur)->cptr[1];
                }

                // left child but not right child, move it up and delete
                else if( (*cur)->cptr[0] != nullptr and (*cur)->cptr[1] == nullptr ) {
                    nxt             = (*cur)->cptr[0];
                }

                // right child but not left child, move it up and delete
                else if( (*cur)->cptr[1] != nullptr and (*cur)->cptr[0] == nullptr ) {
                    nxt             = (*cur)->cptr[1];
                }

                delete *cur;
                *cur            = nxt;

                --m_sz;
            }

            // try to recursively erase, if could not, return 0 to indicate failed deletion
            else if( !erase( ( comp( val, (*cur)->val ) )?( &(*cur)->cptr[0] ):( &(*cur)->cptr[1] ), val ) ) {
                return 0;
            }

            // in case a node was removed, recalculate heights and rebalance if required
            if( *cur != nullptr ) {

                dep_t   ldep;
                dep_t   rdep;

                dep_t   lldep;
                dep_t   rrdep;

                calc_depth<val_t, dep_t>( *cur, ldep, rdep );

                if( ldep > ( 1 + rdep ) ) {

                    calc_depth<val_t, dep_t>( (*cur)->cptr[0], lldep, rrdep );

                    if( lldep >= rrdep) {
                        m_balance[0][0]( cur );
                    }
                    else {
                        m_balance[0][1]( cur );
                    }

                    --ldep;
                }
                else if( rdep > ( 1 + ldep ) ) {

                    calc_depth<val_t, dep_t>( (*cur)->cptr[1], lldep, rrdep );

                    if( lldep >= rrdep ) {
                        m_balance[1][0]( cur );
                    }
                    else {
                        m_balance[1][1]( cur );
                    }

                    --rdep;
                }

                (*cur)->dep = max( ldep, rdep );
            }

            return 1;
        }

        // functions to get minimum and maximum node_t in a subtree and move their children up (used while erasing)
        node_t              *find_min_move_up( node_t **cur ) {

            node_t    *res;

            // since the left child is not null, we can find atleast one smaller node
            if( (*cur)->cptr[0] != nullptr ) {

                res             = find_min_move_up( &(*cur)->cptr[0] );

                dep_t   ldep;
                dep_t   rdep;

                dep_t   lldep;
                dep_t   rrdep;

                calc_depth<val_t, dep_t>( *cur, ldep, rdep );

                // if( ldep > ( 1 + rdep ) ) {

                //     calc_depth<val_t, dep_t>( (*cur)->cptr[0], lldep, rrdep );

                //     if( lldep >= rrdep ) {
                //         m_balance[0][0]( cur );
                //     }
                //     else {
                //         m_balance[0][1]( cur );
                //     }

                //     --ldep;
                // }

                if( rdep > ( 1 + ldep ) ) {

                    calc_depth<val_t, dep_t>( (*cur)->cptr[1], lldep, rrdep );

                    if( lldep >= rrdep ) {
                        m_balance[1][0]( cur );
                    }
                    else {
                        m_balance[1][1]( cur );
                    }

                    --rdep;
                }

                (*cur)->dep = max( ldep, rdep );
            }

            // this is the smallest node_t, move its right child up and return it
            else {

                res         = *cur;
                *cur        = res->cptr[1];
            }

            return res;
        }
        node_t              *find_min_move_up() { return find_min_move_up( &m_root ); }

        node_t              *find_max_move_up( node_t **cur );
        node_t              *find_max_move_up() { return find_max_move_up( &m_root ); }

        // returns pointer to node_t containing exact value of target
        node_t              *find_ptr( const val_t val ) const {

            node_t    *cur{ m_root };

            // break out if reached beyond a leaf node_t
            while( cur != nullptr ) {

                // found node_t with same value
                if( equals( val, cur->val ) ) {
                    return cur;
                }

                // if specified value is less than current node, go left, else go right
                cur     = cur->cptr[ !comp( val, cur->val )];
            }

            return nullptr;
        }
        // returns pointer to node_t containing least value strictly greater than target
        node_t              *first_greater_strict_ptr( const val_t &val, node_t *cur ) const {

            // reached beyond leaf, return garbage
            if( cur == nullptr )
                return nullptr;

            // current node_t has value <= requirement, go right
            if( comp( cur->val, val ) or equals( cur->val, val ) )
                return first_greater_strict_ptr( val, cur->cptr[1] );

            node_t    *res{ first_greater_strict_ptr( val, cur->cptr[0] ) };

            return ( res != nullptr )?( res ):( cur );
        }
        node_t              *first_greater_strict_ptr( const val_t &val ) const { return first_greater_strict_ptr( val, m_root ); }
        // returns pointer to node_t containing least value greater than or equal to target
        node_t              *first_greater_equals_ptr( const val_t &val, node_t *cur ) const {

            // reached beyond leaf, return garbage
            if( cur == nullptr )
                return nullptr;

            // current node_t has value < requirement, go right
            if( comp( cur->val, val ) )
                return first_greater_equals_ptr( val, cur->cptr[1] );

            node_t    *res{ first_greater_equals_ptr( val, cur->cptr[0] ) };

            return ( res != nullptr )?( res ):( cur );
        }
        node_t              *first_greater_equals_ptr( const val_t &val ) const { return first_greater_equals_ptr( val, m_root ); }
        // returns pointer to node_t containing greatest value strictly less than target
        node_t              *last_smaller_strict_ptr( const val_t &val, node_t *cur ) const {

            // reached beyond leaf, return garbage
            if( cur == nullptr )
                return nullptr;

            // current node_t has value >= requirement, go left
            if( comp( val, cur->val ) or equals( val, cur->val ) )
                return last_smaller_strict_ptr( val, cur->cptr[0] );

            node_t    *res{ last_smaller_strict_ptr( val, cur->cptr[1] ) };

            return ( res != nullptr )?( res ):( cur );
        }
        node_t              *last_smaller_strict_ptr( const val_t &val ) const { return last_smaller_strict_ptr( val, m_root ); }
        // returns pointer to node_t containing greatest value less than or equal to target
        node_t              *last_smaller_equals_ptr( const val_t &val, node_t *cur ) const {

            // reached beyond leaf, return garbage
            if( cur == nullptr )
                return nullptr;

            // if specified value < current node's value, go left
            if( comp( val, cur->val ) )
                return last_smaller_equals_ptr( val, cur->cptr[0] );

            node_t    *res{ last_smaller_equals_ptr( val, cur->cptr[1] ) };

            return ( res != nullptr )?( res ):( cur );
        }
        node_t              *last_smaller_equals_ptr( const val_t &val ) const { return last_smaller_equals_ptr( val, m_root ); }

        void                clear( node_t *cur ) {

            if( cur->cptr[0] != nullptr )
                clear( cur->cptr[0] );
            if( cur->cptr[1] != nullptr )
                clear( cur->cptr[1] );
            delete cur;

        }

public:

        struct iterator {

            using       value_t     = val_t;
            using       ptr_t       = node_t *;
            using       tree_ptr_t  = const AVL<val_t, comp, equals, dep_t, node_t> *;
            using       ref_t       = const val_t &;

        #ifndef AVL_TEST_MODE
        private:
        #endif
            ptr_t       ptr{ nullptr };
            tree_ptr_t  tree_ptr{ nullptr };

        public:

            iterator( ptr_t _ptr, tree_ptr_t _tree_ptr ) : ptr{ _ptr }, tree_ptr{ _tree_ptr } {}

            iterator() = default;

            ref_t operator*() const { return ptr->val; }

            iterator    operator++() {

                if( ptr != nullptr ) {
                    ptr     = tree_ptr->first_greater_strict_ptr( ptr->val );
                }
                return *this;
            }

            iterator    operator++( int ) {

                iterator tmp    = *this;
                ++(*this);
                return tmp;
            }

            iterator    operator--() {

                if( ptr != nullptr ) {

                    ptr_t t = tree_ptr->last_smaller_strict_ptr( ptr->val );

                    if( t != nullptr ) {
                        ptr     = t;
                    }

                    // this points to beginning of container
                    else {}
                }
                else {
                    ptr     = tree_ptr->find_max();
                }

                return *this;
            }

            iterator    operator--( int ) {

                iterator tmp    = *this;
                --(*this);
                return tmp;
            }

            friend bool operator==( const iterator &a, const iterator &b ) { return a.ptr == b.ptr and a.tree_ptr == b.tree_ptr; }
            friend bool operator!=( const iterator &a, const iterator &b ) { return a.ptr != b.ptr or a.tree_ptr != b.tree_ptr; }
        };

        struct reverse_iterator {

            using       value_t     = val_t;
            using       ptr_t       = node_t *;
            using       tree_ptr_t  = const AVL<val_t, comp, equals, dep_t, node_t> *;
            using       ref_t       = const val_t &;

        #ifndef AVL_TEST_MODE
        private:
        #endif

            ptr_t       ptr{ nullptr };
            tree_ptr_t  tree_ptr{ nullptr };

        public:

            reverse_iterator( ptr_t _ptr, tree_ptr_t _tree_ptr ) : ptr{ _ptr }, tree_ptr{ _tree_ptr } {}

            reverse_iterator() = default;


            ref_t               operator*() const { return ptr->val; }

            reverse_iterator    operator++() {

                if( ptr != nullptr ) {
                    ptr     = tree_ptr->last_smaller_strict_ptr( ptr->val );
                }
                return *this;
            }

            reverse_iterator    operator++( int ) {

                reverse_iterator    tmp    = *this;
                ++(*this);
                return tmp;
            }

            reverse_iterator    operator--() {

                if( ptr != nullptr ) {

                    ptr_t   t   = tree_ptr->first_greater_strict_ptr( ptr->val );

                    if( t != nullptr ) {
                        ptr     = t;
                    }
                    else {}
                }
                else {
                    ptr     = tree_ptr->find_min();
                }
                return *this;
            }

            reverse_iterator    operator--( int ) {

                reverse_iterator    tmp     = *this;
                --(*this);
                return tmp;
            }

            friend bool         operator==( const reverse_iterator &a, const reverse_iterator &b ) { return a.ptr == b.ptr and a.tree_ptr == b.tree_ptr; }
            friend bool         operator!=( const reverse_iterator &a, const reverse_iterator &b ) { return a.ptr != b.ptr or a.tree_ptr != b.tree_ptr; }
        };

//! Constructors and iterators (PUBLIC)

        // default simple constructor and delete copy constructor
        AVL()                       = default;
        AVL( const AVL & )          = delete;

        // functions for getting iterators to end and beginning
        iterator        begin() const { return iterator( find_min( m_root ), this ); };
        iterator        end() const { return iterator( nullptr, this ); };

        reverse_iterator rbegin() const { return reverse_iterator( find_max( m_root ), this ); }
        reverse_iterator rend() const { return reverse_iterator( nullptr, this ); }

//! PUBLIC SAFE

        bool            insert( const val_t val ) { return insert( &m_root, val ); }
        bool            erase( const val_t val ) { return erase( &m_root, val ); }

//! Implementations of binary search to return iterators to elements (PUBLIC)
//! return end() on failure
        // returns iterator to element containing exact value of target
        iterator        find( const val_t val ) const
        {
            node_t    *res    = find_ptr( val );
            return iterator( res, this );
        }
        // returns iterator to element containing smallest value strictly greater than target
        iterator        first_greater_strict( const val_t val ) const
        {
            node_t    *res    = first_greater_strict_ptr( val );
            return iterator( res, this );
        }
        // returns iterator to element containing smallest value greater than or equal to target
        iterator        first_greater_equals( const val_t val ) const
        {
            node_t    *res    = first_greater_equals_ptr( val );
            return iterator( res, this );
        }
        // returns iterator to element containing largest value strictly less than target
        iterator        last_smaller_strict( const val_t val ) const
        {
            node_t    *res    = last_smaller_strict_ptr( val );
            return iterator( res, this );
        }
        // returns iterator to element containing largest value less than or equal to target
        iterator        last_smaller_equals( const val_t val ) const
        {
            node_t    *res    = last_smaller_equals_ptr( val );
            return iterator( res, this );
        }

        size_t          size() const { return m_sz; }

        void            clear() { clear( m_root ); m_root = nullptr; m_sz = 0; }

//! helper functions to print tree (TO REMOVE SOON)

        void printPreorder( node_t *cur ) {

            if( cur == nullptr )
                return;

            printPreorder( cur->cptr[0] );
            printPreorder( cur->cptr[1] );

            std::cout << '(';
            if( cur->cptr[0] != nullptr )
                std::cout << cur->cptr[0]->val;
            else
                std::cout << 'x';

            std::cout << ", " << cur->val << ", ";

            if( cur->cptr[1] != nullptr )
                std::cout << cur->cptr[1]->val;
            else
                std::cout << 'x';
            std::cout << ')';

            std::cout << ' ' << cur->dep;

            std::cout << std::endl;
        }

        void printPreorder() {
            printPreorder( m_root );
        }

        void printPostorder( node_t *cur ) {

            if( cur == nullptr )
                return;

            std::cout << '(';
            if( cur->cptr[0] != nullptr )
                std::cout << cur->cptr[0]->val;
            else
                std::cout << 'x';

            std::cout << ", " << cur->val << ", ";

            if( cur->cptr[1] != nullptr )
                std::cout << cur->cptr[1]->val;
            else
                std::cout << 'x';
            std::cout << ')';

            std::cout << ' ' << cur->dep;

            std::cout << std::endl;

            printPostorder( cur->cptr[0] );
            printPostorder( cur->cptr[1] );
        }

        void printPostorder() {
            printPostorder( m_root );
        }

        bool checkBalance( node_t *cur ) {

            dep_t dep[2]{ 0, 0 };
            bool flag{ 1 };

            #pragma unroll
            for( int32_t which = 0; which < 2; ++which ) {

                if( cur->cptr[which] != nullptr ) {
                    flag = checkBalance( cur->cptr[which] );
                    dep[which]  = 1 + cur->cptr[which]->dep;
                }
            }

            if( !( dep[1] <= ( 1 + dep[0] ) or dep[0] <= ( 1 + dep[1] ) ) ) {
                return false;
            }

            return flag;
        }

        bool checkBalance() {
            return checkBalance( m_root );
        }
    };
};

#endif
