#pragma once
#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"

namespace net 
{
	template<typename T>
	class connection : public std::enable_shared_from_this<connection<T>>
	{
	public:
		connection() {}

		virtual ~connection() {}

	public:
		bool ConnectToServer();
		bool Disconnect();
		bool IsConnected();

	public:
		bool Send(const message<T>& msg);

	protected:
		//Each connection has a unique socket to a remote
		asio::ip::tcp::socket m_socket;

		//Share context with entire ASIO instance
		asio::io_context& m_asioContext;

		//Holds all messages going out
		tsqueue<message<T>> m_qMessagesOut;

		//Holds incoming messages, must provide reference
		tsqueue<owned_message<T>>& m_qMessagesIn;

	};

}