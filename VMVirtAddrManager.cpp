#include "VMVirtAddrManager.h"

bool
VMVirtAddrManager::addBlock(const VirtBlock &block)
{
    auto success = false;

    auto *newNode = new VMNode{};
    if( newNode )
    {
        newNode->data = block;
        success = addressTree.insertNode(newNode);
        success = success && blockSizeTree.insertNode(newNode);
    }

    return success;
}
