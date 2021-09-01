#ifndef AVLTREET_H_INCLUDED
#define AVLTREET_H_INCLUDED
#pragma once

#include <memory>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <cstring>
#include <stack>
#include "PrintTree.h"

//template<typename Data>
//struct AVLNodeT
//{
//	Data	data{};
//	AVLNodeT<Data> *left{nullptr};
//	AVLNodeT<Data> *right{nullptr};
//};
//
//namespace std {
//	template<typename Data>
//	struct less<AVLNodeT<Data>>
//	{
//		constexpr bool operator()( const AVLNodeT<Data>& lhs, const AVLNodeT<Data>& rhs ) const
//		{
//			return less<Data>{}(lhs.data, rhs.data);
//		}
//		constexpr bool operator()( const AVLNodeT<Data>* lhs, const AVLNodeT<Data>& rhs ) const
//		{
//			return lhs ? less<Data>{}(lhs->data, rhs.data) : false;
//		}
//		constexpr bool operator()( const AVLNodeT<Data>& lhs, const AVLNodeT<Data>* rhs ) const
//		{
//			return rhs ? less<Data>{}(lhs.data, rhs->data) : false;
//		}
//		constexpr bool operator()( const AVLNodeT<Data>* lhs, const AVLNodeT<Data>* rhs ) const
//		{
//			return (lhs && rhs ) ? less<Data>{}(lhs->data, rhs->data) : false;
//		}
//	};
//}

template<typename Node, typename traits=typename Node::traits, typename DefaultCompare=std::less<Node>>
class AVLTreeT
{
public:
	using value_type = Node;

public:
    using node = Node;
	AVLTreeT()=default;
	AVLTreeT(const AVLTreeT&)=delete;
	AVLTreeT &operator=(const AVLTreeT&)=delete;
	AVLTreeT(AVLTreeT &&)=default;
	AVLTreeT &operator=(AVLTreeT &&)=default;
	~AVLTreeT() = default;

	struct const_iterator;
	struct const_reverse_iterator;

	template<typename Compare=DefaultCompare>
	Node *removeNode(const Node *pNode, const Compare &cmp=Compare{});

	template<typename Compare=DefaultCompare>
	bool insertNode(node *pNode, const Compare &cmp=Compare{});

	template<typename Compare=DefaultCompare>
	node *find(const Node *pNode, const Compare &cmp=Compare{});

	template<typename Compare=DefaultCompare>
	node *lowerBound(const Node *pData, const Compare &cmp=Compare{});

	node *rootNode() { return root; }
	const node *rootNode()const { return root; }

	void printTree()const;
	void dottyTree()const;

	const_iterator begin()const;
	const_iterator end()const;

	const_reverse_iterator rbegin()const;
	const_reverse_iterator rend()const;

private:
    node *&Left(node *n) { return traits::Left(n); }
    node *&Right(node *n) { return traits::Right(n); }
    const node *Left(node *n)const { return traits::Left(n); }
    const node *Right(node *n)const { return traits::Right(n); }
private:
	template<typename Compare>
	node *findNode(node *r, const node *pData, const Compare &cmp);

	template<typename Compare>
	node *lowerBound(node *r, const node *pData, const Compare &cmp);

	template<typename Compare>
	bool insertNode(node *&r,node *p, node *pData, const Compare &cmp);

	// template<typename Compare>
	// bool  deleteNode(node *&r,const node *pData, int32_t &bf, const Compare &cmp);

	template<typename Compare>
	node *removeNode(node *&r,const node *pData, int32_t &bf, const Compare &cmp);

	template<typename Compare>
	node *removeNode(node *&r,node *parent, const node *pData, int32_t &bf, const Compare &cmp);


