#pragma once

namespace sst::serialization
{
	template<typename _Ty>
	class singleton
	{
	public:
		static _Ty* get_instance();
		static void destroy();

		singleton(singleton const& ) = delete;
		singleton& operator=(singleton const& ) = delete;

	protected:
		static _Ty* instance_;

		singleton() { instance_ = static_cast <_Ty*> ( this ); }
		~singleton() {}
	};

	template<typename _Ty>
	_Ty* singleton<_Ty>::instance_ = nullptr;

	template<typename _Ty>
	_Ty* singleton<_Ty>::get_instance()
	{
		if ( !instance_)
		{
			instance_ = new _Ty();
		}

		return instance_;
	}

	template<typename _Ty>
	void singleton<_Ty>::destroy()
	{
		delete instance_;
		instance_ = nullptr;
	}

}

