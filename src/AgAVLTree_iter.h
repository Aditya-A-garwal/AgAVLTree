/**
 * @file                    AgAVLTree_iter.h
 * @author                  Aditya Agarwal (aditya,agarwal@dumblebots.com)
 * @brief                   Implementation of AgAVLTree forward and reverse iterator methods
 */

/**
 * @brief                   Construct a new avl tree::AVL<val t>::iterator::iterator object
 *
 * @note                    if pPtr is nullptr, then the iterator is said to be pointing to pTreePtr->end() (the logical element after the end in the forward direction)
 *
 * @param pPtr              The pointer to encapsulate (points to node or nullptr)
 * @param PTreeptr          The point to the tree which contains the node
 */
template <typename val_t, auto mComp, auto mEquals>
AgAVLTree<val_t, mComp, mEquals>::iterator::iterator (node_ptr_t pPtr, tree_ptr_t pTreePtr) noexcept :
    mPtr {pPtr}, mTreePtr {pTreePtr}
{}

/**
 * @brief                   Dereferences and returns the value held by the encapsulated node
 *
 * @return AgAVLTree<val_t, mComp, mEquals>::iterator::ref_t Data held by tree node the iterator points to
 */
template <typename val_t, auto mComp, auto mEquals>
typename AgAVLTree<val_t, mComp, mEquals>::iterator::ref_t
AgAVLTree<val_t, mComp, mEquals>::iterator::operator* () const
{
    return mPtr->val;
}

/**
 * @brief                   Prefix increment operator (incremements the pointer to the next inorder node)
 *
 * @return AgAVLTree<val_t, mComp, mEquals>::iterator Incremented Iterator
 */
template <typename val_t, auto mComp, auto mEquals>
typename AgAVLTree<val_t, mComp, mEquals>::iterator
AgAVLTree<val_t, mComp, mEquals>::iterator::operator++ ()
{
    // if not pointing to end(), then get the next greater node
    if (mPtr != nullptr) {
        mPtr = mTreePtr->first_greater_strict_ptr (mPtr->val);                  // if a valid node is pointed to (not end()), then get the next greater node
    }
    return *this;
}

/**
 * @brief                   Suffix increment operator (incremements the pointer to the next inorder node)
 *
 * @return AgAVLTree<val_t, mComp, mEquals>::iterator Incremented Iterator
 */
template <typename val_t, auto mComp, auto mEquals>
typename AgAVLTree<val_t, mComp, mEquals>::iterator
AgAVLTree<val_t, mComp, mEquals>::iterator::operator++ (int)
{
    iterator cpy (this->mPtr, this->mTreePtr);                                  // create a copy of the current iterator,
    ++(*this);                                                                  // increment it,
    return cpy;                                                                 // and finally return the copy
}

/**
 * @brief                   Prefix decrement operator (incremements the pointer to the next inorder node)
 *
 * @return AgAVLTree<val_t, mComp, mEquals>::iterator Decremented Iterator
 */
template <typename val_t, auto mComp, auto mEquals>
typename AgAVLTree<val_t, mComp, mEquals>::iterator
AgAVLTree<val_t, mComp, mEquals>::iterator::operator-- ()
{
    if (mPtr != nullptr) {                                                      // if the node being being pointed to is valid,
        node_ptr_t t    {mTreePtr->last_smaller_strict_ptr (mPtr->val)};        // try to get the next smaller node
        mPtr            = (t != nullptr) ? (t) : (mPtr);                        // if such a node exists, use it
    }
    else {                                                                    // else, if the current node is not valid (instance points to end())
        mPtr = mTreePtr->find_max ();                                           // then get the greateest (last) element of the tree
    }

    return *this;
}

/**
 * @brief                   Suffix decrement operator (incremements the pointer to the next inorder node)
 *
 * @return AgAVLTree<val_t, mComp, mEquals>::iterator Decremented Iterator
 */
template <typename val_t, auto mComp, auto mEquals>
typename AgAVLTree<val_t, mComp, mEquals>::iterator
AgAVLTree<val_t, mComp, mEquals>::iterator::operator-- (int)
{
    iterator cpy (this->mPtr, this->mTreePtr);                                  // create a copy of the current iterator,
    --(*this);                                                                  // decrement it,
    return cpy;                                                                 // and finally return the copy
}

/**
 * @brief                   Checks if the iterator points to the same node in the same tree
 *
 * @param pOther            The iterator to compare to
 *
 * @return true             If both iterators point to the same node of the same tree
 * @return false            If both iterators point to different nodes or belong to different trees
 */
template <typename val_t, auto mComp, auto mEquals>
bool
AgAVLTree<val_t, mComp, mEquals>::iterator::operator== (const AgAVLTree<val_t, mComp, mEquals>::iterator & pOther) const
{
    return (mPtr == pOther.mPtr) && (mTreePtr == pOther.mTreePtr);
}

/**
 * @brief                   Checks if the iterator points to different nodes or belong to different trees
 *
 * @param pOther            The iterator to compare to
 *
 * @return true             If both iterators point to the different nodes or belong to different trees
 * @return false            If both iterators point to the same node in the same tree
 */
template <typename val_t, auto mComp, auto mEquals>
bool
AgAVLTree<val_t, mComp, mEquals>::iterator::operator!= (const AgAVLTree<val_t, mComp, mEquals>::iterator & pOther) const
{
    return (mPtr != pOther.mPtr) || (mTreePtr != pOther.mTreePtr);
}

