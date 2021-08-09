#include "stdafx.h"
#include "astar.h"
#include "astar_node_master.h"


namespace sst::ai::path_finder
{
	astar::astar( const astar_map* map )
		: master_( new astar_node_master( map->max_row(), map->max_col() ) )
		  , map_( map )
	{

	}

	astar::~astar()
	{
		delete master_;
	}

	bool astar::search( const node::location& start, [[maybe_unused]] const node::location& goal )
	{
		if ( map_ == nullptr )
		{
			return false;
		}

		// param check

		// clear 
		while ( !open_.empty() )
		{
			//open_.pop();
		}
		closed_.clear();

		open_.push( master_->get_node( start ) );

		node* startNode = new node;
		startNode->cost = 0;
		startNode->loc.x = start.x;
		startNode->loc.y = start.y;
		startNode->total = 0;
		startNode->parent = nullptr;

		open_.emplace( startNode );

		while ( !open_.empty() )
		{
			auto node = open_.top();
			node;


		}

		//
		return true;
	}
}
