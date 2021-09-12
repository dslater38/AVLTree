#include "catch2/catch_all.hpp"
#include "AVLTreeT.h"
#include "PrintTree.h"
#include "VMVirtAddrManager.h"
#include "BinaryTreeNodeT.h"



TEST_CASE("AVLTree1", "[tree]")
{
    VMVirtAddrManager mgr{};
    VirtBlock blocks[] = {
        VirtBlock{ reinterpret_cast<void *>(0x0000000000001000), 1ull },
        VirtBlock{ reinterpret_cast<void *>(0x0000000000002000), 64ull },
        VirtBlock{ reinterpret_cast<void *>(0x000000000C002000), 0x1000ull },
        VirtBlock{ reinterpret_cast<void *>(0x000000000C00A000), 0x3ull },
        VirtBlock{ reinterpret_cast<void *>(0x000000000C01A000), 0x128ull }
    };
    const auto count = sizeof(blocks) / sizeof(blocks[0]);

    for (auto i = 0; i < count; ++i)
    {
        mgr.addBlock(blocks[i]);
    }

    const auto &r = mgr.addressTree.rootNode();

    CHECK( reinterpret_cast<uint64_t>(r->data.address) == 0x0000000000002000);

    //std::cout << "digraph {\n";
    //std::cout << std::resetiosflags(std::cout.boolalpha);
    //dottyTree<const VMNode *, VMNodeTraitsEW>(mgr.addressTree.rootNode());
    //std::cout << std::setiosflags(std::cout.boolalpha);
    //dottyTree<const VMNode *, VMNodeTraitsNS>(mgr.blockSizeTree.rootNode());
    //std::cout << "}" << std::endl;

}