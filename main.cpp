#include <iostream>
// #include "AVLTree.h"
#include "AVLTreeT.h"
#include <cstdlib>
#include <string>

// #include "AVLTreeT.h"

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

auto foo = AVLTreeT<node2>{};

int main(int argc, char **argv)
{
    if(argc > 1)
    {
        AVLTreeT<node2> tree{};
        for( auto i=1; i<argc; ++i )
        {
            char *strend = nullptr;
            uint64_t data = std::strtoull(argv[i], &strend, 10);
            tree.insertNode(data);
            // checkTree(tree.rootNode());
        }

        auto *n= tree.find(2);
        n = tree.find(1);
        n = tree.find(45);
        ((void)n);

        printTree<const node2 *, DefaultNodeTraits<node2>>(tree.rootNode(), std::shared_ptr<Trunk>{}, false);
        // printTree(tree);
    }
    return 0;
}

#if 0

void walkTree(const NodePtr &node, int depth)
{
    if(node)
    {
        if( node ->left )
        {
            walkTree(node->left, depth+1);
        }
        for(auto i=0; i<depth; ++i)
        {
            std::cout << "|--";
        }
        std::cout << node->data << '\n';
        if( node ->right )
        {
            walkTree(node->right, depth+1);
        }
    }
}

#endif

#if 0

static int count__ = 0;
struct Trunk
{
    shared_ptr<Trunk> prev{};
    string str{};
    Trunk()
    {
        ++count__;
    }
    ~Trunk()
    {
        --count__;
    }
    Trunk(shared_ptr<Trunk> prev, string str)
    {
        this->prev = prev;
        this->str = str;
        ++count__;
    }
};


// Helper function to print branches of the binary tree
void showTrunks(const Trunk *p)
{
    if (p == nullptr) {
        return;
    }
 
    showTrunks(p->prev.get());
    cout << p->str;
}

// Recursive function to print a binary tree.
// It uses the inorder traversal.
void printTree(const node* root, shared_ptr<Trunk> prev, bool isLeft)
{
    if (root == nullptr) {
        return;
    }
 
    string prev_str = "    ";
    auto trunk = make_shared<Trunk>(prev, prev_str);
 
    printTree(root->right, trunk, true);
 
    if (!prev) {
        trunk->str = "---";
    }
    else if (isLeft)
    {
        trunk->str = ".---";
        prev_str = "   |";
    }
    else {
        trunk->str = "`---";
        prev->str = prev_str;
    }
 
    showTrunks(trunk.get());
    cout << root->data << endl;
 
    if (prev) {
        prev->str = prev_str;
    }
    trunk->str = "   |";
 
    printTree(root->left, trunk, false);
}

void printTree(const AVLTree &tree)
{
	printTree(tree.rootNode(), unique_ptr<Trunk>{}, false);
}
#endif // 0
