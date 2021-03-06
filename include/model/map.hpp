#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

#include <random>
#include <time.h>
#include "PerlinNoise.h"
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
			map(std::mt19937& rng);
			bool add_building(building* value);
			void remove_building(building* value);
			void add_resource(resource* value);
			void remove_resource(resource* value);
			bool add_villager(villager* value);
			void remove_villager(villager* value);
			std::vector<building*> get_buildings();
			std::vector<resource*> get_resources();
			std::vector<villager*> get_villagers();
			int get_resource_count(resourcetype value);
			tile* get_tile_at(int x, int y);
			std::pair<int, int> get_tile_coords(tile* value);
			std::vector<tile*> get_neighbour_tiles(int x, int y);
			bool get_available_space(int x, int y, buildingtype value);

		private:
			std::vector<std::unique_ptr<building>> buildings;
			std::vector<std::unique_ptr<resource>> resources;
			std::unique_ptr<tile> tiles[50][50];
			std::vector<std::unique_ptr<villager>> villagers;
			std::mt19937& rng;
	};
}

#endif /* INCLUDE_MAP_H_ */
