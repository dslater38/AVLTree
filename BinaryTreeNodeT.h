#ifndef BINARY_TREE_NODE_T_INCLUDED
#define BINARY_TREE_NODE_T_INCLUDED
#pragma once


template<typename Data>
class BinaryTreeNodeT
{
public:
    using Node = BinaryTreeNodeT<Data>;
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

    const Node* left()const { return leftChild;  }
    Node* &left() { return leftChild; }
    const Node* right()const { return rightChild; }
    Node*& right() { return rightChild; }

    int8_t bf()const { return bf_; }
    int8_t &bf() { return bf_; }

    const Data& data()const { return data_; };
    struct traits;
private:
    Data    data_{};
    Node* leftChild{ nullptr };
    Node* rightChild{ nullptr };
    int8_t  bf_{ 0 };
};

namespace std
{
    template<typename Data>
    struct less<BinaryTreeNodeT<Data>>
    {
        constexpr bool operator()(const BinaryTreeNodeT<Data>& lhs, const BinaryTreeNodeT<Data>& rhs) const
        {
            return lhs.data() < rhs.data();
        }
    };
}

template<typename Data, typename... Args>
std::unique_ptr<BinaryTreeNodeT<Data>> make_node(Args &&... args)
{
    return std::make_unique<BinaryTreeNodeT<Data>>(std::forward<Args>(args)...);
}

template<typename Data>
struct BinaryTreeNodeT<Data>::traits
{
    using Node = BinaryTreeNodeT<Data>;
    static const Node* Left(const Node* n)
    {
        return n->left();
    }
    static Node*& Left(Node* n)
    {
        return n->left();
    }
    static Node*& Right(Node* n)
    {
        return n->right();
    }
    static const Node* Right(const Node* n)
    {
        return n->right();
    }
    static int32_t bf(const Node* n)
    {
        return static_cast<int32_t>(n->bf());
    }

    static void bf(Node* n, int32_t b)
    {
        n->bf() = static_cast<int8_t>(b & 0x000000FF);
    }

    static const Data& data(const Node* n)
    {
        return n->data();
    }
};

#endif // BINARY_TREE_NODE_T_INCLUDED
