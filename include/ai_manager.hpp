#ifndef INCLUDE_AI_MANAGER_HPP_
#define INCLUDE_AI_MANAGER_HPP_

#include <random>
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
		public:
			ai_manager(map* simulation_map, std::mt19937& rng);
			void think();
			std::vector<std::pair<int, int>> get_path(int x, int y, int target_x, int target_y);
			std::vector<std::pair<int, int>> get_path_resource(int x, int y);
			unsigned int get_time();
			void set_time(unsigned int time);

		private:
			map* simulation_map;
			std::mt19937& rng;
	};
}

#endif /* INCLUDE_AI_MANAGER_HPP_ */
