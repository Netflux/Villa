#include "map.hpp"

namespace villa
{
	/**
	 * Constructor for the Map class.
	 */
	map::map()
	{
		for(int i = 0; i < 50; ++i)
		{
			for(int j = 0; j < 50; ++j)
			{
				tiles[i][j].reset(new tile(tiletype::water));
			}
		}

		for(int i = 2; i < 48; ++i)
		{
			for(int j = 2; j < 48; ++j)
			{
				tiles[i][j].reset(new tile(tiletype::grass));
			}
		}

		for(int i = 5; i < 45; ++i)
		{
			for(int j = 5; j < 45; ++j)
			{
				tiles[i][j].reset(new tile(tiletype::sand));
			}
		}

		for(int i = 10; i < 40; ++i)
		{
			for(int j = 10; j < 40; ++j)
			{
				tiles[i][j].reset(new tile(tiletype::dirt));
			}
		}

		for(int i = 15; i < 35; ++i)
		{
			for(int j = 15; j < 35; ++j)
			{
				tiles[i][j].reset(new tile(tiletype::grass));
			}
		}

		for(int i = 20; i < 30; ++i)
		{
			for(int j = 20; j < 30; ++j)
			{
				tiles[i][j].reset(new tile(tiletype::water));
			}
		}
	}

	/**
	 * Adds the building to the village.
	 * @param value - The building to add.
	 */
	void map::add_building(building* value)
	{
		buildings.push_back(std::shared_ptr<building>(value));
	}

	/**
	 * Removes the building from the village.
	 * @param value - The building to remove.
	 */
	void map::remove_building(building* value)
	{
		// Loop through each building in the vector
		// If we've found the target building, remove it from the vector and stop the loop
		for(std::vector<std::shared_ptr<building>>::iterator iterator = buildings.begin(); iterator != buildings.end(); ++iterator)
		{
			if(iterator->get() == value)
			{
				buildings.erase(iterator);
				break;
			}
		}
	}

	/**
	 * Adds the villager to the village.
	 * @param value - The villager to add.
	 */
	void map::add_villager(villager* value)
	{
		villagers.push_back(std::shared_ptr<villager>(value));
	}

	/**
	 * Removes the villager from the village.
	 * @param value - The villager to remove.
	 */
	void map::remove_villager(villager* value)
	{
		// Loop through each villager in the vector
		// If we've found the target villager, remove it from the vector and stop the loop
		for(std::vector<std::shared_ptr<villager>>::iterator iterator = villagers.begin(); iterator != villagers.end(); ++iterator)
		{
			if(iterator->get() == value)
			{
				villagers.erase(iterator);
				break;
			}
		}
	}

	/**
	 * Gets the vector of buildings.
	 * @return The building vector.
	 */
	std::vector<std::shared_ptr<building>> map::get_buildings()
	{
		return buildings;
	}

	/**
	 * Gets the vector of resources.
	 * @return - The resource vector.
	 */
	std::vector<std::shared_ptr<resource>> map::get_resources()
	{
		return resources;
	}

	/**
	 * Gets the vector of villagers.
	 * @return The villager vector.
	 */
	std::vector<std::shared_ptr<villager>> map::get_villagers()
	{
		return villagers;
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
}
