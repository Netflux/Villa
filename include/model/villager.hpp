#ifndef INCLUDE_VILLAGER_H_
#define INCLUDE_VILLAGER_H_

#include <memory>
#include "entity.hpp"

namespace villa
{
	/**
	 * Villager class.
	 * Represents a character entity that can interact with the world.
	 */
	class villager : entity
	{
		public:
			villager();
			virtual ~villager();
			void move();
			virtual void rest();
			virtual void harvest();
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
	};
}

#endif /* INCLUDE_VILLAGER_H_ */
