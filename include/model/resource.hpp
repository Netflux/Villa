#ifndef INCLUDE_RESOURCE_H_
#define INCLUDE_RESOURCE_H_

#include "entity.hpp"

namespace villa
{
	/**
	 * Resource type enumeration.
	 */
	enum class resourcetype
	{
		water,//!< water
		food, //!< food
		tree, //!< tree
		stone,//!< stone
		ore   //!< ore
	};

	/**
	 * Resource class.
	 * Represents resource entities that appear on the map.
	 */
	class resource : public entity
	{
		public:
			resource(int x, int y, resourcetype type, inventory* storage, bool harvestable);
			resourcetype get_type();
			void set_type(resourcetype type);
			bool get_harvestable();
			void set_harvestable(bool value);
			unsigned int get_harvestable_time();
			void set_harvestable_time(unsigned int value);

		private:
			resourcetype type;
			bool harvestable;
			unsigned int harvestable_time;
	};
}

#endif /* INCLUDE_RESOURCE_H_ */
