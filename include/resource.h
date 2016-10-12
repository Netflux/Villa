#ifndef INCLUDE_RESOURCE_H_
#define INCLUDE_RESOURCE_H_

#include <entity.h>

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
	class resource : entity
	{
		public:
			resource(int x, int y, resourcetype type, bool harvestable);
			bool get_harvestable();
			void set_harvestable(bool value);

		private:
			resourcetype type;
			bool harvestable;
	};
}

#endif /* INCLUDE_RESOURCE_H_ */
