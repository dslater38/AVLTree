#ifndef AVLTREET_H_INCLUDED
#define AVLTREET_H_INCLUDED
#pragma once

#include <memory>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <cstring>
#include "PrintTree.h"

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

	bool insertNode(node *data);

	node *find(Data data);
	node *lowerBound(Data data);
	const node *rootNode()const { return root; }

	void printTree()const;

private:
    node *&Left(node *n) { return traits::Left(n); }
    node *&Right(node *n) { return traits::Right(n); }
    const node *Left(node *n)const { return traits::Left(n); }
    const node *Right(node *n)const { return traits::Right(n); }
    int32_t bf(const node *n) { return traits::bf(n); }
    void bf(node *n, int32_t b)const { traits::bf(n, b); }
private:
	node *findNode(node *r, const Data &data);
	node *lowerBound(node *r, const Data &data);
	bool insertNode(node *&r,node *p, node *pData);
	bool  deleteNode(node *&r,Data data, int32_t &bf);
	node *removeNode(node *&r,Data data, int32_t &bf);
	node *left_rotate(node *node);
	node *right_rotate(node *node);
	node *right_left_rotate(node *node);
	node *left_right_rotate(node *node);
	void rebalance(node *&node);
	node *stepDeleteLeft(node *target, node *&surrogate, node *&prev, int32_t &bf);
	node *stepDeleteRight(node *target, node *&surrogate, node *&prev, int32_t &bf);
private:
	node	*root{nullptr};
#if defined(DEBUG) || defined(_DEBUG)
	inline static std::string rebalance_msg="";
	inline void REBALANCEMSG(std::string s) {rebalance_msg=s;}
	inline void ANIMATE(std::string msg) { animateTree(msg ); }
	inline void RANIMATE() { if( !rebalance_msg.empty() ) { animateTree(rebalance_msg); } }
	inline void animateTree(std::string msg)
	{
		std::cout << "======================== "	<< msg << " ========================\n";
		printTree();
	}

#else
	inline void REBALANCEMSG(std::string s) {}
	inline void ANIMATE(std::string msg) {}
	inline void RANIMATE() {}
#endif
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
Node *AVLTreeT<Node,traits>::lowerBound(Node *r, const Data &data)
{
	if(r)
	{
		if( traits::lt(data, traits::data(r)) )
		{
			auto tmp = lowerBound(Left(r),data);
			if(tmp &&
				(traits::lt(data, traits::data(tmp)) ||
				!traits::lt(traits::data(tmp), data)) )
			{
				r = tmp;
			}
		}
		else if( traits::lt(traits::data(r), data) )
		{
			r = lowerBound(Right(r), data);
		}
	}
	return r;
}

template<typename Node, typename traits>
inline
bool AVLTreeT<Node,traits>::insertNode(Data data)
{
	auto *node = traits::createNode(data);
	auto success = insertNode(node);
	if(success)
	{
		if(root)
		{
			success = insertNode(root, nullptr, node);
		}
		else
		{
			root = node;
			ANIMATE("insertNode");
		}
	}
	return success;
}

template<typename Node, typename traits>
inline
bool AVLTreeT<Node,traits>::insertNode(Node *pNewNode)
{
	auto success = (pNewNode != nullptr);
	if(success)
	{
		if(root)
		{
			success = insertNode(root, nullptr, pNewNode);
		}
		else
		{
			root = pNewNode;
		}
	}
	return success;
}
template<typename Node, typename traits>
inline
bool AVLTreeT<Node,traits>::deleteNode(Data data)
{
	auto *node = removeNode(data);
	auto success = (node != nullptr);
	if(success)
	{
		delete node;
	}
	return success;
}


template<typename Node, typename traits>
inline
Node *AVLTreeT<Node,traits>::find(Data data)
{
	return findNode(root, data);
}

template<typename Node, typename traits>
inline
Node *AVLTreeT<Node,traits>::lowerBound(Data data)
{
	return lowerBound(root, data);
}

template<typename Node, typename traits>
inline
void AVLTreeT<Node,traits>::printTree()const
{
	::printTree<const Node *, traits>(rootNode(), std::shared_ptr<Trunk>{}, false);
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

	
	bf(p, bf(p) - (1 + std::max(0, bf(q))) ); // bf(p) -= 1 + max(0, bf(q));
	bf(q, bf(q) - (1 - std::min(0, bf(p))) ); // bf(q) -= 1 - min(0, bf(p));
	REBALANCEMSG("left_rotate");
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

	bf(p, bf(p) + (1 - std::min(0, bf(q)))); // bf(p) += 1 - min(0, bf(q));
	bf(q, bf(q) + (1 + std::max(0, bf(p)))); // bf(q) += 1 + max(0, bf(p));
	REBALANCEMSG("right_rotate");
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

	
	bf(q, bf(q) - (1 + std::max(0, bf(r)))); // bf(q) -= 1 + max(0, bf(r));
	bf(p, bf(p) + (1 - std::min(std::min(0, bf(r)) - 1, bf(r) + bf(q)))); // bf(p) += 1 - min(min(0, bf(r)) - 1, bf(r) + bf(q));
	bf(r, bf(r) + (std::max(0, bf(p)) + std::min(0, bf(q)))); // bf(r) += max(0, bf(p)) + min(0, bf(q));
	REBALANCEMSG("left_right_rotate");
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

	
	bf(q, bf(q) + (1 - std::min(0, bf(r)))); // bf(q) += 1 - min(0, bf(r));
	bf(p, bf(p) - (1 + std::max(std::max(0, bf(r)) + 1, bf(r) + bf(q)))); // bf(p) -= 1 + max(max(0, bf(r)) + 1, bf(r) + bf(q));
	bf(r, bf(r) + (std::max(0, bf(q)) + std::min(0, bf(p)))); // bf(r) += max(0, bf(q)) + min(0, bf(p));
	REBALANCEMSG("right_left_rotate");
	return r;
}

