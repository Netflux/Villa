#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

#include "building.hpp"
#include "resource.hpp"
#include "tile.hpp"
#include "villager.hpp"

namespace villa
{
	/**
	 * Map class.
	 * Represents the simulation area which contains all entities within it.
	 */
	class map
	{
		public:
			map();
			void add_building(building* value);
			void remove_building(building* value);
			void add_villager(villager* value);
			void remove_villager(villager* value);
			std::vector<std::shared_ptr<building>> get_buildings();
			std::vector<std::shared_ptr<resource>> get_resources();
			std::vector<std::shared_ptr<villager>> get_villagers();
			tile* get_tile_at(int x, int y);

		private:
			std::vector<std::shared_ptr<building>> buildings;
			std::vector<std::shared_ptr<resource>> resources;
			std::unique_ptr<tile> tiles[50][50];
			std::vector<std::shared_ptr<villager>> villagers;
	};
}

#endif /* INCLUDE_MAP_H_ */
