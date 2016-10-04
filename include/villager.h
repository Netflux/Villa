#ifndef INCLUDE_VILLAGER_H_
#define INCLUDE_VILLAGER_H_

#include <inventory.h>
#include <memory>

namespace villa
{
	/**
	 * Villager class.
	 * Represents a character entity that can interact with the world.
	 */
	class villager
	{
		public:
			villager();
			virtual ~villager();
			void move();
			virtual void rest();
			virtual void harvest();
			int get_x();
			void set_x(int value);
			int get_y();
			void set_y(int value);

		private:
			int x;
			int y;
			int speed;
			int health;
			int hunger;
			int thirst;
			int fatigue;
			std::shared_ptr<inventory> storage;
	};
}

#endif /* INCLUDE_VILLAGER_H_ */
