#ifndef PRINTTREE_H_INCLUDED
#define PRINTTREE_H_INCLUDED
#pragma once

#include <iomanip>

template<typename NodePtr, typename traits>
inline int32_t height(const NodePtr &r)
{
	int32_t h = 0;
	if(r)
	{
		h = std::max(height<NodePtr, traits>(traits::Left(r)), height<NodePtr, traits>(traits::Right(r))) + 1;
	}
	return h;
}

template<typename NodePtr, typename traits>
inline int32_t BF(const NodePtr &r)
{
	int32_t bf = 0;
	if(r)
	{
		bf = height<NodePtr, traits>(traits::Right(r)) - height<NodePtr, traits>(traits::Left(r));
	}
	return bf;
}


struct Trunk
{
    std::shared_ptr<Trunk> prev{};
    std::string str{};
	inline static int count__ = 0;
    Trunk()
    {
        ++count__;
    }
    ~Trunk()
    {
        --count__;
    }
    Trunk(std::shared_ptr<Trunk> prev, std::string str)
    {
        this->prev = prev;
        this->str = str;
        ++count__;
    }
};


// Helper function to print branches of the binary tree
inline void showTrunks(const Trunk *p)
{
    if (p == nullptr) {
        return;
    }
 
    showTrunks(p->prev.get());
    std::cout << p->str;
}

template<typename NodePtr, typename traits>
inline std::string toStr(const NodePtr &n)
{
	std::stringstream ss{};
    
    auto ff = std::cout.flags();
    if( ff & std::cout.boolalpha )
    {
        ss << std::setiosflags( ss.boolalpha );
    }
    else
    {
        ss << std::resetiosflags( ss.boolalpha );
    }

	if(n)
	{
		auto bf = BF<NodePtr, traits>(n);
		
		ss << "\"" << traits::data(n);
		ss << "\"";
	}
	return ss.str();
}

// Recursive function to print a binary tree.
// It uses the inorder traversal.
template<typename NodePtr, typename traits>
inline void printTree(const NodePtr root, std::shared_ptr<Trunk> prev, bool isLeft)
{
    if (root == nullptr) {
        return;
    }
 
    std::string prev_str = "    ";
    auto trunk = std::make_shared<Trunk>(prev, prev_str);
 
    printTree<NodePtr, traits>(traits::Right(root), trunk, true);
 
    if (!prev) {
        trunk->str = "───";
    }
    else if (isLeft)
    {
        trunk->str = "╭───";
        prev_str = "    │";
    }
    else {
        trunk->str = "╰───";
        prev->str = prev_str;
    }
 
    showTrunks(trunk.get());
    // std::cout << root->data << std::endl;
	std::cout << traits::data(root);
	auto bf = BF<NodePtr, traits>(root);
	while(bf>0)
	{
		std::cout << "+";
		--bf;
	}
	while(bf<0)
	{
		std::cout << "-";
		++bf;
	}
	std::cout << std::endl;

    if (prev) {
        prev->str = prev_str;
    }
    trunk->str = "    │";
 
    printTree<NodePtr, traits>(traits::Left(root), trunk, false);
}

template<typename NodePtr, typename traits>
inline void dottyTree(const NodePtr n)
{
	if(n /* && (traits::Left(n) || traits::Right(n)) */ )
	{
		auto s = toStr<NodePtr, traits>(n);
		auto bf = BF<NodePtr, traits>(n);
		std::cout << s;
		switch (bf)
		{
			case -2:
				std::cout << " [style=filled,fillcolor=red]";
				break;
			case -1:
				std::cout << " [style=filled,fillcolor=pink]";
				break;
			case 1:
				std::cout << " [style=filled,fillcolor=cyan]";
				break;
			case 2:
				std::cout << " [style=filled,fillcolor=blue]";
				break;
			default:
				break;
		}
		std::cout << '\n';

		if(traits::Left(n))
		{
			std::cout << '\t' << s << " -> {";
			std::cout << toStr<NodePtr, traits>(traits::Left(n)) << " [color=red] ";
			std::cout << "} [color=red arrowhead=vee]\n";
		}

		if(traits::Right(n))
		{
			std::cout << '\t' << s << " -> {";
			std::cout << toStr<NodePtr, traits>(traits::Right(n)) << " [color=blue]";
			std::cout << "} [color=blue arrowhead=vee]\n";
		}
		dottyTree<NodePtr,traits>(traits::Left(n));
		dottyTree<NodePtr,traits>(traits::Right(n));
	}
}


#endif // PRINTTREE_H_INCLUDED
