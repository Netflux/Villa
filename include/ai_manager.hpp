#ifndef INCLUDE_AI_MANAGER_HPP_
#define INCLUDE_AI_MANAGER_HPP_

#include <random>
#include <queue>
#include "map.hpp"
#include <SDL.h>

namespace villa
{
	/**
	 * AI Manager class.
	 * Handles all villager behaviour and decision-making.
	 */
	class ai_manager
	{
		template<typename T, typename priority_t>
		struct PriorityQueue {
			typedef std::pair<priority_t, T> PQElement;
			std::priority_queue<PQElement, std::vector<PQElement>,
										 std::greater<PQElement>> elements;

			inline bool empty() const { return elements.empty(); }

			inline void put(T item, priority_t priority) {
				elements.emplace(priority, item);
			}

			inline T get() {
				T best_item = elements.top().second;
				elements.pop();
				return best_item;
			}
		};

		public:
			ai_manager(map* simulation_map, std::mt19937& rng);
			void think();
			unsigned int get_time();
			void set_time(unsigned int time);

		private:
			map* simulation_map;
			std::mt19937& rng;
			std::vector<std::pair<int, int>> get_path(int x, int y, int target_x, int target_y);
			std::vector<std::pair<int, int>> get_path_closest_resource(int x, int y, resourcetype type);
	};
}

#endif /* INCLUDE_AI_MANAGER_HPP_ */
