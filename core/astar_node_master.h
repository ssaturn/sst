#pragma once

#include "astar.h"
#include "common.h"

namespace sst::ai::path_finder
{
	class astar_node_master
	{
	public:
		astar_node_master(uint32 row, uint32 col);
		~astar_node_master();

		astar::node* get_node(const astar::node::location& node) const;

	private:
		uint32 row_{ 0 };
		uint32 col_{ 0 };
		astar::node* nodes_{ nullptr };
	};

}