	node *left_rotate(node *node);
	node *right_rotate(node *node);
	node *right_left_rotate(node *node);
	node *left_right_rotate(node *node);
	void rebalance(node *&node);
	node *stepDeleteLeft(node *&target, node *surrogate, node *prev, int32_t &bf);
	node *stepDeleteRight(node *&target, node *surrogate, node *prev, int32_t &bf);
private:
	node	*root{nullptr};
#if defined(DEBUG) || defined(_DEBUG)
	inline static std::string rebalance_msg="";
	inline void REBALANCEMSG(std::string s) {rebalance_msg=s;}
	inline void ANIMATE(std::string msg) { animateTree(msg ); }
	inline void RANIMATE() { if( !rebalance_msg.empty() ) { animateTree(rebalance_msg); } }
	inline void animateTree(std::string msg)
	{
		// std::cout << "======================== "	<< msg << " ========================\n";
		std::cout << "{\"title\" : \'" << msg << "\', \"dot\" : `";
		dottyTree();
		std::cout << "`},\n";
	}

#else
	inline void REBALANCEMSG(std::string s) {}
	inline void ANIMATE(std::string msg) {}
	inline void RANIMATE() {}
#endif
};

template<typename Node, typename traits, typename DefaultCompare>
struct AVLTreeT<Node, traits, DefaultCompare>::const_iterator
{
	using Tree = AVLTreeT<Node, traits, DefaultCompare>;
	using It = const_iterator;
	const_iterator() = default;
	const_iterator(const It&) = default;
	const_iterator& operator=(const const_iterator&) = default;
	const_iterator(It&&) = default;
	const_iterator& operator=(const_iterator&&) = default;
	~const_iterator() = default;
	bool operator==(const It& o)const
	{
		return o.stack == stack;
	}
	bool operator!=(const It& o)const { return !operator==(o); }
	const Node* operator *()
	{
		return stack.top();
	}
	const Node* operator->()
	{
		return stack.top();
	}
	It& operator++()
	{
		if (!stack.empty())
		{
			auto* n = stack.top();
			stack.pop();
			pushLeftChildren(traits::Right(n));
		}
		return *this;
	}
	It operator++(int)
	{
		It ret{ *this };
		operator++();
		return ret;
	}
	const_iterator(const AVLTreeT<Node, traits, DefaultCompare>& tree)
	{
		pushLeftChildren(tree.rootNode());
	}
private:
	void pushLeftChildren(const Node* pNode)
	{
		for (; pNode; pNode = traits::Left(pNode))
		{
			stack.push(pNode);
		}
	}
private:
	std::stack<const Node*> stack{};
};

template<typename Node, typename traits, typename DefaultCompare>
struct AVLTreeT<Node, traits, DefaultCompare>::const_reverse_iterator
{
	using Tree = AVLTreeT<Node, traits, DefaultCompare>;
	using It = const_reverse_iterator;
	const_reverse_iterator() = default;
	const_reverse_iterator(const It&) = default;
	const_reverse_iterator& operator=(const const_reverse_iterator&) = default;
	const_reverse_iterator(It&&) = default;
	const_reverse_iterator& operator=(const_reverse_iterator&&) = default;
	~const_reverse_iterator() = default;
	bool operator==(const It& o)const
	{
		return o.stack == stack;
	}
	bool operator!=(const It& o)const { return !operator==(o); }
	const Node* operator *()
	{
		return stack.top();
	}
	const Node* operator->()
	{
		return stack.top();
	}
	It& operator++()
	{
		if (!stack.empty())
		{
			auto* n = stack.top();
			stack.pop();
			pushRightChildren(traits::Left(n));
		}
		return *this;
	}
	It operator++(int)
	{
		It ret{ *this };
		operator++();
		return ret;
	}
	const_reverse_iterator(const AVLTreeT<Node, traits, DefaultCompare>& tree)
	{
		pushRightChildren(tree.rootNode());
	}
private:
	void pushRightChildren(const Node* pNode)
	{
		for (; pNode; pNode = traits::Right(pNode))
		{
			stack.push(pNode);
		}
	}
private:
	std::stack<const Node*> stack{};
};

template<typename Node, typename traits, typename DefaultCompare>
typename AVLTreeT<Node, traits, DefaultCompare>::const_iterator AVLTreeT<Node, traits, DefaultCompare>::begin()const
{
	return AVLTreeT<Node, traits, DefaultCompare>::const_iterator{ *this };
}