/**
 * @brief                   Construct a new avl tree::AVL<val t>::reverse_iterator instance
 *
 * @note                    if pPtr is nullptr, then the iterator is said to be pointing to pTreePtr->rend() (the logical element after the end in the reverse direction)
 *
 * @param pPtr              The pointer to encapsulate (points to node or nullptr)
 * @param pTreePtr          The point to the tree which contains the node
 */
template <typename val_t, auto mComp, auto mEquals>
AgAVLTree<val_t, mComp, mEquals>::reverse_iterator::reverse_iterator (node_ptr_t pPtr, tree_ptr_t pTreePtr) noexcept :
    mPtr {pPtr}, mTreePtr {pTreePtr}
{}

/**
 * @brief                   Dereferences and returns the value held by the encapsulated node
 *
 * @return AgAVLTree<val_t, mComp, mEquals>::reverse_iterator::ref_t Data held by tree node the iterator points to
 */
template <typename val_t, auto mComp, auto mEquals>
typename AgAVLTree<val_t, mComp, mEquals>::reverse_iterator::ref_t
AgAVLTree<val_t, mComp, mEquals>::reverse_iterator::operator* () const
{
    return mPtr->val;
}

/**
 * @brief                   Prefix increment operator (incremements the pointer to the next inorder node)
 *
 * @return AgAVLTree<val_t, mComp, mEquals>::reverse_iterator Incremented Iterator
 */
template <typename val_t, auto mComp, auto mEquals>
typename AgAVLTree<val_t, mComp, mEquals>::reverse_iterator
AgAVLTree<val_t, mComp, mEquals>::reverse_iterator::operator++ ()
{
    // if not pointing to rend(), then get the next greater node
    if (mPtr != nullptr) {
        mPtr = mTreePtr->last_smaller_strict_ptr (mPtr->val);
    }
    return *this;
}

/**
 * @brief                   Suffix increment operator (incremements the pointer to the next inorder node)
 *
 * @return AgAVLTree<val_t, mComp, mEquals>::reverse_iterator Incremented Iterator
 */
template <typename val_t, auto mComp, auto mEquals>
typename AgAVLTree<val_t, mComp, mEquals>::reverse_iterator
AgAVLTree<val_t, mComp, mEquals>::reverse_iterator::operator++ (int)
{
    reverse_iterator cpy (this->mPtr, this->mTreePtr);                          // create a copy of the current iterator,
    ++(*this);                                                                  // increment it,
    return cpy;                                                                 // and finally return the copy
}

/**
 * @brief                   Prefix decrement operator (incremements the pointer to the next inorder node)
 *
 * @return AgAVLTree<val_t, mComp, mEquals>::iterator Decremented Iterator
 */
template <typename val_t, auto mComp, auto mEquals>
typename AgAVLTree<val_t, mComp, mEquals>::reverse_iterator
AgAVLTree<val_t, mComp, mEquals>::reverse_iterator::operator-- ()
{
    if (mPtr != nullptr) {                                                      // if the node being being pointed to is valid,
        node_ptr_t t    {mTreePtr->first_greater_strict_ptr (mPtr->val)};       // try to get the next greater node
        mPtr            = (t != nullptr) ? (t) : (mPtr);                        // if such a node exists, use it
    }
    else {                                                                      // else, if the current node is not valid (instance points to rend())
        mPtr = mTreePtr->find_min ();                                           // then get the smallest (first) element of the tree
    }
    return *this;
}

/**
 * @brief                   Suffix decrement operator (incremements the pointer to the next inorder node)
 *
 * @return AgAVLTree<val_t, mComp, mEquals>::reverse_iterator Decremented Iterator
 */
template <typename val_t, auto mComp, auto mEquals>
typename AgAVLTree<val_t, mComp, mEquals>::reverse_iterator
AgAVLTree<val_t, mComp, mEquals>::reverse_iterator::operator-- (int)
{
    reverse_iterator cpy (this->mPtr, this->mTreePtr);                          // create a copy of the current iterator,
    --(*this);                                                                  // decrement it,
    return cpy;                                                                 // and finally return the copy
}

/**
 * @brief                   Checks if the iterator points to different nodes or belong to different trees
 *
 * @param pOther            The iterator to compare to
 *
 * @return true             If both iterators point to the same node in the same tree
 * @return false            If both iterators point to the same nodes of belong to different trees
 */
template <typename val_t, auto mComp, auto mEquals>
bool
AgAVLTree<val_t, mComp, mEquals>::reverse_iterator::operator== (const AgAVLTree<val_t, mComp, mEquals>::reverse_iterator & pOther) const
{
    return (mPtr == pOther.mPtr) && (mTreePtr == pOther.mTreePtr);
}

/**
 * @brief                   Checks if the iterator points to different nodes or belong to different trees
 *
 * @param pOther            The iterator to compare to
 *
 * @return true             If both iterators point to the different nodes or belong to different trees
 * @return false            If both iterators point to the same node in the same tree
 */
template <typename val_t, auto mComp, auto mEquals>
bool
AgAVLTree<val_t, mComp, mEquals>::reverse_iterator::operator!= (const AgAVLTree<val_t, mComp, mEquals>::reverse_iterator & pOther) const
{
    return (mPtr != pOther.mPtr) || (mTreePtr != pOther.mTreePtr);
}
