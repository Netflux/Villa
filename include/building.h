#ifndef INCLUDE_BUILDING_H_
#define INCLUDE_BUILDING_H_

#include <entity.h>
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
	class building : entity
	{
		public:
			building();
			building(int x, int y, buildingtype type);
			buildingtype get_type();
			void set_type(buildingtype value);
			std::shared_ptr<inventory> get_inventory();

		private:
			buildingtype type;
			std::shared_ptr<inventory> storage;
	};
}

#endif /* INCLUDE_BUILDING_H_ */