template<typename Node, typename traits, typename DefaultCompare>
typename AVLTreeT<Node, traits, DefaultCompare>::const_iterator AVLTreeT<Node, traits, DefaultCompare>::end()const
{
	return AVLTreeT<Node, traits, DefaultCompare>::const_iterator{};
}

template<typename Node, typename traits, typename DefaultCompare>
typename AVLTreeT<Node, traits, DefaultCompare>::const_reverse_iterator AVLTreeT<Node, traits, DefaultCompare>::rbegin()const
{
	return AVLTreeT<Node, traits, DefaultCompare>::const_reverse_iterator{ *this };
}

template<typename Node, typename traits, typename DefaultCompare>
typename AVLTreeT<Node, traits, DefaultCompare>::const_reverse_iterator AVLTreeT<Node, traits, DefaultCompare>::rend()const
{
	return AVLTreeT<Node, traits, DefaultCompare>::const_reverse_iterator{};
}

template<typename Node, typename traits, typename DefaultCompare>
template<typename Compare>
inline
Node *AVLTreeT<Node,traits,DefaultCompare>::findNode(Node *r, const Node *pData, const Compare &cmp)
{
	if(r)
	{
		if( cmp(*pData, *r) )
		{
			r = findNode(Left(r), pData, cmp);
		}
		else if( cmp(*r, *pData) )
		{
			r = findNode(Right(r), pData, cmp);
		}
	}
	return r;
}

template<typename Node, typename traits, typename DefaultCompare>
template<typename Compare>
inline
Node *AVLTreeT<Node,traits,DefaultCompare>::lowerBound(Node *r, const Node *pNode, const Compare &cmp)
{
	if(r)
	{
		if( cmp(*pNode, *r) )
		{
			auto tmp = lowerBound(Left(r),pNode, cmp);
			if(tmp &&
				( cmp(*pNode, *tmp) || !cmp(*tmp, *pNode) )
			) {
				r = tmp;
			}
		}
		else if( cmp(*r, *pNode) )
		{
			r = lowerBound(Right(r), pNode, cmp);
		}
	}
	return r;
}

template<typename Node, typename traits, typename DefaultCompare>
template<typename Compare>
inline
bool AVLTreeT<Node,traits,DefaultCompare>::insertNode(Node *pNewNode, const Compare &cmp)
{
	auto success = (pNewNode != nullptr);
	if(success)
	{
		if(root)
		{
			success = insertNode(root, nullptr, pNewNode, cmp);
		}
		else
		{
			root = pNewNode;
			ANIMATE(std::string{ "Insert Node: " } + toStr<const Node *,traits>(pNewNode));
		}
	}
	return success;
}

// template<typename Node, typename traits, typename DefaultCompare>
// template<typename Compare>
// inline
// bool AVLTreeT<Node,traits,DefaultCompare>::deleteNode(const Node *pNode, const Compare &cmp)
// {
// 	int32_t bf = 0;
// 	auto *node = removeNode(rootNode(), pNode, bf, cmp);
// 	auto success = (node != nullptr);
// 	if(success)
// 	{
// 		delete node;
// 	}
// 	return success;
// }

template<typename Node, typename traits, typename DefaultCompare>
template<typename Compare>
inline
Node *AVLTreeT<Node,traits,DefaultCompare>::removeNode(const Node *pNode, const Compare &cmp)
{
	int32_t bf = 0;
	return removeNode(root, pNode, bf, cmp);
}


template<typename Node, typename traits, typename DefaultCompare>
template<typename Compare>
inline
Node *AVLTreeT<Node,traits,DefaultCompare>::find(const Node *pData, const Compare &cmp)
{
	return pData ? findNode(root, pData, cmp) : false;
}

template<typename Node, typename traits, typename DefaultCompare>
template<typename Compare>
inline
Node *AVLTreeT<Node,traits,DefaultCompare>::lowerBound(const Node *pNode, const Compare &cmp)
{
	return lowerBound(root, pNode, cmp);
}

template<typename Node, typename traits, typename DefaultCompare>
inline
void AVLTreeT<Node,traits,DefaultCompare>::printTree()const
{
	::printTree<const Node *, traits>(rootNode(), std::shared_ptr<Trunk>{}, false);
}

