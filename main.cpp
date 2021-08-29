#include <iostream>
// #include "AVLTree.h"
#include "AVLTreeT.h"
#include <cstdlib>
#include <string>
#ifdef WIN32
#include <windows.h>
#endif
#include <limits>
#include <cerrno>
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
            // if( data < std::numeric_limits<uint64_t>::max() || errno == 0)
            // {
            //     if(*str != '\0' && strend && *strend=='\0' )
            //     {
                    
            //     }
            //     else
            //     {

            //     }
            // }
            
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
