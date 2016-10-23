#include "map.hpp"

namespace villa
{
	/**
	 * Constructor for the Map class.
	 */
	map::map() : town(new village()) { }

	/**
	 * Gets the vector of resources.
	 * @return - The resource vector.
	 */
	std::vector<std::shared_ptr<resource>> map::get_resources()
	{
		return resources;
	}

	/**
	 * Gets the tile at the given coordinates.
	 * @param x - The x-coord of the tile.
	 * @param y - The y-coord of the tile.
	 * @return The tile at the given coordinates.
	 */
	tile* map::get_tile_at(int x, int y)
	{
		return tiles[x][y].get();
	}

	/**
	 * Sets the tile at the given coordinates.
	 * @param x - The x-coord of the tile.
	 * @param y - The y-coord of the tile.
	 * @param value - The tile.
	 */
	void map::set_tile_at(int x, int y, tile* value)
	{
		tiles[x][y] = std::unique_ptr<tile>(value);
	}

	/**
	 * Gets the village.
	 * @return The village.
	 */
	village* map::get_village()
	{
		return town.get();
	}
}
