#ifndef INCLUDE_BUILDING_H_
#define INCLUDE_BUILDING_H_

#include "entity.hpp"
#include "inventory.hpp"

namespace villa
{
	/**
	 * Building type enumeration.
	 */
	enum class buildingtype
	{
		null,       //!< null
		town_hall,  //!< town_hall
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
	class building : public entity
	{
		public:
			building();
			building(double x, double y, buildingtype type);
			buildingtype get_type();
			void set_type(buildingtype value);
			int get_width();
			int get_height();

		private:
			buildingtype type;
			int width;
			int height;
	};
}

#endif /* INCLUDE_BUILDING_H_ */