template<typename NodePtr, typename traits, typename DefaultCompare>
inline void dottyTree(const AVLTreeT<NodePtr, traits, DefaultCompare>& tree)
{
	std::cout << "digraph {\n";

	for (const auto* n : tree)
	{
		auto s = toStr<const NodePtr *, traits>(n);
		auto bf = BF<const NodePtr*, traits>(n);
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
	}
	for (const auto* n : tree)
	{
		auto s = toStr<const NodePtr *, traits>(n);
		if (traits::Left(n))
		{
			std::cout << '\t' << s << " -> {";
			std::cout << toStr<const NodePtr *, traits>(traits::Left(n)) << " [color=red] ";
			std::cout << "} [color=red arrowhead=vee]\n";
		}

		if (traits::Right(n))
		{
			std::cout << '\t' << s << " -> {";
			std::cout << toStr<const NodePtr*, traits>(traits::Right(n)) << " [color=blue]";
			std::cout << "} [color=blue arrowhead=vee]\n";
		}
	}


	// dottyTree<const NodePtr*, traits>(tree.rootNode());
	std::cout << "}" << std::endl;
}

template<typename Node, typename traits, typename DefaultCompare>
inline
void AVLTreeT<Node,traits,DefaultCompare>::dottyTree()const
{
	::dottyTree(*this);
}
/*         left rotate
 *
 *    1              2
 *     \            / \
 *      2    ==>   1   3
 *       \ 
 *        3
 */
