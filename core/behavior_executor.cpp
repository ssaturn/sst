#include "stdafx.h"
#include "behavior_executor.h"


namespace sst
{
	behavior_executor::behavior_executor(behavior_owner* owner)
		: owner_(owner)
	{
	}

	behavior_executor::~behavior_executor()
	{
	}

	bool behavior_executor::start(behavior* behavior)
	{
		if (owner_ == nullptr || behavior == nullptr)
		{
			return false;
		}

		if (true == behavior->done())
		{
			return false;
		}

		if (current_)
		{
			return false;
		}

		current_ = behavior;
		current_->OnBegin();

		return true;
	}

	void behavior_executor::add_pend(behavior* behavior)
	{
		pending_.push_back(behavior);
	}

	bool behavior_executor::cancel(const behavior_end_reason reason)
	{
		// �������� �̾Ƽ� �ؾ��ϳ�? front�� �ؼ� ã�Ƽ� �غ���?
		// �����ɶ� ����Ǹ� pop�ϵ��� �ϴ°� ���?
		if (current_)
		{
			current_->OnEnd(reason);

			delete current_;
			current_ = nullptr;
		}

		_Start_pending();

		return true;
	}

	void behavior_executor::end(const behavior_end_reason reason)
	{
		_Do_end(reason);
	}

	void behavior_executor::update()
	{
		if (owner_ == nullptr || current_ == nullptr)
		{
			return;
		}

		if (current_->done())
		{
			_Do_end(current_->end_reason());
		}
		else
		{
			current_->OnProcess();
		}
	}
	// ���� �ൿ����
	void behavior_executor::_Do_end(behavior_end_reason reason)
	{
		if (current_ == nullptr)
		{
			return;
		}

		current_->OnEnd(reason);

		auto pendedBehaviors = current_->get_pending();
		if (pendedBehaviors.empty() == false)
		{
			pending_.insert(pending_.begin(), pendedBehaviors.begin(), pendedBehaviors.end());
		}

		delete current_;
		current_ = nullptr;

		// ���ϵ� ���� �ٽ� ����
		_Start_pending();
	}

	bool behavior_executor::doing_something() const
	{
		return current_ != nullptr;
	}

	bool behavior_executor::doing(behavior_type type) const
	{
		if (current_ != nullptr)
		{
			return (current_->type() == type);
		}

		return false;
	}

	void behavior_executor::_Start_pending()
	{
		if (doing_something())
		{
			return;
		}

		if (pending_.empty())
		{
			return;
		}

		behavior* behavior = pending_.front();
		pending_.pop_front();

		start(behavior);
	}

	void behavior_executor::clear()
	{
		while (!pending_.empty())
		{
			if (behavior * behavior = pending_.front(); behavior)
			{
				delete behavior;
			}
			pending_.pop_front();
		}

		pending_.clear();
	}


}