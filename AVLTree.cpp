#include "AVLTree.h"
#include <algorithm>
#include <iostream>

using namespace std;

bool AVLTree::insertNode(uint64_t data)
{
	auto success = false;
	if(root)
	{
		success = insertNode(root, nullptr, data);
	}
	else
	{
		root = new node{};
		if( root )
		{
			root->data = data;
			success = true;
		}
	}
	return success;
}

node *AVLTree::find(uint64_t data)
{
	return findNode(root, data);
}

node *AVLTree::findNode(node *r, uint64_t data)
{
	if(r)
	{
		if( data < r->data )
		{
			r = findNode(r->left,data);
		}
		else if( r->data < data )
		{
			r = findNode(r->right, data);
		}
	}
	return r;
}

bool AVLTree::deleteNode(uint64_t data)
{
	uint32_t bf = 0;
	return deleteNode(root, data, bf);
}


/*         left rotate
 *
 *    1              2
 *     \            / \
 *      2    ==>   1   3
 *       \ 
 *        3
 */
node *AVLTree::left_rotate(node *p)
{
	// node *p = n;
	node *q = p->right;

	p->right = q->left;
	q->left = p;
	// n = q;

	p->bf -= 1 + max(0, q->bf);
	q->bf -= 1 - min(0, p->bf);
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
node *AVLTree::right_rotate(node *p)
{
//	node *p = n;
	node *q = p->left;
	p->left = q->right;
	q->right = p;
	// n = q;

	p->bf += 1 - min(0, q->bf);
	q->bf += 1 + max(0, p->bf);
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
node *AVLTree::left_right_rotate(node *p)
{
	//node *p = n;
	//node *q = n->left;
	node *q = p->left;
	node *r = q->right;

	q->right = r->left;
	p->left = r->right;
	r->right = p;
	r->left = q;

	// n = r;

	q->bf -= 1 + max(0, r->bf);
	p->bf += 1 - min(min(0, r->bf) - 1, r->bf + q->bf);
	r->bf += max(0, p->bf) + min(0, q->bf);
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
node *AVLTree::right_left_rotate(node *p)
{
	// node *p = n;
	// node *q = n->right;
	node *q = p->right;
	node *r = q->left;

	q->left = r->right;
	p->right = r->left;
	r->left = p;
	r->right = q;

	// n = r;

	q->bf += 1 - min(0, r->bf);
	p->bf -= 1 + max(max(0, r->bf) + 1, r->bf + q->bf);
	r->bf += max(0, q->bf) + min(0, p->bf);
	return r;
}


node *
AVLTree::rebalance(node *r)
{
	if(r)
	{
		if(r->bf == -2 )
		{
			if( r->left->bf == 1 )
			{
				r = left_right_rotate(r);
			}
			else
			{
				r = right_rotate(r);
			}
		}
		else if( r->bf == 2 )
		{
			if( r->right && r->right->bf == -1 )
			{
				r = right_left_rotate(r);
			}
			else
			{
				r = left_rotate(r);
			}
		}
	}
	return r;
}


bool
AVLTree::insertNode(node *&r,node *p, uint64_t data)
{
	auto success = false;
	auto before = 0u;
	if( r )
	{
		before = r->bf;
		if( data < r->data )
		{
			if( r->left )
			{
				success = insertNode(r->left, r, data );
			}
			else
			{
				r->left = new node{};
				if( r->left )
				{
					r->left->data = data;
					r->bf--;
					success = true;
				}
			}
		}
		else if( r->data < data )
		{
			if( r->right )
			{
				success = insertNode(r->right, r, data );
			}
			else
			{
				r->right = new node{};
				if( r->right )
				{
					r->right->data = data;
					r->bf++;
					success = true;
				}
			}
		}
	}
	if(success)
	{
		if (p && r->bf && before == 0)
		{
			p->bf += (p->left == r ? (-1) : (+1));
		}
		
		r = rebalance(r);
	}
	return success;
}

node *
AVLTree::stepDeleteLeft(node *target, node *&surrogate, node *&prev, uint32_t &bf)
{
	node *rm = nullptr;
	
	if( ! surrogate->left )
	{
		rm = surrogate;
		std::swap(target->data, rm->data);
		surrogate = surrogate->right;
		bf = 1;
	}
	else
	{
		rm = stepDeleteLeft(target, surrogate->left, surrogate, bf );
	}
	
	if( bf )
	{
		if( prev == target )
		{
			prev->bf--;
		}
		else
		{
			prev->bf++;
		}

		prev = rebalance(prev);

		if( prev->bf == 1 || prev->bf == -1 )
		{
			bf = 0;
		}
	}
	return rm;
}

node *
AVLTree::stepDeleteRight(node *target, node *&surrogate, node *&prev, uint32_t &bf)
{
	node *rm = nullptr;
	
	if( ! surrogate->right )
	{
		rm = surrogate;
		std::swap(target->data, rm->data);
		surrogate = surrogate->left;
		bf = 1;
	}
	else
	{
		rm = stepDeleteRight(target, surrogate->right, surrogate, bf );
	}
	
	if( bf )
	{
		if( prev == target )
		{
			prev->bf++;
		}
		else
		{
			prev->bf--;
		}

		prev = rebalance(prev);

		if( prev->bf == 1 || prev->bf == -1 )
		{
			bf = 0;
		}
	}
	return rm;
}

bool
AVLTree::deleteNode(node *&r,uint64_t data, uint32_t &bf)
{
	auto success = false;
	auto direction = 0;
	if(r)
	{
		if( data < r->data )
		{
			direction = 1;
			success = deleteNode(r->left, data,bf);
		}
		else if( r->data < data )
		{
			direction = -1;
			success = deleteNode(r->right, data,bf);
		}
		else
		{
			node *old = nullptr;
			success = true;
			
			if( ! r->left )
			{
				old = r;
				r = r->right;
				bf = 1;
			}
			else if ( ! r->right )
			{
				old =r;
				r = r->left;
				bf = 1;
			}
			else
			{
				if( r->bf > 0 )
				{
					old = stepDeleteLeft(r, r->right, r, bf);
				}
				else
				{
					old = stepDeleteRight(r, r->left, r, bf);
				}
				
				if( r->bf == 1 || r->bf == -1 )
				{
					bf = 0;
				}
			}
			delete old;
		}
		
		if( direction )
		{
			if(bf)
			{
				r->bf += direction;
			}
			
			r = rebalance(r);
			
			if( r->bf )
			{
				bf = 0;
			}
		}
	}
	return success;
}

# if 0
struct node*
AVLTree::insertNode(node *r,node *parent, int data)
{
	if(r==nullptr)
	{
		struct node *n;
		n = new struct node;
		n->data = data;
		r = n;
		r->left = r->right = NULL;
		r->bf = 0; 
		updateBalance = true;
	}
	else
	{
		auto bf = r->bf;
		if( data < r->data )
		{
			r->left = insert(r->left, data, updateBalance);
			if(updateBalance)
			{
				--(r->bf);
			}
		}
		else
		{
			r->right = insert(r->right, data, updateBalance);
			if(updateBalance)
			{
				++(r->bf);
			}
		}
		
		if( updateBalance && (r->bf < -1) || (r->bf > 1) )
		{
			rebalance(r);
			updateBalance = false;
		}
		
		//~ if( bf == 2 )
		//~ {
			//~ if( r->left->bf==1 )
			//~ {
				//~ r = llrotation(r);
				//~ r->bf = 0;
			//~ }
			//~ else if(r->left->bf==-1)
			//~ {
				//~ r = lrrotation(r);
				//~ r->bf = 0;
			//~ }
		//~ }
		//~ else if( bf == -2 )
		//~ {
			//~ if( r->right->bf==-1 )
			//~ {
				//~ r = rrrotation(r);
				//~ r->bf = 0;
			//~ }
			//~ else if(r->right->bf==1)
			//~ {
				//~ r = rlrotation(r);
				//~ r->bf = 0;
			//~ }
		//~ }
	}
	return r;
}
#endif // 0
