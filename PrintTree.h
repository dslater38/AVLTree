#ifndef PRINTTREE_H_INCLUDED
#define PRINTTREE_H_INCLUDED
#pragma once

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
inline std::string toStr(const NodePtr n)
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
		
		ss << "\"" << n->data;
		// auto str = std::string{""};
		// while(bf>0)
		// {
		// 	ss << '+';
		// 	--bf;
		// }
		// while(bf<0)
		// {
		// 	ss << '-';
		// 	++bf;
		// }
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
	std::cout << root->data;
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
	if(n && (traits::Left(n) || traits::Right(n)) )
	{
		auto s = toStr<NodePtr, traits>(n);
		
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
