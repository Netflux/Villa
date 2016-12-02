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
			virtual bool is_at(double x, double y);
			double get_x();
			void set_x(double value);
			double get_y();
			void set_y(double value);
			inventory* get_inventory();

		protected:
			entity();
			entity(double x, double y, inventory* storage);
			virtual ~entity();
			double x;
			double y;
			std::unique_ptr<inventory> storage;
	};
}

#endif /* INCLUDE_ENTITY_H_ */
