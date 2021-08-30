#ifndef AVL_VM_TREE_H_INCLUDED
#define AVL_VM_TREE_H_INCLUDED
#include "AVLTreeT.h"
#include <iostream>

struct VirtBlock
{
    void *  address{nullptr};
    size_t  size{0u};
};

namespace std 
{
    inline
    ostream &operator<<(ostream &os, const VirtBlock &b)
    {
        const auto flags = os.flags();
        if( flags & std::cout.boolalpha )
        {
            os << "{" << b.size << ",0x" << b.address << '}';
        }
        else
        {
            os << "{0x" << b.address << "," << b.size << '}';
        }
        return os;
    }
}

struct VMNode
{
    VMNode  *north{nullptr};
    VMNode  *south{nullptr};
    VMNode  *east{nullptr};
    VMNode  *west{nullptr};

    VirtBlock data{};
    int8_t    bf_ew{0};
    int8_t    bf_ns{0};
};

struct VMNodeTraitsEW
{
    using Datatype = VirtBlock;
    using Node = VMNode;

   static bool lt(const Datatype &a, const Datatype &b)
    {
        return (a.address < b.address);
    }

    static Node *createNode(const Datatype &d)
    {
        auto *n = new Node{};
        if(n)
        {
            n->data = d;
        }
        return n;
    }

    static Node *&Left(Node *n)
    {
        return n->east;
    }

    static const Node *Left(const Node *n)
    {
        return n->east;
    }

    static Node *&Right(Node *n)
    {
        return n->west;
    }

    static const Node *Right(const Node *n)
    {
        return n->west;
    }

    static int32_t bf(const Node *n)
    {
        return n->bf_ew;
    }

    static void bf(Node *n, int32_t b)
    {
        n->bf_ew = static_cast<int8_t>(b);
    }

    static Datatype &data(Node *n)
    {
        return n->data;
    }

    static const Datatype &data(const Node *n)
    {
        return n->data;
    }
};

struct VMNodeTraitsNS
{
    using Datatype = VirtBlock;
    using Node = VMNode;

   static bool lt(const Datatype &a, const Datatype &b)
    {
        if( a.size < b.size )
        {
            return true;
        }
        else if( b.size < a.size )
        {
            return false;
        }
        else 
        {
            return a.address < b.address;
        }
    }

    static Node *createNode(const Datatype &d)
    {
        auto *n = new Node{};
        if(n)
        {
            n->data = d;
        }
        return n;
    }

    static Node *&Left(Node *n)
    {
        return n->north;
    }

    static const Node *Left(const Node *n)
    {
        return n->north;
    }

    static Node *&Right(Node *n)
    {
        return n->south;
    }

    static const Node *Right(const Node *n)
    {
        return n->south;
    }

    static int32_t bf(const Node *n)
    {
        return n->bf_ns;
    }

    static void bf(Node *n, int32_t b)
    {
        n->bf_ns = static_cast<int8_t>(b);
    }

    static Datatype &data(Node *n)
    {
        return n->data;
    }

    static const Datatype &data(const Node *n)
    {
        return n->data;
    }
};

class VMVirtAddrManager
{
public:
    using AddressTree = AVLTreeT<VMNode, VMNodeTraitsEW>;
    using BlockSizeTree = AVLTreeT<VMNode, VMNodeTraitsNS>;
    VMVirtAddrManager() = default;
    VMVirtAddrManager(VMVirtAddrManager &&) = default;
    VMVirtAddrManager & operator=(VMVirtAddrManager &&) = default;
    VMVirtAddrManager(const VMVirtAddrManager &) = delete;
    VMVirtAddrManager & operator=(const VMVirtAddrManager &) = delete;
    ~VMVirtAddrManager() = default;

    bool addBlock(const VirtBlock &block);

    AddressTree     addressTree{};
    BlockSizeTree   blockSizeTree{};
};

#endif // AVL_VM_TREE_H_INCLUDED
