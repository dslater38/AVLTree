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

template<typename NodePtr, typename traits=typename NodePtr::traits, typename DefaultCompare=std::less<NodePtr>>
class AVLTreeT
{
public:
	using value_type = NodePtr;

public:
	AVLTreeT()=default;
	AVLTreeT(const AVLTreeT&)=delete;
	AVLTreeT &operator=(const AVLTreeT&)=delete;
	AVLTreeT(AVLTreeT &&)=default;
	AVLTreeT &operator=(AVLTreeT &&)=default;
	~AVLTreeT() = default;

	struct const_iterator;
	struct const_reverse_iterator;

	template<typename Compare=DefaultCompare>
	NodePtr removeNode(const NodePtr &pNode, const Compare &cmp=Compare{});

	template<typename Compare=DefaultCompare>
	bool insertNode(NodePtr pNode, const Compare &cmp=Compare{});

	template<typename Compare=DefaultCompare>
	NodePtr &find(const NodePtr &pNode, const Compare &cmp=Compare{});

	template<typename Compare=DefaultCompare>
	NodePtr &lowerBound(const NodePtr &pData, const Compare &cmp=Compare{});

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
	template<typename Compare>
	NodePtr &findNode(NodePtr &r, const NodePtr &pData, const Compare &cmp);

	template<typename Compare>
	NodePtr &lowerBound(NodePtr &r, const NodePtr &pData, const Compare &cmp);

	template<typename Compare>
	bool insertNode(NodePtr &r, NodePtr &p, NodePtr &pData, const Compare &cmp);

	// template<typename Compare>
	// bool  deleteNode(node *&r,const node *pData, int32_t &bf, const Compare &cmp);

	template<typename Compare>
	NodePtr removeNode(NodePtr &r,const NodePtr &pData, int32_t &bf, const Compare &cmp);

	template<typename Compare>
	NodePtr removeNode(NodePtr &r, NodePtr &parent, const NodePtr &pData, int32_t &bf, const Compare &cmp);


