#ifndef INCLUDE_BUILDING_H_
#define INCLUDE_BUILDING_H_

#include <inventory.h>

namespace villa
{
	/**
	 * Building type enumeration.
	 */
	enum class buildingtype
	{
		null,
		town_hall,
		windmill,
		house,
		house_small,
		farmhouse,
		blacksmith,
		stall
	};

	/**
	 * Building class.
	 * Represents a building entity that can be constructed by villagers.
	 */
	class building
	{
		public:
			building();
			building(int x, int y, buildingtype type);
			int get_x();
			void set_x(int value);
			int get_y();
			void set_y(int value);
			buildingtype get_type();
			void set_type(buildingtype value);

		private:
			int x;
			int y;
			buildingtype type;
			std::shared_ptr<inventory> storage;
	};
}

#endif /* INCLUDE_BUILDING_H_ */
