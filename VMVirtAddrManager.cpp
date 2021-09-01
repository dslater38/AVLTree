#include "VMVirtAddrManager.h"
#include <set>

VMVirtAddrManager::~VMVirtAddrManager()
{
    std::set<VMNode *>nodes{};
    for(auto *pNode = addressTree.rootNode(); pNode != nullptr; pNode = addressTree.rootNode() )
    {
        auto *p = addressTree.removeNode(pNode);
        nodes.emplace(p);
    }
    for(auto *pNode = blockSizeTree.rootNode(); pNode != nullptr; pNode = blockSizeTree.rootNode() )
    {
        auto *p = blockSizeTree.removeNode(pNode);
        nodes.emplace(p);
    }
    for( auto *pNode : nodes )
    {
        delete pNode;
    }
}

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
