#ifndef INCLUDE_ENTITY_H_
#define INCLUDE_ENTITY_H_

#include <memory>
#include "inventory.hpp"

namespace villa
{
	/**
	 * Entity class.
	 * Represents all entities that physically appear in the simulation.
	 */
	class entity
	{
		public:
			bool is_at(int x, int y);
			int get_x();
			void set_x(int value);
			int get_y();
			void set_y(int value);
			inventory* get_inventory();

		protected:
			entity();
			entity(int x, int y, inventory* storage);
			double x;
			double y;
			std::unique_ptr<inventory> storage;
	};
}

#endif /* INCLUDE_ENTITY_H_ */
