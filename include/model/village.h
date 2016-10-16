#ifndef INCLUDE_VILLAGE_H_
#define INCLUDE_VILLAGE_H_

#include <building.h>
#include <villager.h>

namespace villa
{
	/**
	 * Village class.
	 * Represents a collection of villagers and buildings.
	 */
	class village
	{
		public:
			void add_villager(std::shared_ptr<villager> value);
			void remove_villager(std::shared_ptr<villager> value);
			void add_building(std::shared_ptr<building> value);
			void remove_building(std::shared_ptr<building> value);
			std::vector<std::shared_ptr<villager>> get_villagers();
			std::vector<std::shared_ptr<building>> get_buildings();

		private:
			std::vector<std::shared_ptr<villager>> villagers;
			std::vector<std::shared_ptr<building>> buildings;
	};
}

#endif /* INCLUDE_VILLAGE_H_ */
