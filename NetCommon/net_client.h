#pragma once
#include "net_common.h"
#include "net_connection.h"

namespace peanut{
	namespace net
	{
		template<typename T>
		class client_interface
		{
		public:
			//Constructor
			client_interface() :m_socket(m_context) {
				//Add things to do
			}
			
			//Destructor
			virtual ~client_interface() {
				Disconnect();
			}
	
			//Connect to server
			bool Connect(const std::string& host, const uint16_t port) {
	
				try {
					//Create connection
					m_connection = std::make_unique<connection<T>>(); //TODO
	
					//Resolve host into useable address
					asio::ip::tcp::resolver resolver(m_context);
					asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
	
					//Connect to Server
					m_connection->ConnectToServer(endpoints);
	
					//Start thread for context
					thrContext = std::thread([this]() {m_context.run(); });
				}
	
				catch (std::exception& e){
					std::cerr << "Client Exception: " << e.what() << std::endl;
					return false;
				}
	
				return false;
			}
	
			//Disconnect from server
			void Disconnect() {
			
				//Check if connection is active
				if (IsConnected()) {
					//disconnect
					m_connection->Disconnect();
				}
	
				//Stop connection
				m_context.stop();
				//stop thread
				if (thrContext.joinable())
					thrContext.join();
	
			}
	
			//Check if client is connected to server
			bool IsConnected() {
				if (m_connection)
					return m_connection->IsConnected();
				else
					return false;
			}
	
			//Retrieve messages from server
			tsqueue<owned_message<T>>& Incoming() {
				return m_qMessagesIn;
			}

			//Send message to server
			void Send(const message<T>& msg) {
				if (IsConnected) {
					m_connection->Send(msg);
				}
			}



		protected:
			//asio context for data transfer
			asio::io_context m_context;

			//Create thread for context
			std::thread thrContext;
		
			//Create Socket for connection
			asio::ip::tcp::socket m_socket;
		
			//Create client connection object
			std::unique_ptr<connection<T>> m_connection;
			
		private:
			//tsQueue for incoming messages
			tsqueue<owned_message<T>> m_qMessagesIn;
		
		
		};	
		
	}
}