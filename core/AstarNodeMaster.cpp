#include "stdafx.h"
#include "AstarNodeMaster.h"

namespace sst::ai
{

	AstarNodeMaster::AstarNodeMaster(uint32 row, uint32 col)
		: m_row(row)
		, m_col(col)
	{
		m_nodes = new Astar::Node[row * col];
	}

	AstarNodeMaster::~AstarNodeMaster()
	{

	}

	Astar::Node* AstarNodeMaster::GetNode(const Astar::Node::Location& node)
	{
		uint32 index = node.x * m_row + node.y;
		m_nodes[index].Reset();
		return &m_nodes[index];
	}
}
