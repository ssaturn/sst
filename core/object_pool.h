#pragma once

#include <concurrent_queue.h>


namespace sst::memory
{
	template<typename T>
	class async_object_pool
	{
	public:
		explicit async_object_pool(size_t size);
		~async_object_pool();

		T* aquire();
		void release(T* object);


	private:
		concurrency::concurrent_queue<T> _queue{};
	};

	template<typename T>
	async_object_pool<T>::async_object_pool(size_t size)
	{
		for (auto i = 0; i < size; ++i)
		{
			_queue.push(new T());
		}
	}

	template<typename T>
	async_object_pool<T>::~async_object_pool()
	{
		T* ptr{ nullptr };
		while (_queue.try_pop(ptr))
		{
			delete ptr;
			ptr = nullptr;
		}
	}

	template<typename T>
	T* async_object_pool<T>::aquire()
	{
		T* ptr{ nullptr };
		if (_queue.try_pop(ptr))
		{
			return ptr;
		}

		return new T();
	}

	template<typename T>
	void async_object_pool<T>::release(T* object)
	{
		_queue.push(object);
	}


}