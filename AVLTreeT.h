#ifndef AVLTREET_H_INCLUDED
#define AVLTREET_H_INCLUDED
#pragma once

#include <memory>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

template<typename Node, typename Data=uint64_t > 
struct DefaultNodeTraits
{
    using Datatype = Data;
    
    static bool lt(const Datatype &a, const Datatype &b)
    {
        return a<b;
    }

    static Node *createNode(Data d)
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
        return n->left;
    }

    static const Node *Left(const Node *n)
    {
        return n->left;
    }

    static Node *&Right(Node *n)
    {
        return n->right;
    }

    static const Node *Right(const Node *n)
    {
        return n->right;
    }

    static int32_t bf(const Node *n)
    {
        return n->bf;
    }

    static void bf(Node *n, int32_t b)
    {
        n->bf = b;
    }

    static Data &data(Node *n)
    {
        return n->data;
    }

    static const Data &data(const Node *n)
    {
        return n->data;
    }
};



template<typename Node, typename traits=DefaultNodeTraits<Node,uint64_t>>
class AVLTreeT
{
public:
    using node = Node;
    using Data = typename traits::Datatype;
	AVLTreeT()=default;
	AVLTreeT(const AVLTreeT&)=delete;
	AVLTreeT &operator=(const AVLTreeT&)=delete;
	AVLTreeT(AVLTreeT &&)=default;
	AVLTreeT &operator=(AVLTreeT &&)=default;
	~AVLTreeT() = default;

	bool insertNode(Data data);
	bool  deleteNode(Data data);
	node *find(Data data);
	const node *rootNode()const { return root; }
private:
    node *&Left(node *n) { return traits::Left(n); }
    node *&Right(node *n) { return traits::Right(n); }
    const node *Left(node *n)const { return traits::Left(n); }
    const node *Right(node *n)const { return traits::Right(n); }
    int32_t bf(const node *n) { return traits::bf(n); }
    void bf(node *n, int32_t b)const { traits::bf(n, b); }
private:
	node *findNode(node *r, const Data &data);
	bool insertNode(node *&r,node *p, Data data);
	bool  deleteNode(node *&r,Data data, int32_t &bf);
	node *left_rotate(node *node);
	node *right_rotate(node *node);
	node *right_left_rotate(node *node);
	node *left_right_rotate(node *node);
	void rebalance(node *&node);
	node *stepDeleteLeft(node *target, node *&surrogate, node *&prev, int32_t &bf);
	node *stepDeleteRight(node *target, node *&surrogate, node *&prev, int32_t &bf);
private:
	node	*root{nullptr};

};


template<typename Node, typename traits>
inline
Node *AVLTreeT<Node,traits>::findNode(Node *r, const Data &data)
{
	if(r)
	{
		if( traits::lt(data, traits::data(r)) )
		{
			r = findNode(Left(r),data);
		}
		else if( traits::lt(traits::data(r), data) )
		{
			r = findNode(Right(r), data);
		}
	}
	return r;
}


template<typename Node, typename traits>
inline
bool AVLTreeT<Node,traits>::insertNode(Data data)
{

	auto success = false;
	if(root)
	{
		success = insertNode(root, nullptr, data);
	}
	else
	{
		root = traits::createNode(data);
    	success = true;
	}
	return success;
}

template<typename Node, typename traits>
inline
Node *AVLTreeT<Node,traits>::find(Data data)
{
	return findNode(root, data);
}


/*         left rotate
 *
 *    1              2
 *     \            / \
 *      2    ==>   1   3
 *       \ 
 *        3
 */
template<typename Node, typename traits>
inline
Node *AVLTreeT<Node,traits>::left_rotate(Node *p)
{
	auto *q = Right(p);

	Right(p) = Left(q);
	Left(q) = p;
	// n = q;

	// bf(p) -= 1 + max(0, bf(q));
	bf(p, bf(p) - (1 + max(0, bf(q))) );
	// bf(q) -= 1 - min(0, bf(p));
	bf(q, bf(q) - (1 - min(0, bf(p))) );
	return q;
}

/*               right rotate
 *  
 *          3                2
 *         /                / \
 *        2       ==>      1   3  
 *       /
 *      1
 *    
 */
template<typename Node, typename traits>
inline
Node *AVLTreeT<Node,traits>::right_rotate(Node *p)
{
	auto *q = Left(p);
	Left(p) = Right(q);
	Right(q) = p;

	// bf(p) += 1 - min(0, bf(q));
	bf(p, bf(p) + (1 - min(0, bf(q))));
	// bf(q) += 1 + max(0, bf(p));
	bf(q, bf(q) + (1 + max(0, bf(p))));
	return q;
}


