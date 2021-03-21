#pragma once
#include "net_common.h"
#include "net_connection.h"

namespace net
{
	//Message header, template uses enum class to validate messages at compile time
	template <typename T>
	struct message_header
	{
		T id;
		uint32_t size = 0; //uint_32 instead of size_t for consistency between platforms
	};

	template <typename T>
	struct message
	{
		message_header<T> header{};
		std::vector<uint8_t> body;

		//Returns size of message
		size_t size() const {
			return sizeof(message_header<T>) + body.size();
		}

		//Overload for std::cout << to easily output message
		friend std::ostream& operator << (std::ostream& os, const message<T>& msg) {
			os << "ID: " << int(msg.header.id) << " Size: " << msg.header.size;
			return os;
		}

		//Push data into message buffer
		template <typename DataType>
		friend message<T>& operator << (message<T>& msg, const DataType& data) {

			//Check that type is standard and easy to insert
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into this vector");

			//Cache current size of body vector
			size_t i = msg.body.size();

			//Resize body vector according to new data
			msg.body.resize(msg.body.size() + sizeof(DataType));

			//Copy data into new space
			std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

			//Recalculate message size
			msg.header.size = msg.size();

			//Return target message
			return msg;
		}

		//Extract data from message
		template <typename DataType>
		friend message<T>& operator >> (message<T>& msg, DataType& data) {

			//Check that type is standard
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into this vector");

			//Cache location at end of vector
			size_t i = msg.body.size() - sizeof(DataType);

			//Copy data from vector into user variable
			std::memcpy(&data, msg.body.data() + 1, sizeof(DataType));

			//Shrink vector to remove read data
			msg.body.resize(i);

			//return target message
			return msg;
		}
	};


	template <typename T>
	struct owned_message
	{
		std::shared_ptr<connection<T>> remote = nullptr;
		message<T> msg;
		
		//Friendly output overload
		friend std::ostream& operator <<(std::ostream& os, const owned_message<T>& msg){
			os << msg.msg;
			return os;
		}

	};

}
