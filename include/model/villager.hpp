#ifndef INCLUDE_VILLAGER_H_
#define INCLUDE_VILLAGER_H_

#include <memory>
#include <stack>
#include "entity.hpp"
#include "resource.hpp"
#include "task.hpp"

namespace villa
{
	/**
	 * Villager class.
	 * Represents a character entity that can interact with the world.
	 */
	class villager : public entity
	{
		public:
			villager(double x, double y);
			void move(double x, double y, double value);
			void rest(double value);
			void harvest();
			void add_task(task* value);
			void remove_task();
			task* get_task();
			int get_task_count();
			int get_speed();
			void set_speed(int value);
			int get_health();
			void set_health(int value);
			int get_hunger();
			void set_hunger(int value);
			int get_thirst();
			void set_thirst(int value);
			int get_fatigue();
			void set_fatigue(int value);

		private:
			int speed;
			int health;
			int hunger;
			int thirst;
			int fatigue;
			std::stack<std::unique_ptr<task>> tasks;
	};
}

#endif /* INCLUDE_VILLAGER_H_ */
