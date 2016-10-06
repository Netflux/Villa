#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

#include <resource.h>
#include <tile.h>
#include <village.h>

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
			std::shared_ptr<tile> get_tile_at(int x, int y);
			void set_tile_at(int x, int y, std::shared_ptr<tile> value);
			std::shared_ptr<village> get_village();

		private:
			std::vector<std::shared_ptr<resource>> resources;
			std::shared_ptr<tile> tiles[10][10];
			std::shared_ptr<village> town;
	};
}

#endif /* INCLUDE_MAP_H_ */
