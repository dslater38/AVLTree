#include "AVLTree.h"
#include <algorithm>
#include <iostream>

using namespace std;

#if defined(DEBUG) || defined(_DEBUG)
static string rebalance_msg="";
#define REBALANCEMSG(a) rebalance_msg=a
#define ANIMATE(a, b) animateTree(a,b)
#define RANIMATE(a) if( !rebalance_msg.empty() ) animateTree(a,rebalance_msg)
#else
#define REBALANCEMSG(a) ((void)0)
#define ANIMATE(a,b) ((void)0)
#define RANIMATE(a) ((void)0)
#endif

static void animateTree(const AVLTree &tree, string msg);

bool AVLTree::insertNode(uint64_t data)
{
	auto success = false;
	if(root)
	{
		auto p = NodePtr{};
		success = insertNode(root, p, data);
	}
	else
	{
		root = make_unique<node>();
		if( root )
		{
			root->data = data;
			success = true;
			ANIMATE(*this, "insertNode");
		}
	}
	return success;
}

const node *AVLTree::find(uint64_t data)
{
	return findNode(root.get(), data);
}

const node *AVLTree::findNode(const node *r, uint64_t data)const
{
	if(r)
	{
		if( data < r->data )
		{
			r = findNode(r->left.get(),data);
		}
		else if( r->data < data )
		{
			r = findNode(r->right.get(), data);
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
NodePtr AVLTree::left_rotate(NodePtr &&p)
{
	NodePtr q = move(p->right);
	p->right = move(q->left);

	p->bf -= 1 + max(0, q->bf);
	q->bf -= 1 - min(0, p->bf);

	q->left = move(p);

	
	return move(q);
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
NodePtr AVLTree::right_rotate(NodePtr &&p)
{
	NodePtr q = move(p->left);
	p->left = move(q->right);

	p->bf += 1 - min(0, q->bf);
	q->bf += 1 + max(0, p->bf);

	q->right = move(p);
		
	return move(q);
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
NodePtr AVLTree::left_right_rotate(NodePtr &&p)
{
	NodePtr q = move(p->left);
	NodePtr r = move(q->right);

	q->right = move(r->left);
	p->left = move(r->right);

	q->bf -= 1 + max(0, r->bf);
	p->bf += 1 - min(min(0, r->bf) - 1, r->bf + q->bf);
	r->bf += max(0, p->bf) + min(0, q->bf);

	r->right = move(p);
	r->left = move(q);

	return move(r);
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
NodePtr AVLTree::right_left_rotate(NodePtr &&p)
{
	NodePtr q = move(p->right);
	NodePtr r = move(q->left);

	q->left = move(r->right);
	p->right = move(r->left);

	q->bf += 1 - min(0, r->bf);
	p->bf -= 1 + max(max(0, r->bf) + 1, r->bf + q->bf);
	r->bf += max(0, q->bf) + min(0, p->bf);

	r->left = move(p);
	r->right = move(q);
	
	return move(r);
}


NodePtr 
AVLTree::rebalance(NodePtr &&r)
{
	REBALANCEMSG("");
	if(r)
	{
		if(r->bf == -2 )
		{
			if( r->left->bf == 1 )
			{
				r = left_right_rotate(move(r));
				REBALANCEMSG("left_right_rotate");
			}
			else
			{
				r = right_rotate(move(r));
				REBALANCEMSG("right_rotate");
			}
		}
		else if( r->bf == 2 )
		{
			if( r->right && r->right->bf == -1 )
			{
				r = right_left_rotate(move(r));
				REBALANCEMSG("right_left_rotate");
			}
			else
			{
				r = left_rotate(move(r));
				REBALANCEMSG("left_rotate");
			}
		}
	}
	return move(r);
}


bool
AVLTree::insertNode(NodePtr &r,NodePtr &p, uint64_t data)
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
				r->left = make_unique<node>();
				if( r->left )
				{
					r->left->data = data;
					r->bf--;
					ANIMATE(*this, "insertNode");
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
				r->right = make_unique<node>();
				if( r->right )
				{
					r->right->data = data;
					r->bf++;
					ANIMATE(*this, "insertNode");
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
		
		r = rebalance(move(r));
		RANIMATE(*this);
	}
	return success;
}

NodePtr
AVLTree::stepDeleteLeft(NodePtr &target, NodePtr &surrogate, NodePtr &prev, uint32_t &bf)
{
	NodePtr rm{};
	
	if( ! surrogate->left )
	{
		rm = move(surrogate);
		std::swap(target->data, rm->data);
		surrogate = move(rm->right);
		bf = 1;
		ANIMATE(*this, "stepDeleteLeft");
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

		prev = rebalance(move(prev));
		RANIMATE(*this);

		if( prev->bf == 1 || prev->bf == -1 )
		{
			bf = 0;
		}
	}
	return move(rm);
}

NodePtr
AVLTree::stepDeleteRight(NodePtr &target, NodePtr &surrogate, NodePtr &prev, uint32_t &bf)
{
	NodePtr rm{};
	
	if( ! surrogate->right )
	{
		rm = move(surrogate);
		std::swap(target->data, rm->data);
		surrogate = move(rm->left);
		bf = 1;
		ANIMATE(*this, "stepDeleteRight");
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

		prev = rebalance(move(prev));
		RANIMATE(*this);

		if( prev->bf == 1 || prev->bf == -1 )
		{
			bf = 0;
		}
	}
	return move(rm);
}

bool
AVLTree::deleteNode(NodePtr &r,uint64_t data, uint32_t &bf)
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
			NodePtr old = nullptr;
			success = true;
			
			if( ! r->left )
			{
				old = move(r);
				r = move(r->right);
				bf = 1;
				ANIMATE(*this, "deleteNode");
			}
			else if ( ! r->right )
			{
				old = move(r);
				r = move(r->left);
				bf = 1;
				ANIMATE(*this, "deleteNode");
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
			// delete old;
		}
		
		if( direction )
		{
			if(bf)
			{
				r->bf += direction;
			}
			
			r = rebalance(move(r));
			RANIMATE(*this);
			
			if( r->bf )
			{
				bf = 0;
			}
		}
	}
	return success;
}

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
static void showTrunks(const Trunk *p)
{
    if (p == nullptr) {
        return;
    }
 
    showTrunks(p->prev.get());
    cout << p->str;
}

// Recursive function to print a binary tree.
// It uses the inorder traversal.
static void printTree(const NodePtr &root, shared_ptr<Trunk> prev, bool isLeft)
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
    // cout << root->data << endl;
	cout << root->data;
	auto bf = BF(root);
	while(bf>0)
	{
		cout << "+";
		--bf;
	}
	while(bf<0)
	{
		cout << "-";
		++bf;
	}
	cout << endl;

    if (prev) {
        prev->str = prev_str;
    }
    trunk->str = "   |";
 
    printTree(root->left, trunk, false);
}

static void animateTree(const AVLTree &tree, string msg)
{
	cout << "======================== "	<< msg << " ========================\n";
	printTree(tree);
}

void printTree(const AVLTree &tree)
{
	printTree(tree.rootNode(), unique_ptr<Trunk>{}, false);
}

int32_t height(const NodePtr &r)
{
	int32_t h = 0;
	if(r)
	{
		h = std::max(height(r->left), height(r->right)) + 1;
	}
	return h;
}

int32_t BF(const NodePtr &r)
{
	int32_t bf = 0;
	if(r)
	{
		bf = height(r->right) - height(r->left);
	}
	return bf;
}

void checkTree(const NodePtr &r)
{
	if(r)
	{
		if(r->left)
		{
			checkTree(r->left);
		}
		if(r->right)
		{
			checkTree(r->right);
		}
		assert( (r->bf) == BF(r) );
	}
}