/*            left-right rotate
 * 
 *          (P)                (R)
 *         /   \              /   \
 *      (Q)     4   ==>     (Q)    (P)  
 *      / \                /  \    /  \
 *     1  (R)             1    2  3    4
 *        / \
 *       2   3
 *
 *  note that left-right rotate could be implemented as a call to 
 *  left_rotate(Q) followed by a call to right_rotate(P).
 */
template<typename Node, typename traits>
inline
Node *AVLTreeT<Node,traits>::left_right_rotate(Node *p)
{
	auto *q = Left(p);
	auto *r = Right(q);

	Right(q) = Left(r);
	Left(p) = Right(r);
	Right(r) = p;
	Left(r) = q;

	// bf(q) -= 1 + max(0, bf(r));
	bf(q, bf(q) - (1 + max(0, bf(r))));
	// bf(p) += 1 - min(min(0, bf(r)) - 1, bf(r) + bf(q));
	bf(p, bf(p) + (1 - min(min(0, bf(r)) - 1, bf(r) + bf(q))));
	// bf(r) += max(0, bf(p)) + min(0, bf(q));
	bf(r, bf(r) + (max(0, bf(p)) + min(0, bf(q))));
	return r;
}

/*              right-left rotate
 * 
 *          (P)                   (R)
 *         /   \                 /   \
 *        1     (Q)     ==>    (P)    (Q)  
 *             /  \           /  \    /  \
 *           (R)   4         1    2  3    4
 *           / \
 *          2   3
 *
 *  note that right-left rotate could be implemented as a call to 
 *  right_rotate(Q) followed by a call to left_rotate(P).
 */
template<typename Node, typename traits>
inline
Node *AVLTreeT<Node,traits>::right_left_rotate(Node *p)
{
	auto *q = Right(p);
	auto *r = Left(q);

	Left(q) = Right(r);
	Right(p) = Left(r);
	Left(r) = p;
	Right(r) = q;

	// bf(q) += 1 - min(0, bf(r));
	bf(q, bf(q) + (1 - min(0, bf(r))));
	// bf(p) -= 1 + max(max(0, bf(r)) + 1, bf(r) + bf(q));
	bf(p, bf(p) - (1 + max(max(0, bf(r)) + 1, bf(r) + bf(q))));
	// bf(r) += max(0, bf(q)) + min(0, bf(p));
	bf(r, bf(r) + (max(0, bf(q)) + min(0, bf(p))));
	return r;
}

template<typename Node, typename traits>
inline
void AVLTreeT<Node,traits>::rebalance(Node *&r)
{
	if(r)
	{
		if(bf(r) == -2 )
		{
			r = (bf(Left(r)) == 1)
				? left_right_rotate(r)
				: right_rotate(r);
		}
		else if( bf(r) == 2 )
		{
			r = (bf(Right(r)) == -1)
				? right_left_rotate(r)
				: left_rotate(r);
		}
	}
}

template<typename Node, typename traits>
inline
bool
AVLTreeT<Node,traits>::insertNode(node *&r,node *p, AVLTreeT<Node,traits>::Data data)
{
	auto success = false;
	auto before = 0u;
	if( r )
	{
		before = bf(r);
		if( traits::lt(data, traits::data(r)) )
		{
			if( Left(r) )
			{
				success = insertNode(Left(r), r, data );
			}
			else
			{
				Left(r) =  traits::createNode(data);
				if( Left(r) )
				{
					bf(r, bf(r) - 1 );
					success = true;
				}
			}
		}
		else if( traits::lt(traits::data(r), data) )
		{
			if( Right(r) )
			{
				success = insertNode(Right(r), r, data );
			}
			else
			{
				Right(r) = traits::createNode(data);
				if( Right(r) )
				{
					bf(r, bf(r) + 1 );
					success = true;
				}
			}
		}
		if(success)
		{
			if (p && bf(r) && before == 0)
			{
				// bf(p) += (Left(p) == r ? (-1) : (+1));
				bf(p, bf(p) + (Left(p) == r ? (-1) : (+1)));
			}
			
			rebalance(r);
		}
	}
	return success;
}