template<typename Node, typename traits>
inline
void AVLTreeT<Node,traits>::rebalance(Node *&r)
{
	REBALANCEMSG("");
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
AVLTreeT<Node,traits>::insertNode(node *&r,node *p, node *pData)
{
	auto success = false;
	auto before = 0u;
	if( r )
	{
		before = bf(r);
		if( traits::lt(traits::data(pData), traits::data(r)) )
		{
			if( Left(r) )
			{
				success = insertNode(Left(r), r, pData );
			}
			else
			{
				Left(r) =  pData;
				bf(r, bf(r) - 1 );
				ANIMATE("insertNode");
				success = true;
			}
		}
		else if( traits::lt(traits::data(r), traits::data(pData)) )
		{
			if( Right(r) )
			{
				success = insertNode(Right(r), r, pData );
			}
			else
			{
				Right(r) = pData;
				bf(r, bf(r) + 1 );
				ANIMATE("insertNode");
				success = true;
			}
		}

		if(success)
		{
			if (p && bf(r) && before == 0)
			{
				bf(p, bf(p) + (Left(p) == r ? (-1) : (+1))); // bf(p) += (Left(p) == r ? (-1) : (+1));
			}
			
			rebalance(r);
			RANIMATE();
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
		ANIMATE("stepDeleteLeft");
	}
	else
	{
		rm = stepDeleteLeft(target, Left(surrogate), surrogate, bf );
	}
	
	if( bf )
	{
		if( prev == target )
		{
			bf(prev, bf(prev) + 1); // bf(prev)--;
		}
		else
		{
			bf(prev, bf(prev) - 1); // bf(prev)++;
		}

		rebalance(prev);
		RANIMATE();

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
		ANIMATE("stepDeleteRight");
	}
	else
	{
		rm = stepDeleteRight(target, Right(surrogate), surrogate, bf );
	}
	
	if( bf )
	{
		if( prev == target )
		{
			bf(prev, bf(prev) + 1); // bf(prev)++;
		}
		else
		{
			bf(prev, bf(prev) - 1); // bf(prev)--;
		}

		rebalance(prev);
		RANIMATE();

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
				ANIMATE("deleteNode");
			}
			else if ( ! Right(r) )
			{
				old =r;
				r = Left(r);
				bf = 1;
				ANIMATE("deleteNode");
			}
			else
			{
				old = ( bf(r) > 0 )
					? stepDeleteLeft(r, Right(r), r, bf)
					: stepDeleteRight(r, Left(r), r, bf);
				
				if( bf(r) == 1 || bf(r) == -1 )
				{
					bf = 0;
				}
			}
            traits::deleteNode(old);
		}
		
		if( direction )
		{
			if(bf)
			{
				bf(r, bf(r) + direction); // bf(r) += direction;
			}
			
			rebalance(r);
			RANIMATE();
			
			if( bf(r) )
			{
				bf = 0;
			}
		}
	}
	return success;
}

template<typename Node, typename traits>
inline
Node *
AVLTreeT<Node,traits>::removeNode(Node *&r,AVLTreeT<Node,traits>::Data data, int32_t &bf)
{
	auto direction = 0;
	node *old = nullptr;
	if(r)
	{
		if( traits::lt(data, traits::data(r)) )
		{
			direction = 1;
			old = removeNode(Left(r), data,bf);
		}
		else if( traits::lt(traits::data(r), data) )
		{
			direction = -1;
			old = removeNode(Right(r), data,bf);
		}
		else if( ! Left(r) )
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
			old =  ( bf(r) > 0 )
					? stepDeleteLeft(r, Right(r), r, bf)
					: stepDeleteRight(r, Left(r), r, bf);

			if( bf(r) == 1 || bf(r) == -1 )
			{
				bf = 0;
			}
		}
		
		if( direction )
		{
			if(bf)
			{
				bf(r, bf(r) + direction); // bf(r) += direction;
			}
			
			rebalance(r);
			
			if( bf(r) )
			{
				bf = 0;
			}
		}
	}
	return old;
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


template<typename NodePtr, typename traits>
inline void dottyTree(const AVLTreeT<NodePtr, traits> &tree)
{
	std::cout << "digraph {\n";
	dottyTree<const NodePtr *, traits>(tree.rootNode());
	std::cout << "}" << std::endl;
}

#endif // AVLTREET_H_INCLUDED

