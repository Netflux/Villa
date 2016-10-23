#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

#include "resource.hpp"
#include "tile.hpp"
#include "village.hpp"

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
			std::vector<std::shared_ptr<resource>> get_resources();
			tile* get_tile_at(int x, int y);
			void set_tile_at(int x, int y, tile* value);
			village* get_village();

		private:
			std::vector<std::shared_ptr<resource>> resources;
			std::unique_ptr<tile> tiles[10][10];
			std::unique_ptr<village> town;
	};
}

#endif /* INCLUDE_MAP_H_ */
