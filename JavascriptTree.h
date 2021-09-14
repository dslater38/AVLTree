#ifndef JAVASCRIPT_TREE_H_INCLUDED
#define JAVASCRIPT_TREE_H_INCLUDED
#pragma once

#ifdef __EMSCRIPTEN__

#include "AVLTreeT.h"
#include "BinaryTreeNodeT.h"
#include <emscripten/bind.h>

using BinaryNode=BinaryTreeNodeT<uint64_t>;

class JavascriptTree : public AVLTreeT<BinaryNode::NodePtr, BinaryNode::traits>
{
public:
    bool insert(uint64_t val);
    // bool contains(uint64_t val);
    bool remove(uint64_t val);
};


// Binding code
EMSCRIPTEN_BINDINGS(uint_tree) {
  emscripten::class_<JavascriptTree>("UintTree")
    .function("insert", &JavascriptTree::insert)
    .function("remove", &JavascriptTree::remove)
    ;
}

#endif // __EMSCRIPTEN__

#endif // JAVASCRIPT_TREE_H_INCLUDED
