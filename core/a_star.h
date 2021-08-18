#pragma once

#include <map>
#include <queue>
#include <list>
#include <functional>
#include <atomic>

namespace sst::ai::path_finder
{
	class astar_map;
	class astar_node_master;
	class astar
	{
	public:
		struct node
		{
			struct location
			{
				uint32 x = 0;
				uint32 y = 0;

				void reset()
				{
					x = 0;
					y = 0;
				}
			};

			float cost{ 0 };
			float total{0};
			bool on_open{ false };
			bool on_closed{ false };
			location loc{};
			node* parent{ nullptr };

			void reset()
			{
				cost = 0;
				total = 0;
				on_open = false;
				on_closed = false;
				loc.reset();
				parent = nullptr;
			}
		};

		struct greater
		{
			bool operator()(const node* lhs, const node* rhs) const
			{
				return (lhs->total > rhs->total);
			}
		};

		astar(const astar_map* map);
		~astar();

		bool search(const node::location& start, const node::location& goal);

	private:
		using open_list = std::priority_queue<node*, std::vector<node*>, greater>;
		using closed_list = std::list<node>;

		astar_node_master* master_ = nullptr;
		const astar_map* map_ = nullptr;
		closed_list closed_;
		open_list open_;

	};

	class astar_map
	{
		uint32 row_{ 0 };
		uint32 col_{ 0 };
	
	public:
		uint32 max_row() const { return row_; }
		uint32 max_col() const { return col_; }
	};
};
