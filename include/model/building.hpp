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
	class building : entity
	{
		public:
			building();
			building(int x, int y, buildingtype type, inventory* storage);
			buildingtype get_type();
			void set_type(buildingtype value);

		private:
			buildingtype type;
	};
}

#endif /* INCLUDE_BUILDING_H_ */
