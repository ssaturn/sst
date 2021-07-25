#pragma once

#include "packet_listener.h"
#include "common.h"
#include "packet_header.h"


namespace sst
{
	class packet_listener;
	class session;

	class callback
	{
	public:
		virtual ~callback() = default;
		virtual size_t proc( void* session, const char* pTr, const uint16 pTrSz ) const = 0;
	};


	class listener_callback : public callback
	{
	public:
		explicit listener_callback( packet_listener* listener )
			: listener_( listener )
		{

		}
		~listener_callback() override = default;

		size_t proc( void* session, const char* pTr, const uint16 pTrSz ) const override
		{
			session;
			listener_->notify( pTr, pTrSz );

			return 0;
		}

	private:
		packet_listener* listener_{ nullptr };
	}; 

	// 패킷을 위한 글로벌 함수를 연결
	typedef bool( *packet_handler_function )( const void*, const packet_header* header, const char*, const uint16);

	class global_func_callback : public callback
	{
	public:
		size_t proc( void* session, const char* pTr, const uint16 pTrSz ) const override
		{
			const auto header = reinterpret_cast<const packet_header*>( pTr );

			func_( session, header, pTr + sizeof(packet_header), pTrSz - sizeof(packet_header) );

			return 0;
		}

	private:
		packet_handler_function func_{};
	};

}