template<typename Node, typename traits>
inline
Node *
AVLTreeT<Node,traits>::stepDeleteLeft(node *target, node *&surrogate, node *&prev, int32_t &bf)
{
	node *rm = nullptr;
	
	if( ! Left(surrogate) )
	{
		rm = surrogate;
		std::swap(traits::data(target), traits::data(rm));
		surrogate = Right(surrogate);
		bf = 1;
		ANIMATE(*this, "stepDeleteLeft");
	}
	else
	{
		rm = stepDeleteLeft(target, Left(surrogate), surrogate, bf );
	}
	
	if( bf )
	{
		if( prev == target )
		{
			// bf(prev)--;
			bf(prev, bf(prev) + 1);
		}
		else
		{
			// bf(prev)++;
			bf(prev, bf(prev) - 1);
		}

		rebalance(prev);

		if( bf(prev) == 1 || bf(prev) == -1 )
		{
			bf = 0;
		}
	}
	return rm;
}

template<typename Node, typename traits>
inline
Node *
AVLTreeT<Node,traits>::stepDeleteRight(node *target, node *&surrogate, node *&prev, int32_t &bf)
{
	node *rm = nullptr;
	
	if( ! Right(surrogate) )
	{
		rm = surrogate;
		std::swap(traits::data(target), traits::data(rm));
		surrogate = Left(surrogate);
		bf = 1;
		ANIMATE(*this, "stepDeleteRight");
	}
	else
	{
		rm = stepDeleteRight(target, Right(surrogate), surrogate, bf );
	}
	
	if( bf )
	{
		if( prev == target )
		{
			// bf(prev)++;
			bf(prev, bf(prev) + 1);
		}
		else
		{
			// bf(prev)--;
			bf(prev, bf(prev) - 1);
		}

		rebalance(prev);

		if( bf(prev) == 1 || bf(prev) == -1 )
		{
			bf = 0;
		}
	}
	return rm;
}

template<typename Node, typename traits>
inline
bool
AVLTreeT<Node,traits>::deleteNode(node *&r,AVLTreeT<Node,traits>::Data data, int32_t &bf)
{
	auto success = false;
	auto direction = 0;
	if(r)
	{
		if( traits::lt(data, traits::data(r)) )
		{
			direction = 1;
			success = deleteNode(Left(r), data,bf);
		}
		else if( traits::lt(traits::data(r), data) )
		{
			direction = -1;
			success = deleteNode(Right(r), data,bf);
		}
		else
		{
			node *old = nullptr;
			success = true;
			
			if( ! Left(r) )
			{
				old = r;
				r = Right(r);
				bf = 1;
			}
			else if ( ! Right(r) )
			{
				old =r;
				r = Left(r);
				bf = 1;
			}
			else
			{
				if( bf(r) > 0 )
				{
					old = stepDeleteLeft(r, Right(r), r, bf);
				}
				else
				{
					old = stepDeleteRight(r, Left(r), r, bf);
				}
				
				if( bf(r) == 1 || bf(r) == -1 )
				{
					bf = 0;
				}
			}
			// delete old;
            traits::deleteNode(old);
		}
		
		if( direction )
		{
			if(bf)
			{
				// bf(r) += direction;
				bf(r, bf(r) + direction);
			}
			
			rebalance(r);
			
			if( bf(r) )
			{
				bf = 0;
			}
		}
	}
	return success;
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

// Recursive function to print a binary tree.
// It uses the inorder traversal.
template<typename NodePtr, typename traits>
inline void printTree(const NodePtr root, std::shared_ptr<Trunk> prev, bool isLeft)
{
    if (root == nullptr) {
        return;
    }
 
    string prev_str = "    ";
    auto trunk = make_shared<Trunk>(prev, prev_str);
 
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
    // cout << root->data << endl;
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
	std::cout << endl;

    if (prev) {
        prev->str = prev_str;
    }
    trunk->str = "    │";
 
    printTree<NodePtr, traits>(traits::Left(root), trunk, false);
}

template<typename NodePtr, typename traits>
inline std::string toStr(const NodePtr n)
{
	std::stringstream ss{};
	if(n)
	{
		auto bf = BF<NodePtr, traits>(n);
		
		ss << "\"" << n->data;
		auto str = std::string{""};
		while(bf>0)
		{
			ss << '+';
			--bf;
		}
		while(bf<0)
		{
			ss << '-';
			++bf;
		}
		ss << "\"";
	}
	return ss.str();
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

template<typename NodePtr, typename traits>
inline void dottyTree(const AVLTreeT<NodePtr, traits> &tree)
{
	cout << "digraph {\n";
	dottyTree<const node2 *, DefaultNodeTraits<node2>>(tree.rootNode());
	cout << "}" << endl;
}

#endif // AVLTREET_H_INCLUDED

