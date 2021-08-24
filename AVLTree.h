#include <memory>


struct node
{
    uint64_t data{0};
    uint32_t bf{0};
	struct node *left{nullptr};
    struct node *right{nullptr};
};

class AVLTree
{
public:
	AVLTree();

	const node *insertNode(uint64_t data);
	const node *deleteNode(uint64_t data);
	
private:
	node	*root{nullptr};
};

struct node {
    uint64_t data;
    uint32_t bf;
	struct node *left;
    struct node *right;
	struct node *front;
    struct node *back;
	
	uint32_t bf1() {
		return ((bf1 & 0x00000003) - 1) - 1;
	}
	
	void bf1(uint32_t bf) {
		ASSERT( -1 <= bf && bf <= 1);
		bf |= ((bf+1) & 0x00000003);
	}
	
	uint32_t bf2() {
		ASSERT( -1 <= bf && bf <= 1);
		return ((bf1 & 0x0000000C)>>2) - 1;
	}
	
	void bf2(uint32_t bf) {
		ASSERT( -1 <= bf && bf <= 1);
		bf |= (((bf+1) & 0x00000003) << 2);
	}	
};

uint32_t height(struct node *r)
{
	uint32_t h = 0;
	if(r)
	{
		h = std::max(height(r->left), height(r->right)) + 1;
	}
	return h;
}

uint32_t BF(struct node *r)
{
	uint32_t bf = 0;
	if(r)
	{
		bf = height(r->right) - height(r->left);
	}
	return bf;
}

void checkTree(struct node *r)
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
		ASSERT( (r->bf1()) == BF(r) );
	}
}


struct node * deleteNode(struct node *p,int data)
{
	if(p->left == NULL && p->right == NULL){
			if(p==this->root)
				this->root = NULL;
		delete p;
		return NULL;
	}

	struct node *t;
	struct node *q;
	uint32_t bf = p->bf1();
	if(p->data < data)
	{
		p->right = deleteNode(p->right,data);
		--bf;
	}
	else if(p->data > data)
	{
		p->left = deleteNode(p->left,data);
		++bf;
	}
	else if(p->left != NULL)
	{
		q = inpre(p->left);
		p->data = q->data;
		p->left=deleteNode(p->left,q->data);
		++bf;
	}
	else{
		q = insuc(p->right);
		p->data = q->data;
		p->right = deleteNode(p->right,q->data);
		--bf;
	}

	if( bf == 2 )
	{
		if( p->left->bf1() == 1 )
		{
			p = llrotation(p);
		}
		else if( p->left->bf1() == -1 )
		{
			p = lrrotation(p);
		}
		else if( p->left->bf1() == 0 )
		{
			p = llrotation(p);
		}
		p->bf1(0);
	}
	else if( bf == -2 )
	{
		if( p->right->bf1() == -1 )
		{
			p = rrrotation(p);
		}
		else if( p->right->bf1() == 1 )
		{
			p = rlrotation(p);
		}
		else if( p->right->bf1() == 0 )
		{
			p = llrotation(p);
		}
		p->bf1(0);
	}
	
	//~ if(bf==2 && p->left->bf==1)
	//~ { 
		//~ p = llrotation(p);
	//~ }
	//~ else if(bf(p)==2 && bf(p->left)==-1)
	//~ { 
		//~ p = lrrotation(p);
	//~ }
	//~ else if(bf(p)==2 && bf(p->left)==0)
	//~ { 
		//~ p = llrotation(p);
	//~ }
	//~ else if(bf(p)==-2 && bf(p->right)==-1)
	//~ { 
		//~ p = rrrotation(p);
	//~ }
	//~ else if(bf(p)==-2 && bf(p->right)==1)
	//~ { 
		//~ p = rlrotation(p);
	//~ }
	//~ else if(bf(p)==-2 && bf(p->right)==0)
	//~ { 
		//~ p = llrotation(p);
	//~ }

	
	return p;
}

struct node* inpre(struct node* p)
{
	while(p->right!=NULL)
		p = p->right;
	return p;    
}

struct node* insuc(struct node* p)
{
	while(p->left!=NULL)
		p = p->left;

	return p;    
}

    struct node * llrotation(struct node *n)
	{
        struct node *p;
        struct node *tp;
        p = n;
        tp = p->left;

        p->left = tp->right;
        tp->right = p;

        return tp; 
    }


int calheight(struct node *p)
{
		if(p->left && p->right)
		{
			if (p->left->height < p->right->height)
				return p->right->height + 1;
			else 
				return p->left->height + 1;
		}
		else if(p->left)
		{
		   return p->left->height + 1;
		}
		else if(p->right)
		{
		   return p->right->height + 1;
		}
		return 0;

}
