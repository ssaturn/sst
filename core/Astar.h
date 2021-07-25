#pragma once

#include <map>
#include <queue>
#include <list>
#include <functional>
#include <atomic>

namespace sst::ai
{
	class AstarMap;
	class AstarNodeMaster;
	class Astar
	{
	public:
		struct Node
		{
			struct Location
			{
				uint32 x = 0;
				uint32 y = 0;

				void Reset()
				{
					x = 0;
					y = 0;
				}
			};

			float cost = 0;
			float total = 0;
			bool onOpen = false;
			bool onClosed = false;
			Location loc;
			Node* parent = nullptr;

			void Reset()
			{
				cost = 0;
				total = 0;
				onOpen = false;
				onClosed = false;
				loc.Reset();
				parent = nullptr;
			}
		};

		struct Greater
		{
			bool operator()(const Node* lhs, const Node* rhs) const
			{
				return (lhs->total > rhs->total);
			}
		};

		Astar(const AstarMap* map);
		~Astar();

		bool Search(const Node::Location& start, const Node::Location& goal);

	private:
		using OpenList = std::priority_queue<Node*, std::vector<Node*>, Greater>;
		using ClosedList = std::list<Node>;

		AstarNodeMaster* m_master = nullptr;
		const AstarMap* m_map = nullptr;
		ClosedList m_closed;
		OpenList m_open;

	};

	class AstarMap
	{
	public:
		uint32 m_row;
		uint32 m_col;

		uint32 MaxRow() const { return m_row; };
		uint32 MaxCol() const { return m_col; };
	};
};