	NodePtr &left_rotate(NodePtr &node);
	NodePtr &right_rotate(NodePtr &node);
	NodePtr &right_left_rotate(NodePtr &node);
	NodePtr &left_right_rotate(NodePtr &node);
	void rebalance(NodePtr &node);
	NodePtr stepDeleteLeft(NodePtr &target, NodePtr &surrogate, NodePtr &prev, int32_t &bf);
	NodePtr stepDeleteRight(NodePtr &target, NodePtr &surrogate, NodePtr &prev, int32_t &bf);
private:
	NodePtr root{};
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

template<typename NodePtr, typename traits, typename DefaultCompare>
struct AVLTreeT<NodePtr, traits, DefaultCompare>::const_iterator
{
	using Tree = AVLTreeT<NodePtr, traits, DefaultCompare>;
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
	const_iterator(const AVLTreeT<NodePtr, traits, DefaultCompare>& tree)
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
	DefaultCompare	cmp{};
};

template<typename NodePtr, typename traits, typename DefaultCompare>
struct AVLTreeT<NodePtr, traits, DefaultCompare>::const_reverse_iterator
{
	using Tree = AVLTreeT<NodePtr, traits, DefaultCompare>;
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
	const_reverse_iterator(const AVLTreeT<NodePtr, traits, DefaultCompare>& tree)
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
	DefaultCompare	cmp{};
};

template<typename NodePtr, typename traits, typename DefaultCompare>
typename AVLTreeT<NodePtr, traits, DefaultCompare>::const_iterator AVLTreeT<NodePtr, traits, DefaultCompare>::begin()const
{
	return AVLTreeT<NodePtr, traits, DefaultCompare>::const_iterator{ *this };
}

template<typename NodePtr, typename traits, typename DefaultCompare>
typename AVLTreeT<NodePtr, traits, DefaultCompare>::const_iterator AVLTreeT<NodePtr, traits, DefaultCompare>::end()const
{
	return AVLTreeT<NodePtr, traits, DefaultCompare>::const_iterator{};
}

template<typename NodePtr, typename traits, typename DefaultCompare>
typename AVLTreeT<NodePtr, traits, DefaultCompare>::const_reverse_iterator AVLTreeT<NodePtr, traits, DefaultCompare>::rbegin()const
{
	return AVLTreeT<NodePtr, traits, DefaultCompare>::const_reverse_iterator{ *this };
}

template<typename NodePtr, typename traits, typename DefaultCompare>
typename AVLTreeT<NodePtr, traits, DefaultCompare>::const_reverse_iterator AVLTreeT<NodePtr, traits, DefaultCompare>::rend()const
{
	return AVLTreeT<NodePtr, traits, DefaultCompare>::const_reverse_iterator{};
}

template<typename NodePtr, typename traits, typename DefaultCompare>
template<typename Compare>
inline
NodePtr &AVLTreeT<NodePtr,traits,DefaultCompare>::findNode(NodePtr &r, const NodePtr &pData, const Compare &cmp)
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

template<typename NodePtr, typename traits, typename DefaultCompare>
template<typename Compare>
inline
NodePtr &AVLTreeT<NodePtr,traits,DefaultCompare>::lowerBound(NodePtr &r, const NodePtr &pNode, const Compare &cmp)
{
	if(r)
	{
		if( cmp(pNode, r) )
		{
			auto &tmp = lowerBound(Left(r),pNode, cmp);
			if(tmp &&
				( cmp(pNode, tmp) || !cmp(tmp, pNode) )
			) {
				return tmp;
			}
		}
		else if( cmp(r, pNode) )
		{
			return lowerBound(Right(r), pNode, cmp);
		}
	}
	return r;
}

template<typename NodePtr, typename traits, typename DefaultCompare>
template<typename Compare>
inline
bool AVLTreeT<NodePtr,traits,DefaultCompare>::insertNode(NodePtr pNewNode, const Compare &cmp)
{
	auto success = (pNewNode != nullptr);
	if(success)
	{
		if(root)
		{
			NodePtr p{ nullptr };
			success = insertNode(root, p, pNewNode, cmp);
		}
		else
		{
			root = std::move(pNewNode);
			ANIMATE(std::string{ "Insert Node: " } + toStr<const NodePtr,traits>(pNewNode));
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

template<typename NodePtr, typename traits, typename DefaultCompare>
template<typename Compare>
inline
NodePtr AVLTreeT<NodePtr,traits,DefaultCompare>::removeNode(const NodePtr &pNode, const Compare &cmp)
{
	int32_t bf = 0;
	return removeNode(root, pNode, bf, cmp);
}


template<typename NodePtr, typename traits, typename DefaultCompare>
template<typename Compare>
inline
NodePtr &AVLTreeT<NodePtr,traits,DefaultCompare>::find(const NodePtr &pData, const Compare &cmp)
{
	return pData ? findNode(root, pData, cmp) : false;
}

template<typename NodePtr, typename traits, typename DefaultCompare>
template<typename Compare>
inline
NodePtr &AVLTreeT<NodePtr,traits,DefaultCompare>::lowerBound(const NodePtr &pNode, const Compare &cmp)
{
	return lowerBound(root, pNode, cmp);
}

template<typename NodePtr, typename traits, typename DefaultCompare>
inline
void AVLTreeT<NodePtr,traits,DefaultCompare>::printTree()const
{
	::printTree<const NodePtr &, traits>(rootNode(), std::shared_ptr<Trunk>{}, false);
}

template<typename NodePtr, typename traits, typename DefaultCompare>
inline void dottyTree(const AVLTreeT<NodePtr, traits, DefaultCompare>& tree)
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

template<typename NodePtr, typename traits, typename DefaultCompare>
inline
void AVLTreeT<NodePtr,traits,DefaultCompare>::dottyTree()const
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
template<typename NodePtr, typename traits, typename DefaultCompare>
inline
NodePtr &AVLTreeT<NodePtr,traits,DefaultCompare>::left_rotate(NodePtr &p)
{
	
	auto &q = Right(p);

	REBALANCEMSG(std::string{ "Left Rotate p(" } + toStr<const NodePtr, traits>(p) + ") q(" + toStr<const NodePtr, traits>(q) + ") r(" + toStr<const NodePtr, traits>(Right(q)) + ")");

	Right(p) = std::forward<NodePtr>(Left(q));
	Left(q) = std::forward<NodePtr>(p);

	
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
template<typename NodePtr, typename traits, typename DefaultCompare>
inline
NodePtr &AVLTreeT<NodePtr,traits,DefaultCompare>::right_rotate(NodePtr &p)
{
	auto &q = Left(p);

	REBALANCEMSG(std::string{ "Right Rotate p(" } + toStr<const NodePtr, traits>(p) + ") q(" + toStr<const NodePtr, traits>(q) + ") r(" + toStr<const NodePtr, traits>(Left(q)) + ")");

	Left(p) = std::forward<NodePtr>(Right(q));
	Right(q) = std::forward<NodePtr>(p);

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
template<typename NodePtr, typename traits, typename DefaultCompare>
inline
NodePtr &AVLTreeT<NodePtr,traits,DefaultCompare>::left_right_rotate(NodePtr &p)
{
	auto &q = Left(p);
	auto &r = Right(q);

	REBALANCEMSG(std::string{ "Left Right Rotate p(" } + toStr<const NodePtr, traits>(p) + ") q(" + toStr<const NodePtr, traits>(q) + ") r(" + toStr<const NodePtr, traits>(r) + ")");

	Right(q) = std::forward<NodePtr>(Left(r));
	Left(p) = std::forward<NodePtr>(Right(r));
	Right(r) = std::forward<NodePtr>(p);
	Left(r) = std::forward<NodePtr>(q);

	
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
template<typename NodePtr, typename traits, typename DefaultCompare>
inline
NodePtr &AVLTreeT<NodePtr,traits,DefaultCompare>::right_left_rotate(NodePtr &p)
{
	auto &q = Right(p);
	auto &r = Left(q);

	REBALANCEMSG(std::string{ "Left Right Rotate p(" } + toStr<const NodePtr, traits>(p) + ") q(" + toStr<const NodePtr, traits>(q) + ") r(" + toStr<const NodePtr, traits>(r) + ")");

	Left(q) = std::forward<NodePtr>(Right(r));
	Right(p) = std::forward<NodePtr>(Left(r));
	Left(r) = std::forward<NodePtr>(p);
	Right(r) = std::forward<NodePtr>(q);

	
	traits::bf(q, traits::bf(q) + (1 - std::min(0, traits::bf(r)))); // bf(q) += 1 - min(0, bf(r));
	traits::bf(p, traits::bf(p) - (1 + std::max(std::max(0, traits::bf(r)) + 1, traits::bf(r) + traits::bf(q)))); // bf(p) -= 1 + max(max(0, bf(r)) + 1, bf(r) + bf(q));
	traits::bf(r, traits::bf(r) + (std::max(0, traits::bf(q)) + std::min(0, traits::bf(p)))); // bf(r) += max(0, bf(q)) + min(0, bf(p));

	return r;
}

template<typename NodePtr, typename traits, typename DefaultCompare>
inline
void AVLTreeT<NodePtr,traits,DefaultCompare>::rebalance(NodePtr &r)
{
	REBALANCEMSG("");
	if(r)
	{
		if(traits::bf(r) == -2 )
		{
			r = std::forward<NodePtr>((traits::bf(Left(r)) == 1)
				? left_right_rotate(r)
				: right_rotate(r));
		}
		else if( traits::bf(r) == 2 )
		{
			r = std::forward<NodePtr>((traits::bf(Right(r)) == -1)
				? right_left_rotate(r)
				: left_rotate(r));
		}
	}
}

template<typename NodePtr, typename traits, typename DefaultCompare>
template<typename Compare>
inline
bool
AVLTreeT<NodePtr,traits,DefaultCompare>::insertNode(NodePtr &r, NodePtr &p, NodePtr &pData, const Compare &cmp)
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
				success = insertNode(Left(r), r, pData, cmp);
			}
			else
			{
				Left(r) =  std::move(pData);
				traits::bf(r, traits::bf(r) - 1 );
				ANIMATE(std::string{ "Insert Node: " } + toStr<const NodePtr, traits>(pData));
				success = true;
			}
		}
		else if( cmp(r, pData) )
		{
			if( Right(r) )
			{
				success = insertNode(Right(r), r, pData, cmp);
			}
			else
			{
				Right(r) = std::move(pData);
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
			
			rebalance(r);
			RANIMATE();
		}
	}
	return success;
}

template<typename NodePtr, typename traits, typename DefaultCompare>
inline
NodePtr
AVLTreeT<NodePtr,traits,DefaultCompare>::stepDeleteLeft(NodePtr &target, NodePtr &surrogate, NodePtr &prev, int32_t &bf)
{
	NodePtr rm{};
	
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
		rm = std::move(target);
		target = std::move(surrogate);
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

template<typename NodePtr, typename traits, typename DefaultCompare>
inline
NodePtr
AVLTreeT<NodePtr,traits,DefaultCompare>::stepDeleteRight(NodePtr &target, NodePtr &surrogate, NodePtr &prev, int32_t &bf)
{
	NodePtr rm{};
	
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
		rm = std::move(target);
		target = std::move(surrogate);
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

template<typename NodePtr, typename traits, typename DefaultCompare>
template<typename Compare>
inline NodePtr
AVLTreeT<NodePtr,traits,DefaultCompare>::removeNode(NodePtr &r, NodePtr &parent, const NodePtr &pData, int32_t &bf, const Compare &cmp)
{
	auto direction = 0;
	NodePtr old{};
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

template<typename NodePtr, typename traits, typename DefaultCompare>
template<typename Compare>
inline
NodePtr
AVLTreeT<NodePtr,traits,DefaultCompare>::removeNode(NodePtr &r,const NodePtr &pData, int32_t &bf, const Compare &cmp)
{
	auto direction = 0;
	NodePtr old{};
	if(r)
	{
		if( cmp(pData, r) )
		{
			direction = 1;
			old = removeNode(Left(r), pData, bf, cmp);
		}
		else if( cmp(r, pData) )
		{
			direction = -1;
			old = removeNode(Right(r), pData, bf, cmp);
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

