#ifndef INCLUDE_AI_MANAGER_HPP_
#define INCLUDE_AI_MANAGER_HPP_

#include <random>
#include "map.hpp"
#include <SDL.h>

namespace villa
{
	class ai_manager
	{
		public:
			ai_manager(map* simulation_map, std::mt19937& rng);
			void think();
			unsigned int get_time();
			void set_time(unsigned int time);

		private:
			map* simulation_map;
			std::mt19937& rng;
	};
}

#endif /* INCLUDE_AI_MANAGER_HPP_ */
