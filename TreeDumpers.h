#ifndef TREE_DUMPERS_H_INCLUDED
#define TREE_DUMPERS_H_INCLUDED
#pragma once

#include <string>
#include <iostream>
#include <type_traits>

enum class OutputType
{
	NoOutput = 0,
	TextOutput = 1,
	DotOutput = 2
};

class TreeDumper
{
public:
	TreeDumper(std::ostream &os = std::cout);

	template<typename Tree>
	void REBALANCEMSG(const std::string &, const Tree &);

	template<typename Tree>
	void ANIMATE(const std::string &, const Tree &t);

	template<typename Tree>
	void RANIMATE(const Tree &t);

	static void setOutputType(OutputType type);
private:
	std::string  rebalance_msg{};
	std::ostream &os;
	inline static OutputType output = OutputType::NoOutput;
};

inline TreeDumper::TreeDumper(std::ostream &os_)
	: os{ os_ }
{
}

template<typename Tree>
inline 
void TreeDumper::REBALANCEMSG(const std::string &msg, const Tree &)
{
	rebalance_msg = msg;
}

template<typename Tree>
inline void TreeDumper::ANIMATE(const std::string &msg, const Tree &tree)
{
	using NodeType = typename Tree::value_type;
	using NodeTypeB = std::remove_pointer_t<NodeType>;
	using Traits = typename NodeTypeB::traits;
	switch (output)
	{
		case OutputType::TextOutput:
			os << "======================== " << msg << " ========================\n";
			::printTree<NodeType, Traits>(tree.rootNode(), std::shared_ptr<Trunk>{}, false);
			break;
		case OutputType::DotOutput:
			os << "{\"title\" : \'" << msg << "\', \"dot\" : `";
			dottyTree(tree, os);
			os << "`},\n";
			break;
		default:
			break;
	}
}

template<typename Tree>
inline void TreeDumper::RANIMATE(const Tree &tree)
{
	ANIMATE(rebalance_msg, tree);
}

inline
void TreeDumper::setOutputType(OutputType type)
{
	output = type;
}

#endif // TREE_DUMPERS_H_INCLUDED
