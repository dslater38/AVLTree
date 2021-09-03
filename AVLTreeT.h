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

template<typename NodePtr, typename traits=typename NodePtr::traits, typename Compare=std::less<NodePtr>>
class AVLTreeT
{
public:
	using value_type = NodePtr;
	using NodePtrArg = typename std::conditional<std::is_pointer<NodePtr>::value, NodePtr, NodePtr &>::type;

public:
	AVLTreeT()=default;
	AVLTreeT(const AVLTreeT&)=delete;
	AVLTreeT &operator=(const AVLTreeT&)=delete;
	AVLTreeT(AVLTreeT &&)=default;
	AVLTreeT &operator=(AVLTreeT &&)=default;
	~AVLTreeT() = default;

	struct const_iterator;
	struct const_reverse_iterator;

	NodePtr removeNode(const NodePtrArg pNode);

	bool insertNode(NodePtr pNode);

	NodePtr &find(const NodePtr &pNode);

	NodePtr &lowerBound(const NodePtr &pData);

	NodePtr &rootNode() { return root; }
	const NodePtr &rootNode()const { return root; }

	void printTree()const;
	void dottyTree()const;

	const_iterator begin()const;
	const_iterator end()const;

	const_reverse_iterator rbegin()const;
	const_reverse_iterator rend()const;

private:
	static NodePtr &Left(NodePtr &n) { return traits::Left(n); }
	static NodePtr &Right(NodePtr &n) { return traits::Right(n); }
	static const NodePtr &Left(const NodePtr &n) { return traits::Left(n); }
	static const NodePtr &Right(const NodePtr &n) { return traits::Right(n); }
private:
	NodePtr &findNode(NodePtr &r, const NodePtr &pData);

	NodePtr &lowerBound(NodePtr &r, const NodePtr &pData);

	bool insertNode(NodePtr &r, NodePtr &p, NodePtr &pData);

	// template<typename Compare>
	// bool  deleteNode(node *&r,const node *pData, int32_t &bf, const Compare &cmp);

	NodePtr removeNode(NodePtr &r,const NodePtrArg pData, int32_t &bf);

	NodePtr removeNode(NodePtr &r, NodePtrArg parent, const NodePtrArg pData, int32_t &bf);


