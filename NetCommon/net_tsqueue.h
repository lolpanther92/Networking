#pragma once
#include "net_common.h"

namespace peanut{
	namespace net
	{
		template <typename T>
		class tsqueue
		{
		public:
			tsqueue() = default;
			tsqueue(const tsqueue<T>&) = delete;
			virtual ~tsqueue() { clear(); }

		public:
			//Returns item in front of queue
			const T& front()
			{
				std::scoped_lock lock(muxQueue);
				return deqQueue.front();
			}

			//Returns item in back of queue
			const T& back()
			{
				std::scoped_lock lock(muxQueue);
				return deqQueue.back();
			}

			//Add item to front
			void push_front(const T& item) {
				std::scoped_lock lock(muxQueue);
				deqQueue.emplace_front(std::move(item));
			}

			//Add item to back
			void push_back(const T& item) {
				std::scoped_lock lock(muxQueue);
				deqQueue.emplace_back(std::move(item));
			}

			//Check if queue is empty
			bool empty() {
				std::scoped_lock lock(muxQueue);
				deqQueue.empty();
			}
			//Returns number of items in queue
			bool count() {
				std::scoped_lock lock(muxQueue);
				deqQueue.size();
			}
			//Clear all items in queue
			bool clear() {
				std::scoped_lock lock(muxQueue);
				deqQueue.clear();
			}

			//Removes and returns item from front
			T pop_front() {
				std::scoped_lock lock(muxQueue);
				auto t = std::move(deqQueue.front());
				deqQueue.pop_front();
				return t;
			}

			//Removes and returns item from back
			T pop_back() {
				std::scoped_lock lock(muxQueue);
				auto t = std::move(deqQueue.back());
				deqQueue.pop_back();
				return t;
			}

		protected:
			std::mutex muxQueue;
			std::deque<T> deqQueue;
		};

	}
}