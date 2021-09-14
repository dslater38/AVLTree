#ifdef __EMSCRIPTEN__

#include "JavascriptTree.h"

bool JavascriptTree::insert(uint64_t val)
{
    auto node = BinaryNode::create(val);
    return insertNode(std::move(node));
}

// bool JavascriptTree::contains(uint64_t val)
// {
//     auto node = BinaryTreeNodeT<uint64_t>::create(val);
//     return static_cast<bool>(find(node));
// }

bool JavascriptTree::remove(uint64_t val)
{
    auto node = BinaryNode::create(val);
    removeNode(node);
    BinaryNode::destroy(node);
    return true;
}

#endif // #ifdef __EMSCRIPTEN__