template<typename Node, typename traits, typename DefaultCompare>
inline
Node *AVLTreeT<Node,traits,DefaultCompare>::left_rotate(Node *p)
{
	
	auto *q = Right(p);

	REBALANCEMSG(std::string{ "Left Rotate p(" } + toStr<const Node*, traits>(p) + ") q(" + toStr<const Node*, traits>(q) + ") r(" + toStr<const Node*, traits>(Right(q)) + ")");

	Right(p) = Left(q);
	Left(q) = p;

	
	traits::bf(p, traits::bf(p) - (1 + std::max(0, traits::bf(q))) ); // bf(p) -= 1 + max(0, bf(q));
	traits::bf(q, traits::bf(q) - (1 - std::min(0, traits::bf(p))) ); // bf(q) -= 1 - min(0, bf(p));
	
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
template<typename Node, typename traits, typename DefaultCompare>
inline
Node *AVLTreeT<Node,traits,DefaultCompare>::right_rotate(Node *p)
{
	auto *q = Left(p);

	REBALANCEMSG(std::string{ "Right Rotate p(" } + toStr<const Node*, traits>(p) + ") q(" + toStr<const Node*, traits>(q) + ") r(" + toStr<const Node*, traits>(Left(q)) + ")");

	Left(p) = Right(q);
	Right(q) = p;

	traits::bf(p, traits::bf(p) + (1 - std::min(0, traits::bf(q)))); // bf(p) += 1 - min(0, bf(q));
	traits::bf(q, traits::bf(q) + (1 + std::max(0, traits::bf(p)))); // bf(q) += 1 + max(0, bf(p));

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
template<typename Node, typename traits, typename DefaultCompare>
inline
Node *AVLTreeT<Node,traits,DefaultCompare>::left_right_rotate(Node *p)
{
	auto *q = Left(p);
	auto *r = Right(q);

	REBALANCEMSG(std::string{ "Left Right Rotate p(" } + toStr<const Node*, traits>(p) + ") q(" + toStr<const Node*, traits>(q) + ") r(" + toStr<const Node*, traits>(r) + ")");

	Right(q) = Left(r);
	Left(p) = Right(r);
	Right(r) = p;
	Left(r) = q;

	
	traits::bf(q, traits::bf(q) - (1 + std::max(0, traits::bf(r)))); // bf(q) -= 1 + max(0, bf(r));
	traits::bf(p, traits::bf(p) + (1 - std::min(std::min(0, traits::bf(r)) - 1, traits::bf(r) + traits::bf(q)))); // bf(p) += 1 - min(min(0, bf(r)) - 1, bf(r) + bf(q));
	traits::bf(r, traits::bf(r) + (std::max(0, traits::bf(p)) + std::min(0, traits::bf(q)))); // bf(r) += max(0, bf(p)) + min(0, bf(q));
	
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
template<typename Node, typename traits, typename DefaultCompare>
inline
Node *AVLTreeT<Node,traits,DefaultCompare>::right_left_rotate(Node *p)
{
	auto *q = Right(p);
	auto *r = Left(q);

	REBALANCEMSG(std::string{ "Left Right Rotate p(" } + toStr<const Node*, traits>(p) + ") q(" + toStr<const Node*, traits>(q) + ") r(" + toStr<const Node*, traits>(r) + ")");

	Left(q) = Right(r);
	Right(p) = Left(r);
	Left(r) = p;
	Right(r) = q;

	
	traits::bf(q, traits::bf(q) + (1 - std::min(0, traits::bf(r)))); // bf(q) += 1 - min(0, bf(r));
	traits::bf(p, traits::bf(p) - (1 + std::max(std::max(0, traits::bf(r)) + 1, traits::bf(r) + traits::bf(q)))); // bf(p) -= 1 + max(max(0, bf(r)) + 1, bf(r) + bf(q));
	traits::bf(r, traits::bf(r) + (std::max(0, traits::bf(q)) + std::min(0, traits::bf(p)))); // bf(r) += max(0, bf(q)) + min(0, bf(p));

	return r;
}

template<typename Node, typename traits, typename DefaultCompare>
inline
void AVLTreeT<Node,traits,DefaultCompare>::rebalance(Node *&r)
{
	REBALANCEMSG("");
	if(r)
	{
		if(traits::bf(r) == -2 )
		{
			r = (traits::bf(Left(r)) == 1)
				? left_right_rotate(r)
				: right_rotate(r);
		}
		else if( traits::bf(r) == 2 )
		{
			r = (traits::bf(Right(r)) == -1)
				? right_left_rotate(r)
				: left_rotate(r);
		}
	}
}

template<typename Node, typename traits, typename DefaultCompare>
template<typename Compare>
inline
bool
AVLTreeT<Node,traits,DefaultCompare>::insertNode(node *&r,node *p, node *pData, const Compare &cmp)
{
	auto success = false;
	auto before = 0u;
	if( r )
	{
		before = traits::bf(r);
		if( cmp(*pData, *r) )
		{
			if( Left(r) )
			{
				success = insertNode(Left(r), r, pData, cmp);
			}
			else
			{
				Left(r) =  pData;
				traits::bf(r, traits::bf(r) - 1 );
				ANIMATE(std::string{ "Insert Node: " } + toStr<const Node*, traits>(pData));
				success = true;
			}
		}
		else if( cmp(*r, *pData) )
		{
			if( Right(r) )
			{
				success = insertNode(Right(r), r, pData, cmp);
			}
			else
			{
				Right(r) = pData;
				traits::bf(r, traits::bf(r) + 1 );
				ANIMATE(std::string{ "Insert Node: " } + toStr<const Node*, traits>(pData));
				success = true;
			}
		}

		if(success)
		{
			if (p && traits::bf(r) && before == 0)
			{
				traits::bf(p, traits::bf(p) + (Left(p) == r ? (-1) : (+1))); // bf(p) += (Left(p) == r ? (-1) : (+1));
			}
			
			rebalance(r);
			RANIMATE();
		}
	}
	return success;
}

template<typename Node, typename traits, typename DefaultCompare>
inline
Node *
AVLTreeT<Node,traits,DefaultCompare>::stepDeleteLeft(node *&target, node *surrogate, node *prev, int32_t &bf)
{
	node *rm = nullptr;
	
	if( ! Left(surrogate) )
	{
		// remove surrogate from it's parent
		if( Left(prev) == surrogate )
		{
			Left(prev) = Right(surrogate);
		}
		else
		{
			assert(Right(prev) == surrogate);
			Right(prev) = Right(surrogate);
		}
		// and exchange surrogate for target
		// copy target's bf to surrogate, as surrogate is
		// taking target's place in the tree.
		traits::bf(surrogate, traits::bf(target));
		// attach target's children to surrogate
		Left(surrogate) = Left(target);
		Right(surrogate) = Right(target);
		Left(target) = Right(target) = nullptr;
		rm = target;
		target = surrogate;
		// traits::swap(target, rm);
		// std::swap(traits::data(target), traits::data(rm));
		// surrogate = Right(surrogate);
		bf = 1;
		ANIMATE(std::string{ "Step Delete Left: " } + toStr<const Node*, traits>(rm) );
	}
	else
	{
		rm = stepDeleteLeft(target, Left(surrogate), surrogate, bf );
	}
	
	if( bf )
	{
		if( prev == target )
		{
			traits::bf(prev, traits::bf(prev) + 1); // bf(prev)--;
		}
		else
		{
			traits::bf(prev, traits::bf(prev) - 1); // bf(prev)++;
		}

		rebalance(prev);
		RANIMATE();

		if( traits::bf(prev) == 1 || traits::bf(prev) == -1 )
		{
			bf = 0;
		}
	}
	return rm;
}

template<typename Node, typename traits, typename DefaultCompare>
inline
Node *
AVLTreeT<Node,traits,DefaultCompare>::stepDeleteRight(node *&target, node *surrogate, node *prev, int32_t &bf)
{
	node *rm = nullptr;
	
	if( !Right(surrogate) )
	{
		if( Left(prev) == surrogate )
		{
			Left(prev) = Left(surrogate);
		}
		else
		{
			assert(Right(prev) == surrogate);
			Right(prev) = Left(surrogate);
		}
		// and exchange surrogate for target
		// copy target's bf to surrogate, as surrogate is
		// taking target's place in the tree.
		traits::bf(surrogate, traits::bf(target));
		// attach target's children to surrogate
		Left(surrogate) = Left(target);
		Right(surrogate) = Right(target);
		Left(target) = Right(target) = nullptr;
		rm = target;
		target = surrogate;
//		traits::swap(target, rm);
//		// std::swap(traits::data(target), traits::data(rm));
//		surrogate = Left(surrogate);
		bf = 1;
		ANIMATE(std::string{ "Step Delete Right: " } + toStr<const Node*, traits>(rm));
	}
	else
	{
		rm = stepDeleteRight(target, Right(surrogate), surrogate, bf );
	}
	
	if( bf )
	{
		if( prev == target )
		{
			traits::bf(prev, traits::bf(prev) + 1); // bf(prev)++;
		}
		else
		{
			traits::bf(prev, traits::bf(prev) - 1); // bf(prev)--;
		}

		rebalance(prev);
		RANIMATE();

		if( traits::bf(prev) == 1 || traits::bf(prev) == -1 )
		{
			bf = 0;
		}
	}
	return rm;
}

// template<typename Node, typename traits, typename DefaultCompare>
// template<typename Compare>
// inline
// bool
// AVLTreeT<Node,traits,DefaultCompare>::deleteNode(node *&r,const Node *pData, int32_t &bf, const Compare &cmp)
// {
// 	auto success = false;
// 	auto direction = 0;
// 	if(r)
// 	{
// 		if( cmp(*pData, *r) )
// 		{
// 			direction = 1;
// 			success = deleteNode(Left(r), pData, bf, cmp);
// 		}
// 		else if( cmp(*r, *pData) )
// 		{
// 			direction = -1;
// 			success = deleteNode(Right(r), pData, bf, cmp);
// 		}
// 		else
// 		{
// 			node *old = nullptr;
// 			success = true;
			
// 			if( ! Left(r) )
// 			{
// 				old = r;
// 				r = Right(r);
// 				bf = 1;
// 				ANIMATE("deleteNode");
// 			}
// 			else if ( ! Right(r) )
// 			{
// 				old =r;
// 				r = Left(r);
// 				bf = 1;
// 				ANIMATE("deleteNode");
// 			}
// 			else
// 			{
// 				old = ( bf(r) > 0 )
// 					? stepDeleteLeft(r, Right(r), r, bf, cmp)
// 					: stepDeleteRight(r, Left(r), r, bf, cmp);
				
// 				if( bf(r) == 1 || bf(r) == -1 )
// 				{
// 					bf = 0;
// 				}
// 			}
//             traits::deleteNode(old);
// 		}
		
// 		if( direction )
// 		{
// 			if(bf)
// 			{
// 				bf(r, bf(r) + direction); // bf(r) += direction;
// 			}
			
// 			rebalance(r);
// 			RANIMATE();
			
// 			if( bf(r) )
// 			{
// 				bf = 0;
// 			}
// 		}
// 	}
// 	return success;
// }

template<typename Node, typename traits, typename DefaultCompare>
template<typename Compare>
inline Node *
AVLTreeT<Node,traits,DefaultCompare>::removeNode(node *&r,node *parent, const node *pData, int32_t &bf, const Compare &cmp)
{
	auto direction = 0;
	node *old = nullptr;
	if(r)
	{
		if( cmp(*pData, *r) )
		{
			direction = 1;
			old = removeNode(Left(r), pData, bf, cmp);
		}
		else if( cmp(*r, *pData) )
		{
			direction = -1;
			old = removeNode(Right(r), pData, bf, cmp);
		}
		else if( ! Left(r) )
		{
			old = r;
			r = Right(r);
			bf = 1;
			ANIMATE(std::string{ "Remove Node: " } + toStr(old));
		}
		else if ( ! Right(r) )
		{
			old = r;
			r = Left(r);
			bf = 1;
			ANIMATE(std::string{ "Remove Node: " } + toStr(old));
		}
		else
		{
			old =  ( traits::bf(r) > 0 )
					? stepDeleteLeft(r, Right(r), r, bf)
					: stepDeleteRight(r, Left(r), r, bf);

			Left(old) = Left(r);
			Right(old) = Right(r);
			if(Left(parent) == r )
			{
				Left(parent) = old;
			}
			else
			{
				assert(Right(parent) == old);
				Right(parent) = old;
			}
			std::swap(old,r);

			if( traits::bf(r) == 1 || traits::bf(r) == -1 )
			{
				bf = 0;
			}
		}
		
		if( direction )
		{
			if(bf)
			{
				traits::bf(r, traits::bf(r) + direction); // bf(r) += direction;
			}
			
			rebalance(r);
			RANIMATE();
			
			if( traits::bf(r) )
			{
				bf = 0;
			}
		}
	}
	return old;
}

template<typename Node, typename traits, typename DefaultCompare>
template<typename Compare>
inline
Node *
AVLTreeT<Node,traits,DefaultCompare>::removeNode(Node *&r,const Node *pData, int32_t &bf, const Compare &cmp)
{
	auto direction = 0;
	node *old = nullptr;
	if(r)
	{
		if( cmp(*pData, *r) )
		{
			direction = 1;
			old = removeNode(Left(r), pData, bf, cmp);
		}
		else if( cmp(*r, *pData) )
		{
			direction = -1;
			old = removeNode(Right(r), pData, bf, cmp);
		}
		else if( ! Left(r) )
		{
			old = r;
			r = Right(r);
			bf = 1;
			ANIMATE(std::string{ "Remove Node: " } + toStr<const Node*, traits>(old));
		}
		else if ( ! Right(r) )
		{
			old =r;
			r = Left(r);
			bf = 1;
			ANIMATE(std::string{ "Remove Node: " } + toStr<const Node*, traits>(old));
		}
		else
		{
			old =  ( traits::bf(r) > 0 )
					? stepDeleteLeft(r, Right(r), r, bf)
					: stepDeleteRight(r, Left(r), r, bf);

			if( traits::bf(r) == 1 || traits::bf(r) == -1 )
			{
				bf = 0;
			}
		}
		
		if( direction )
		{
			if(bf)
			{
				traits::bf(r, traits::bf(r) + direction); // bf(r) += direction;
			}
			
			rebalance(r);
			RANIMATE();
			
			if( traits::bf(r) )
			{
				bf = 0;
			}
		}
	}
	return old;
}

#endif // AVLTREET_H_INCLUDED

