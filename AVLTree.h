#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED
#pragma once

#include <memory>
#include <algorithm>
#include <cassert>

struct node;

using NodePtr = std::unique_ptr<node>;

struct node
{
    uint64_t data{0};
    int32_t bf{0};
	NodePtr left{};
    NodePtr right{};
};

class AVLTree
{
public:
	AVLTree()=default;
	AVLTree(const AVLTree&)=delete;
	AVLTree &operator=(const AVLTree&)=delete;
	AVLTree(AVLTree &&)=default;
	AVLTree &operator=(AVLTree &&)=default;
	~AVLTree() = default;

	bool insertNode(uint64_t data);
	bool  deleteNode(uint64_t data);
	const node *find(uint64_t data);
	const NodePtr &rootNode()const { return root; }
private:
	const node *findNode(const node *r, uint64_t data)const;
	bool insertNode(NodePtr &r,NodePtr &p, uint64_t data);
	bool  deleteNode(NodePtr &r,uint64_t data, uint32_t &bf);
	NodePtr left_rotate(NodePtr &&node);
	NodePtr right_rotate(NodePtr &&node);
	NodePtr right_left_rotate(NodePtr &&node);
	NodePtr left_right_rotate(NodePtr &&node);
	NodePtr rebalance(NodePtr &&node);
	NodePtr stepDeleteLeft(NodePtr &target, NodePtr &surrogate, NodePtr &prev, uint32_t &bf);
	NodePtr stepDeleteRight(NodePtr &target, NodePtr &surrogate, NodePtr &prev, uint32_t &bf);
private:
	std::unique_ptr<node> root{};
};

void printTree(const AVLTree &tree);
int32_t height(const NodePtr &r);
int32_t BF(const NodePtr &r);
void checkTree(const NodePtr &r);




#endif // AVLTREE_H_INCLUDED
