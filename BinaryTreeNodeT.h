#ifndef BINARY_TREE_NODE_T_INCLUDED
#define BINARY_TREE_NODE_T_INCLUDED
#pragma once
#include <memory>

template<typename Data>
class BinaryTreeNodeT
{
public:
    using Node = BinaryTreeNodeT<Data>;
    using NodePtr = std::unique_ptr<Node>;
    BinaryTreeNodeT() = default;
    BinaryTreeNodeT(const BinaryTreeNodeT &) = default;
    BinaryTreeNodeT &operator=(const BinaryTreeNodeT &) = default;
    BinaryTreeNodeT(BinaryTreeNodeT &&) = default;
    BinaryTreeNodeT &operator=(BinaryTreeNodeT &&) = default;
    ~BinaryTreeNodeT() = default;
    template<typename... Args>
    BinaryTreeNodeT(Args &&... args) 
        : data_{ std::forward<Args>(args)... }
    {
    }

    const NodePtr &left()const { return leftChild;  }
    NodePtr &left() { return leftChild; }
    const NodePtr &right()const { return rightChild; }
    NodePtr &right() { return rightChild; }

    int8_t bf()const { return bf_; }
    int8_t &bf() { return bf_; }

    const Data& data()const { return data_; };
    struct traits;
private:
    Data    data_{};
    NodePtr leftChild{  };
    NodePtr rightChild{  };
    int8_t  bf_{ 0 };
};

namespace std
{
    template<class Data>
    struct less< unique_ptr<BinaryTreeNodeT<Data>> >
    {
        constexpr bool operator()(const unique_ptr<BinaryTreeNodeT<Data>> &lhs, const unique_ptr<BinaryTreeNodeT<Data>> &rhs) const
        {
            return lhs && rhs ? lhs->data() < rhs->data() : false;
        }
    };
}

template<typename Data, typename... Args>
typename BinaryTreeNodeT<Data>::NodePtr make_node(Args &&... args)
{
    return std::make_unique<BinaryTreeNodeT<Data>>(std::forward<Args>(args)...);
}

template<typename Data>
struct BinaryTreeNodeT<Data>::traits
{
    using Node = typename BinaryTreeNodeT<Data>::Node;
    using NodePtr = typename BinaryTreeNodeT<Data>::NodePtr;

    static const NodePtr &Left(const NodePtr &n)
    {
        return n->left();
    }
    static NodePtr &Left(NodePtr &n)
    {
        return n->left();
    }
    static NodePtr &Right(NodePtr &n)
    {
        return n->right();
    }
    static const NodePtr &Right(const NodePtr &n)
    {
        return n->right();
    }
    static int32_t bf(const NodePtr &n)
    {
        return static_cast<int32_t>(n->bf());
    }

    static void bf(NodePtr &n, int32_t b)
    {
        n->bf() = static_cast<int8_t>(b & 0x000000FF);
    }

    static const Data& data(const NodePtr &n)
    {
        return n->data();
    }
};

#endif // BINARY_TREE_NODE_T_INCLUDED
