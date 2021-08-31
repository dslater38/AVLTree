#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 1
#define NOMINMAX 1
#include <windows.h>
#endif
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <limits>
#include <cerrno>
#include "AVLTreeT.h"
#include "PrintTree.h"
#include "VMVirtAddrManager.h"

using namespace std;

// void walkTree(const NodePtr &node, int depth = 0);
// void printTree(const AVLTree &tree);

struct node2
{
    uint64_t data{0};
    int32_t bf{0};
	node2 *left{nullptr};
    node2 *right{nullptr};
};

static void runTest();
static void test2();

auto foo = AVLTreeT<node2>{};
static auto dottyOutput = false;
int main(int argc, char **argv)
{
    if(argc > 1)
    {
        auto i = 1;
        if( *(argv[1]) == '-' && *(argv[1]+1) == 'd' )
        {
            dottyOutput = true;
        }
#ifdef WIN32
    SetConsoleOutputCP(65001);
#endif        
        AVLTreeT<node2> tree{};
        for( auto i=1; i<argc; ++i )
        {
            char *strend = nullptr;
            const char *str = argv[i];
            errno = 0;
            uint64_t data = std::strtoull(str, &strend, 10);
            if( errno == 0 )
            {
                tree.insertNode(data);
            }
            else
            {
                std::cerr << "Error: bad integer: " << str << std::strerror(errno) << "\n";
            }            
        }

        if( dottyOutput )
        {
            dottyTree(tree);
        }
        else
        {
            printTree<const node2 *, DefaultNodeTraits<node2>>(tree.rootNode(), std::shared_ptr<Trunk>{}, false);
        }        
    }
    else
    {
        test2();
        runTest();
    }
    return 0;
}

static void lowerBound( AVLTreeT<node2> &t, uint64_t v)
{
    const auto *n = t.lowerBound(v);
    if(n)
    {
        cout << "Lower Bound: " << v << " == " << n->data << '\n';
    }
    else
    {
        cout << "Lower Bound: " << v << " Failed\n";
    }
}

static void runTest()
{
    AVLTreeT<node2> tree{};
    for( auto i=0ull; i<=50ull; ++i)
    {
        tree.insertNode(i*2);
    }

    lowerBound(tree, 45);
    lowerBound(tree, 44);
    lowerBound(tree, 46);

    lowerBound(tree, 3);

    lowerBound(tree, 99);
    
}

static void test2()
{
    VMVirtAddrManager mgr{};
    VirtBlock blocks[] = {
        VirtBlock{ reinterpret_cast<void *>(0x0000000000001000), 1ull },
        VirtBlock{ reinterpret_cast<void *>(0x0000000000002000), 64ull },
        VirtBlock{ reinterpret_cast<void *>(0x000000000C002000), 0x1000ull },
        VirtBlock{ reinterpret_cast<void *>(0x000000000C00A000), 0x3ull },
        VirtBlock{ reinterpret_cast<void *>(0x000000000C01A000), 0x128ull }
    };
    const auto count = sizeof(blocks)/sizeof(blocks[0]);

    for( auto i=0; i<count; ++i)
    {
        mgr.addBlock(blocks[i]);    
    }


    std::cout << "digraph {\n";
    std::cout << std::resetiosflags( std::cout.boolalpha );
    dottyTree<const VMNode *, VMNodeTraitsEW>(mgr.addressTree.rootNode());
    std::cout << std::setiosflags( std::cout.boolalpha );
    dottyTree<const VMNode *, VMNodeTraitsNS>(mgr.blockSizeTree.rootNode());
    std::cout << "}" << std::endl;

    // printTree<const VMNode *, VMNodeTraitsEW>(mgr.addressTree.rootNode(), std::shared_ptr<Trunk>{}, false);
    // cout << "======================\n"; 
    // printTree<const VMNode *, VMNodeTraitsNS>(mgr.blockSizeTree.rootNode(), std::shared_ptr<Trunk>{}, false);

    return;

}