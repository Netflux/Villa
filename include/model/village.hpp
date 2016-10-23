#ifndef INCLUDE_VILLAGE_H_
#define INCLUDE_VILLAGE_H_

#include "building.hpp"
#include "villager.hpp"

namespace villa
{
	/**
	 * Village class.
	 * Represents a collection of villagers and buildings.
	 */
	class village
	{
		public:
			void add_villager(villager* value);
			void remove_villager(villager* value);
			void add_building(building* value);
			void remove_building(building* value);
			std::vector<std::shared_ptr<villager>> get_villagers();
			std::vector<std::shared_ptr<building>> get_buildings();

		private:
			std::vector<std::shared_ptr<villager>> villagers;
			std::vector<std::shared_ptr<building>> buildings;
	};
}

#endif /* INCLUDE_VILLAGE_H_ */
