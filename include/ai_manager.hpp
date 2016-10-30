#ifndef INCLUDE_AI_MANAGER_HPP_
#define INCLUDE_AI_MANAGER_HPP_

#include "map.hpp"
#include <SDL.h>

namespace villa
{
	class ai_manager
	{
		public:
			ai_manager(map* simulation_map);
			void think();
			unsigned int get_time();
			void set_time(unsigned int time);

		private:
			map* simulation_map;
			unsigned int time;
	};
}

#endif /* INCLUDE_AI_MANAGER_HPP_ */
