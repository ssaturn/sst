#include "stdafx.h"
#include "hsm.h"

namespace sst
{
	hsm::hsm()
	{
	}

	hsm::~hsm()
	{
		finish();
	}

	bool hsm::start(hsm_owner* owner)
	{
		//ASSERT_CRASH( owner );

		owner_ = owner;
		current_ = &root_;
		current_->init(this, state_root, 0);

		return true;
	}

	void hsm::on_tick(const uint32 age)
	{
		state* state = current_;

		while (state != nullptr) // keep going to parent
		{
			state = state->on_tick(age);
		}
	}

	hsm::state* hsm::get_state(const state_type stateId)
	{
		return root_.get_state(stateId);
	}

	void hsm::tran(const state_type target_state)
	{
		if (current_ == nullptr)
		{
			return;
		}

		if (!current_->is_transable(target_state))
		{
			return;
		}

		state::path path;

		bool rc = current_->get_transition_path_to(target_state, path);

		if (!rc)
		{
			return;
		}

		state::path::iterator i(path.begin());
		state::path::iterator iEnd(path.end());

		for (; i != iEnd; ++i)
		{
			state::node& n = *i;
			//ASSERT_CRASH( n.state != 0 );

			bool targetFound = 0;

			switch (n.code)
			{
			case state::node::ENTER:
			{
				n.state->on_enter();
			}
			break;
			case state::node::EXIT:
			{
				n.state->on_exit();
			}
			break;
			case state::node::TARGET:
			{
				targetFound = true;

				current_ = n.state;
				n.state->on_enter();
			}
			break;
			case state::node::NOOP:		// NOOP
			{
			}
			break;
			default:
				break;
			}
		}
	}

	void hsm::finish()
	{
		root_.finish();
	}

	hsm::state::state()
	{
	}

	hsm::state::~state()
	{
		finish();
	}

	bool hsm::state::init(hsm* hsm, const size_t id, state* parent)
	{
		//ASSERT_CRASH( hsm != nullptr );

		hsm_ = hsm;
		id_ = id;
		parent_ = parent;

		if (parent)
		{
			parent->_Attach(this);
		}

		target_id_ = 0;

		return _Init();
	}

	void hsm::state::on_enter()
	{
		_On_enter();
	}

	hsm::state* hsm::state::on_tick(uint32 age)
	{
		return _On_tick(age);
	}

	void hsm::state::on_exit()
	{
		_On_exit();
	}

	void hsm::state::finish()
	{
		for (auto state : children_)
		{
			//ASSERT_CRASH( state != 0 );

			state->finish();

			delete state;
		}

		children_.clear();

		_Fini();
	}

	bool hsm::state::has_state(const size_t id) const
	{
		if (id == id_)
		{
			return true;
		}

		for (const auto child : children_)
		{
			if (child->has_state(id))
			{
				return true;
			}
		}

		return false;
	}

	hsm::state* hsm::state::get_state(const size_t id)
	{
		if (id == id_)
		{
			return this;
		}

		if (!has_state(id))
		{
			return nullptr;
		}

		for (auto child : children_)
		{
			//ASSERT_CRASH( child  );

			if (child->has_state(id))
			{
				return child->get_state(id);
			}
		}

		return nullptr;
	}

	bool hsm::state::get_transition_path_to(const size_t target, path& path)
	{
		if (has_state(target))
		{
			if (target == id_)
			{
				state::node n;

				n.state = this;
				n.code = state::node::TARGET;

				path.push_back(n);

				return true;
			}
			else
			{
				state::node n;

				n.state = this;
				n.code = state::node::NOOP;

				path.push_back(n);

				return _Get_child_path_to(target, path);
			}
		}
		else
		{
			if (parent_ != nullptr)
			{
				state::node n;

				n.state = this;
				n.code = state::node::EXIT;

				path.push_back(n);

				return parent_->get_transition_path_to(target, path);
			}
		}

		return false;
	}

	void hsm::state::_Attach(state* state)
	{
		//ASSERT_CRASH( state != 0 );

		if (has_state(state->get_id()))
		{
			return;
		}

		children_.push_back(state);
	}

	bool hsm::state::_Get_child_path_to(size_t target, path& path)
	{
		for (auto child : children_)
		{
			//ASSERT_CRASH( child != 0 );

			if (child->has_state(target))
			{
				return child->get_transition_path_to(target, path);
			}
		}

		//ASSERT_CRASH( !_T( "Should not reach here" ) );

		return false; // No child is in the path
	}

	bool hsm::state::_Init()
	{
		return true;
	}

	void hsm::state::_On_enter()
	{
	}

	hsm::state* hsm::state::_On_tick(uint32 age)
	{
		age;

		return nullptr;
	}

	bool hsm::state::is_transable(size_t target_state)
	{
		target_state;
		return true;
	}

	hsm_owner* hsm::state::get_owner()
	{
		//ASSERT_CRASH( m_hsm );

		return hsm_->get_owner();
	}


} // namespace mu2s