	NodePtr left_rotate(NodePtr &&node);
	NodePtr right_rotate(NodePtr &&node);
	NodePtr right_left_rotate(NodePtr &&node);
	NodePtr left_right_rotate(NodePtr &&node);
	NodePtr rebalance(NodePtr &&node);
	NodePtr stepDeleteLeft(NodePtr &target, NodePtrArg surrogate, NodePtrArg prev, int32_t &bf);
	NodePtr stepDeleteRight(NodePtr &target, NodePtrArg surrogate, NodePtrArg prev, int32_t &bf);
private:
	NodePtr root{};
	const Compare cmp{};
#if defined(DEBUG) || defined(_DEBUG)
	inline static std::string rebalance_msg="";
	inline void REBALANCEMSG(std::string s) {rebalance_msg=s;}
	inline void ANIMATE(std::string msg) { animateTree(msg ); }
	inline void RANIMATE() { if( !rebalance_msg.empty() ) { animateTree(rebalance_msg); } }
	inline void animateTree(std::string msg)
	{

		//std::cout << "======================== " << msg << " ========================\n";
		//printTree();

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

template<typename NodePtr, typename traits, typename Compare>
struct AVLTreeT<NodePtr, traits, Compare>::const_iterator
{
	using Tree = AVLTreeT<NodePtr, traits, Compare>;
	using It = const_iterator;
	const_iterator() = default;
	const_iterator(const It&) = default;
	const_iterator& operator=(const const_iterator&) = default;
	const_iterator(It&&) = default;
	const_iterator& operator=(const_iterator&&) = default;
	~const_iterator() = default;
	bool operator==(const It& o)const
	{
		if (stack.empty() == o.stack.empty())
		{
			if (stack.empty())
			{
				return true;
			}
			else
			{
				const auto &a = stack.top();
				const auto &b = o.stack.top();
				return (cmp(a, b) == false && cmp(b, a) == false);
			}
		}
		return false;
	}
	bool operator!=(const It& o)const { return !operator==(o); }
	const NodePtr &operator *()
	{
		return stack.top();
	}
	const NodePtr &operator->()
	{
		return stack.top();
	}
	It& operator++()
	{
		if (!stack.empty())
		{
			const auto &n = stack.top();
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
	const_iterator(const AVLTreeT<NodePtr, traits, Compare>& tree)
	{
		pushLeftChildren(tree.rootNode());
	}
private:
	void pushLeftChildren(const NodePtr &pNode)
	{
		for (auto n = std::cref(pNode);
			static_cast<bool>(n.get());
			n = std::cref(traits::Left(n.get())))
		{
			stack.push(n);
		}
	}
private:
	std::stack<std::reference_wrapper<const NodePtr>> stack{};
	const Compare	cmp{};
};

template<typename NodePtr, typename traits, typename Compare>
struct AVLTreeT<NodePtr, traits, Compare>::const_reverse_iterator
{
	using Tree = AVLTreeT<NodePtr, traits, Compare>;
	using It = const_reverse_iterator;
	const_reverse_iterator() = default;
	const_reverse_iterator(const It&) = default;
	const_reverse_iterator& operator=(const const_reverse_iterator&) = default;
	const_reverse_iterator(It&&) = default;
	const_reverse_iterator& operator=(const_reverse_iterator&&) = default;
	~const_reverse_iterator() = default;
	bool operator==(const It &o)const
	{
		if (stack.empty() == o.stack.empty())
		{
			if (stack.empty())
			{
				return true;
			}
			else
			{
				const auto &a = stack.top();
				const auto &b = o.stack.top();
				return (cmp(a, b) == false && cmp(b, a) == false);
			}
		}
		return false;
	}
	bool operator!=(const It& o)const { return !operator==(o); }
	const NodePtr &operator *()
	{
		return stack.top();
	}
	const NodePtr &operator->()
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
	const_reverse_iterator(const AVLTreeT<NodePtr, traits, Compare>& tree)
	{
		pushRightChildren(tree.rootNode());
	}
private:
	void pushRightChildren(const NodePtr &pNode)
	{
		for (auto n = std::cref(pNode);
			static_cast<bool>(n.get());
			n = std::cref(traits::Right(n.get())))
		{
			stack.push(n);
		}
	}
private:
	std::stack<std::reference_wrapper<const NodePtr>> stack{};
	const Compare	cmp{};
};

template<typename NodePtr, typename traits, typename Compare>
typename AVLTreeT<NodePtr, traits, Compare>::const_iterator AVLTreeT<NodePtr, traits, Compare>::begin()const
{
	return AVLTreeT<NodePtr, traits, Compare>::const_iterator{ *this };
}

template<typename NodePtr, typename traits, typename Compare>
typename AVLTreeT<NodePtr, traits, Compare>::const_iterator AVLTreeT<NodePtr, traits, Compare>::end()const
{
	return AVLTreeT<NodePtr, traits, Compare>::const_iterator{};
}

template<typename NodePtr, typename traits, typename Compare>
typename AVLTreeT<NodePtr, traits, Compare>::const_reverse_iterator AVLTreeT<NodePtr, traits, Compare>::rbegin()const
{
	return AVLTreeT<NodePtr, traits, Compare>::const_reverse_iterator{ *this };
}

template<typename NodePtr, typename traits, typename Compare>
typename AVLTreeT<NodePtr, traits, Compare>::const_reverse_iterator AVLTreeT<NodePtr, traits, Compare>::rend()const
{
	return AVLTreeT<NodePtr, traits, Compare>::const_reverse_iterator{};
}

template<typename NodePtr, typename traits, typename Compare>
inline
NodePtr &AVLTreeT<NodePtr,traits,Compare>::findNode(NodePtr &r, const NodePtr &pData)
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

template<typename NodePtr, typename traits, typename Compare>
inline
NodePtr &AVLTreeT<NodePtr,traits,Compare>::lowerBound(NodePtr &r, const NodePtr &pNode)
{
	if(r)
	{
		if( cmp(pNode, r) )
		{
			auto &tmp = lowerBound(Left(r),pNode);
			if(tmp &&
				( cmp(pNode, tmp) || !cmp(tmp, pNode) )
			) {
				return tmp;
			}
		}
		else if( cmp(r, pNode) )
		{
			return lowerBound(Right(r), pNode);
		}
	}
	return r;
}

template<typename NodePtr, typename traits, typename Compare>
inline
bool AVLTreeT<NodePtr,traits,Compare>::insertNode(NodePtr pNewNode)
{
	auto success = (pNewNode != nullptr);
	if(success)
	{
		if(root)
		{
			NodePtr p{ nullptr };
			success = insertNode(root, p, pNewNode);
		}
		else
		{
			root = std::move(pNewNode);
			ANIMATE(std::string{ "Insert Node: " } + toStr<const NodePtr,traits>(pNewNode));
		}
	}
	return success;
}

// template<typename Node, typename traits, typename Compare>
// template<typename Compare>
// inline
// bool AVLTreeT<Node,traits,Compare>::deleteNode(const Node *pNode, const Compare &cmp)
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

template<typename NodePtr, typename traits, typename Compare>
inline
NodePtr AVLTreeT<NodePtr,traits,Compare>::removeNode(const NodePtrArg pNode)
{
	int32_t bf = 0;
	return removeNode(root, pNode, bf);
}


template<typename NodePtr, typename traits, typename Compare>
inline
NodePtr &AVLTreeT<NodePtr,traits,Compare>::find(const NodePtr &pData)
{
	return pData ? findNode(root, pData) : false;
}

template<typename NodePtr, typename traits, typename Compare>
inline
NodePtr &AVLTreeT<NodePtr,traits,Compare>::lowerBound(const NodePtr &pNode)
{
	return lowerBound(root, pNode);
}

template<typename NodePtr, typename traits, typename Compare>
inline
void AVLTreeT<NodePtr,traits,Compare>::printTree()const
{
	::printTree<const NodePtr &, traits>(rootNode(), std::shared_ptr<Trunk>{}, false);
}

template<typename NodePtr, typename traits, typename Compare>
inline void dottyTree(const AVLTreeT<NodePtr, traits, Compare>& tree)
{
	std::cout << "digraph {\n";

	for (const NodePtr &n : tree)
	{
		auto s = toStr<const NodePtr, traits>(n);
		auto bf = BF<const NodePtr, traits>(n);
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
	for (const NodePtr &n : tree)
	{
		auto s = toStr<const NodePtr, traits>(n);
		if (traits::Left(n))
		{
			std::cout << '\t' << s << " -> {";
			std::cout << toStr<const NodePtr, traits>(traits::Left(n)) << " [color=red] ";
			std::cout << "} [color=red arrowhead=vee]\n";
		}

		if (traits::Right(n))
		{
			std::cout << '\t' << s << " -> {";
			std::cout << toStr<const NodePtr, traits>(traits::Right(n)) << " [color=blue]";
			std::cout << "} [color=blue arrowhead=vee]\n";
		}
	}


	// dottyTree<const NodePtr*, traits>(tree.rootNode());
	std::cout << "}" << std::endl;
}

template<typename NodePtr, typename traits, typename Compare>
inline
void AVLTreeT<NodePtr,traits,Compare>::dottyTree()const
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
template<typename NodePtr, typename traits, typename Compare>
inline
NodePtr AVLTreeT<NodePtr,traits,Compare>::left_rotate(NodePtr &&p)
{
	
	auto q = std::forward<NodePtr>(Right(p));

	REBALANCEMSG(std::string{ "Left Rotate p(" } + toStr<const NodePtr, traits>(p) + ") q(" + toStr<const NodePtr, traits>(q) + ") r(" + toStr<const NodePtr, traits>(Right(q)) + ")");

	Right(p) = std::forward<NodePtr>(Left(q));

	traits::bf(p, traits::bf(p) - (1 + std::max(0, traits::bf(q)))); // bf(p) -= 1 + max(0, bf(q));
	traits::bf(q, traits::bf(q) - (1 - std::min(0, traits::bf(p)))); // bf(q) -= 1 - min(0, bf(p));

	Left(q) = std::forward<NodePtr>(p);

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
template<typename NodePtr, typename traits, typename Compare>
inline
NodePtr AVLTreeT<NodePtr,traits,Compare>::right_rotate(NodePtr &&p)
{
	auto q = std::forward<NodePtr>(Left(p));

	REBALANCEMSG(std::string{ "Right Rotate p(" } + toStr<const NodePtr, traits>(p) + ") q(" + toStr<const NodePtr, traits>(q) + ") r(" + toStr<const NodePtr, traits>(Left(q)) + ")");

	Left(p) = std::forward<NodePtr>(Right(q));

	traits::bf(p, traits::bf(p) + (1 - std::min(0, traits::bf(q)))); // bf(p) += 1 - min(0, bf(q));
	traits::bf(q, traits::bf(q) + (1 + std::max(0, traits::bf(p)))); // bf(q) += 1 + max(0, bf(p));


	Right(q) = std::forward<NodePtr>(p);


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
template<typename NodePtr, typename traits, typename Compare>
inline
NodePtr AVLTreeT<NodePtr,traits,Compare>::left_right_rotate(NodePtr &&p)
{
#ifdef VERBOSE_ROTATIONS
	Left(p) = left_rotate(std::forward<NodePtr>(Left(p)));
	Left(p) = rebalance(std::forward<NodePtr>(Left(p)));
	return right_rotate(std::forward<NodePtr>(p));
#else
	auto q = std::forward<NodePtr>(Left(p));
	auto r = std::forward<NodePtr>(Right(q));

	REBALANCEMSG(std::string{ "Left Right Rotate p(" } + toStr<const NodePtr, traits>(p) + ") q(" + toStr<const NodePtr, traits>(q) + ") r(" + toStr<const NodePtr, traits>(r) + ")");

	Right(q) = std::forward<NodePtr>(Left(r));
	Left(p) = std::forward<NodePtr>(Right(r));

	traits::bf(q, traits::bf(q) - (1 + std::max(0, traits::bf(r)))); // bf(q) -= 1 + max(0, bf(r));
	traits::bf(p, traits::bf(p) + (1 - std::min(std::min(0, traits::bf(r)) - 1, traits::bf(r) + traits::bf(q)))); // bf(p) += 1 - min(min(0, bf(r)) - 1, bf(r) + bf(q));
	traits::bf(r, traits::bf(r) + (std::max(0, traits::bf(p)) + std::min(0, traits::bf(q)))); // bf(r) += max(0, bf(p)) + min(0, bf(q));

	Right(r) = std::forward<NodePtr>(p);
	Left(r) = std::forward<NodePtr>(q);
	

	return r;
#endif
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
template<typename NodePtr, typename traits, typename Compare>
inline
NodePtr AVLTreeT<NodePtr,traits,Compare>::right_left_rotate(NodePtr &&p)
{
#ifdef VERBOSE_ROTATIONS
	Right(p) = right_rotate(std::forward<NodePtr>(Right(p)));
	Right(p) = rebalance(std::forward<NodePtr>(Right(p)));
	return left_rotate(std::forward<NodePtr>(p));
#else

	auto q = std::forward<NodePtr>(Right(p));
	auto r = std::forward<NodePtr>(Left(q));

	REBALANCEMSG(std::string{ "Left Right Rotate p(" } + toStr<const NodePtr, traits>(p) + ") q(" + toStr<const NodePtr, traits>(q) + ") r(" + toStr<const NodePtr, traits>(r) + ")");

	Left(q) = std::forward<NodePtr>(Right(r));
	Right(p) = std::forward<NodePtr>(Left(r));

	traits::bf(q, traits::bf(q) + (1 - std::min(0, traits::bf(r)))); // bf(q) += 1 - min(0, bf(r));
	traits::bf(p, traits::bf(p) - (1 + std::max(std::max(0, traits::bf(r)) + 1, traits::bf(r) + traits::bf(q)))); // bf(p) -= 1 + max(max(0, bf(r)) + 1, bf(r) + bf(q));
	traits::bf(r, traits::bf(r) + (std::max(0, traits::bf(q)) + std::min(0, traits::bf(p)))); // bf(r) += max(0, bf(q)) + min(0, bf(p));

	Left(r) = std::forward<NodePtr>(p);
	Right(r) = std::forward<NodePtr>(q);


	return r;
#endif
}

template<typename NodePtr, typename traits, typename Compare>
inline
NodePtr AVLTreeT<NodePtr,traits,Compare>::rebalance(NodePtr &&r)
{
	REBALANCEMSG("");
	if(r)
	{
		if(traits::bf(r) == -2 )
		{
			if(traits::bf(Left(r)) == 1)
			{
				r = left_right_rotate(std::forward<NodePtr>(r));
			}
			else
			{
				r = right_rotate(std::forward<NodePtr>(r));
			}
		}
		else if( traits::bf(r) == 2 )
		{
			if(traits::bf(Right(r)) == -1)
			{
				r = right_left_rotate(std::forward<NodePtr>(r));
			}
			else
			{
				r = left_rotate(std::forward<NodePtr>(r));
			}
		}
	}
	return r;
}

template<typename NodePtr, typename traits, typename Compare>
inline
bool
AVLTreeT<NodePtr,traits,Compare>::insertNode(NodePtr &r, NodePtr &p, NodePtr &pData)
{
	auto success = false;
	auto before = 0u;
	if( r )
	{
		before = traits::bf(r);
		if( cmp(pData, r) )
		{
			if( Left(r) )
			{
				success = insertNode(Left(r), r, pData);
			}
			else
			{
				Left(r) =  std::forward<NodePtr>(pData);
				traits::bf(r, traits::bf(r) - 1 );
				ANIMATE(std::string{ "Insert Node: " } + toStr<const NodePtr, traits>(pData));
				success = true;
			}
		}
		else if( cmp(r, pData) )
		{
			if( Right(r) )
			{
				success = insertNode(Right(r), r, pData);
			}
			else
			{
				Right(r) = std::forward<NodePtr>(pData);
				traits::bf(r, traits::bf(r) + 1 );
				ANIMATE(std::string{ "Insert Node: " } + toStr<const NodePtr, traits>(pData));
				success = true;
			}
		}

		if(success)
		{
			if (p && traits::bf(r) && before == 0)
			{
				traits::bf(p, traits::bf(p) + (Left(p) == r ? (-1) : (+1))); // bf(p) += (Left(p) == r ? (-1) : (+1));
			}
			
			r = rebalance(std::forward<NodePtr>(r));
			RANIMATE();
		}
	}
	return success;
}

template<typename NodePtr, typename traits, typename Compare>
inline
NodePtr
AVLTreeT<NodePtr,traits,Compare>::stepDeleteLeft(NodePtr &target, NodePtrArg surrogate, NodePtrArg prev, int32_t &bf)
{
	NodePtr rm{nullptr};
	
	if( ! Left(surrogate) )
	{
		// remove surrogate from it's parent
		// and replace it with it's right child
		const bool isLeftChild = (Left(prev) == surrogate);
		rm = std::move(surrogate);
		if(isLeftChild)
		{
			Left(prev) = std::forward<NodePtr>(Right(rm));
		}
		else
		{
			// assert(Right(prev) == tmp);			
			Right(prev) = std::forward<NodePtr>(Right(rm));
		}
		Right(rm) = nullptr;
#if 0
		// and exchange surrogate for target
		// copy target's bf to surrogate, as surrogate is
		// taking target's place in the tree.
		traits::bf(tmp, traits::bf(target));
		// attach target's children to surrogate
		if (prev.get() == Left(target))
		{
			prev = std::ref(Left(tmp));
		}
		Left(tmp) = std::forward<NodePtr>(Left(target));
		if (prev.get() == Right(target))
		{
			prev = std::ref(Right(tmp));
		}
		Right(tmp) = std::forward<NodePtr>(Right(target));

		rm = std::forward<NodePtr>(target);
		Left(rm) = nullptr;
		Right(rm) = nullptr;
		target = std::forward<NodePtr>(tmp);
#endif // 0
#if 0
		surrogate = std::move(tmp);
		// and exchange surrogate for target
		// copy target's bf to surrogate, as surrogate is
		// taking target's place in the tree.
		traits::bf(surrogate, traits::bf(target));
		// attach target's children to surrogate
		Left(surrogate) = Left(target);
		Right(surrogate) = Right(target);
		Left(target) = Right(target) = nullptr;
		rm = std::move(target);
		target = std::move(surrogate);
#endif
		// traits::swap(target, rm);
		// std::swap(traits::data(target), traits::data(rm));
		// surrogate = Right(surrogate);
		bf = 1;
		ANIMATE(std::string{ "Step Delete Left: " } + toStr<const NodePtr, traits>(rm) );
	}
	else
	{
		rm = stepDeleteLeft(target, Left(surrogate), surrogate, bf );
	}
	
	if( bf )
	{
		if( prev == target )
		{
			traits::bf(prev, traits::bf(prev) - 1); // bf(prev)--;
		}
		else
		{
			traits::bf(prev, traits::bf(prev) + 1); // bf(prev)++;
		}

		prev = rebalance(std::forward<NodePtr>(prev));
		RANIMATE();

		if( traits::bf(prev) == 1 || traits::bf(prev) == -1 )
		{
			bf = 0;
		}
	}
	return rm;
}

template<typename NodePtr, typename traits, typename Compare>
inline
NodePtr
AVLTreeT<NodePtr,traits,Compare>::stepDeleteRight(NodePtr &target, NodePtrArg surrogate, NodePtrArg prev, int32_t &bf)
{
	NodePtr rm{};
	
	if( !Right(surrogate) )
	{
		const bool isLeftChild = (Left(prev) == surrogate);
		rm = std::move(surrogate);
		if(isLeftChild)
		{
			Left(prev) = std::forward<NodePtr>(Left(rm));
		}
		else
		{
			Right(prev) = std::forward<NodePtr>(Left(rm));
		}
		Left(rm) = nullptr;
#if 0
		// and exchange surrogate for target
		// copy target's bf to surrogate, as surrogate is
		// taking target's place in the tree.
		traits::bf(tmp, traits::bf(target));
		// attach target's children to surrogate
		if (prev.get() == Left(target))
		{
			prev = std::ref(Left(tmp));
		}
		Left(tmp) = std::forward<NodePtr>(Left(target));
		if (prev.get() == Right(target))
		{
			prev = std::ref(Right(tmp));
		}
		Right(tmp) = std::forward<NodePtr>(Right(target));

		rm = std::forward<NodePtr>(target);
		Left(rm) = nullptr;
		Right(rm) = nullptr;
		target = std::forward<NodePtr>(tmp);
#endif // 0
#if 0
		surrogate = std::move(tmp);
		// and exchange surrogate for target
		// copy target's bf to surrogate, as surrogate is
		// taking target's place in the tree.
		traits::bf(surrogate, traits::bf(target));
		// attach target's children to surrogate
		Left(surrogate) = Left(target);
		Right(surrogate) = Right(target);
		Left(target) = Right(target) = nullptr;
		rm = std::move(target);
		target = std::move(surrogate);
#endif
//		traits::swap(target, rm);
//		// std::swap(traits::data(target), traits::data(rm));
//		surrogate = Left(surrogate);
		bf = 1;
		ANIMATE(std::string{ "Step Delete Right: " } + toStr<const NodePtr, traits>(rm));
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

		prev = rebalance(std::forward<NodePtr>(prev));
		RANIMATE();

		if( traits::bf(prev) == 1 || traits::bf(prev) == -1 )
		{
			bf = 0;
		}
	}
	return rm;
}

// template<typename Node, typename traits, typename Compare>
// template<typename Compare>
// inline
// bool
// AVLTreeT<Node,traits,Compare>::deleteNode(node *&r,const Node *pData, int32_t &bf, const Compare &cmp)
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

template<typename NodePtr, typename traits, typename Compare>
inline NodePtr
AVLTreeT<NodePtr,traits,Compare>::removeNode(NodePtr &r, NodePtrArg parent, const NodePtrArg pData, int32_t &bf)
{
	auto direction = 0;
	NodePtr old{};
	if(r)
	{
		if( cmp(*pData, *r) )
		{
			direction = 1;
			old = removeNode(Left(r), r, pData, bf, cmp);
		}
		else if( cmp(*r, *pData) )
		{
			direction = -1;
			old = removeNode(Right(r), r, pData, bf, cmp);
		}
		else if( ! Left(r) )
		{
			old = std::move(r);
			r = std::move(Right(old));
			bf = 1;
			ANIMATE(std::string{ "Remove Node: " } + toStr(old));
		}
		else if ( ! Right(r) )
		{
			old = std::move(r);
			r = std::move(Left(old));
			bf = 1;
			ANIMATE(std::string{ "Remove Node: " } + toStr(old));
		}
		else
		{
			old =  ( traits::bf(r) > 0 )
					? stepDeleteLeft(r, Right(r), r, bf)
					: stepDeleteRight(r, Left(r), r, bf);



			Left(old) = std::move(Left(r));
			Right(old) = std::move(Right(r));
			bf(old) = bf(r);
			if(Left(parent) == r )
			{
				Left(parent) = std::move(old);
			}
			else
			{
				assert(Right(parent) == old);
				Right(parent) = std::move(old);
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
			
			r = rebalance(std::forward<NodePtr>(r));
			RANIMATE();
			
			if( traits::bf(r) )
			{
				bf = 0;
			}
		}
	}
	return old;
}

template<typename NodePtr, typename traits, typename Compare>
inline
NodePtr
AVLTreeT<NodePtr,traits,Compare>::removeNode(NodePtr &r,const NodePtrArg pData, int32_t &bf)
{
	auto direction = 0;
	NodePtr old{};
	if(r)
	{
		if( cmp(pData, r) )
		{
			direction = 1;
			old = removeNode(Left(r), pData, bf);
		}
		else if( cmp(r, pData) )
		{
			direction = -1;
			old = removeNode(Right(r), pData, bf);
		}
		else if( ! Left(r) )
		{
			old = std::move(r);
			r = std::move(Right(old));
			bf = 1;
			ANIMATE(std::string{ "Remove Node: " } + toStr<const NodePtr, traits>(old));
		}
		else if ( ! Right(r) )
		{
			old = std::move(r);
			r = std::move(Left(old));
			bf = 1;
			ANIMATE(std::string{ "Remove Node: " } + toStr<const NodePtr, traits>(old));
		}
		else
		{
			old =  ( traits::bf(r) > 0 )
					? stepDeleteLeft(r, std::forward<NodePtr>(Right(r)), r, bf)
					: stepDeleteRight(r, std::forward<NodePtr>(Left(r)), r, bf);

			Left(old) = std::move(Left(r));
			Right(old) = std::move(Right(r));
			const auto bfSave = traits::bf(r);
			std::swap(old, r);
			traits::bf(r, bfSave);
			//if (Left(parent) == r)
			//{
			//	Left(parent) = std::move(old);
			//}
			//else
			//{
			//	assert(Right(parent) == old);
			//	Right(parent) = std::move(old);
			//}
			


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
			
			r = rebalance(std::forward<NodePtr>(r));
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

