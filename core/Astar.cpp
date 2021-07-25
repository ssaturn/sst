#include "stdafx.h"
#include "Astar.h"
#include "AstarNodeMaster.h"

namespace sst::ai
{
	Astar::Astar( const AstarMap* map )
		: m_master( new AstarNodeMaster( map->MaxRow(), map->MaxCol() ) )
		  , m_map( map )
	{

	}

	Astar::~Astar()
	{
		delete m_master;
	}

	bool Astar::Search( const Node::Location& start, [[maybe_unused]] const Node::Location& goal )
	{
		if ( m_map == nullptr )
		{
			return false;
		}

		// param check

		// clear 
		while ( !m_open.empty() )
		{
			//m_open.pop();
		}
		m_closed.clear();

		m_open.push( m_master->GetNode( start ) );

		Node* startNode = new Node;
		startNode->cost = 0;
		startNode->loc.x = start.x;
		startNode->loc.y = start.y;
		startNode->total = 0;
		startNode->parent = nullptr;

		m_open.emplace( startNode );

		while ( !m_open.empty() )
		{
			auto node = m_open.top();
			node;


		}

		//
		return true;
	}
}
