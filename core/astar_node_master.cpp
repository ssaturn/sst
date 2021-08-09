#include "stdafx.h"
#include "astar_node_master.h"


namespace sst::ai::path_finder
{
	astar_node_master::astar_node_master(const uint32 row, const uint32 col)
		: row_(row)
		, col_(col)
	{
		nodes_ = new astar::node[row * col];
	}

	astar_node_master::~astar_node_master()
	= default;

	astar::node* astar_node_master::get_node(const astar::node::location& node) const
	{
		const uint32 index = node.x * row_ + node.y;
		nodes_[index].reset();
		return &nodes_[index];
	}
}
