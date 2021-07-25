#pragma once

#include "Astar.h"

namespace sst::ai
{
	class AstarNodeMaster
	{
	public:
		AstarNodeMaster(uint32 row, uint32 col);
		~AstarNodeMaster();

		Astar::Node* GetNode(const Astar::Node::Location& node);

	private:
		uint32 m_row{ 0 };
		uint32 m_col{ 0 };
		Astar::Node* m_nodes{ nullptr };
	};

}