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
		null,       //!< null
		town_hall,  //!< town_hall
		windmill,   //!< windmill
		house,      //!< house
		house_small,//!< house_small
		farmhouse,  //!< farmhouse
		blacksmith, //!< blacksmith
		stall       //!< stall
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
			std::shared_ptr<inventory> get_inventory();

		private:
			int x;
			int y;
			buildingtype type;
			std::shared_ptr<inventory> storage;
	};
}

#endif /* INCLUDE_BUILDING_H_ */
