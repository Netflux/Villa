#include "map.hpp"
#include <iostream>

namespace villa
{
	/**
	 * Constructor for the Map class.
	 */
	map::map(std::mt19937& rng) : rng(rng)
	{
		// Seed the Perlin Noise generator
		PerlinNoise pn(time(nullptr));

		// Reset the map to contain water tiles
		for(int i = 0; i < 50; ++i)
		{
			for(int j = 0; j < 50; ++j)
			{
				tiles[i][j].reset(new tile(tiletype::water, false, false));
			}
		}

		// Generate a new map based on the Perlin Noise values
		for(int i = 2; i < 48; ++i)
		{
			for(int j = 2; j < 48; ++j)
			{
				double x = (double)j/((double)48) / 8;
				double y = (double)i/((double)48) / 8;
				double n = pn.noise(10 * x, 10 * y, 0);

				if(n < 0.4)
				{
					tiles[i][j].reset(new tile(tiletype::water, false, false));
				}
				else if(n < 0.475)
				{
					tiles[i][j].reset(new tile(tiletype::sand, true, false));
				}
				else if(n < 0.6)
				{
					tiles[i][j].reset(new tile(tiletype::grass, true, false));
				}
				else
				{
					tiles[i][j].reset(new tile(tiletype::dirt, true, false));
				}
			}
		}

		// Scale down random number while preserving uniform distribution
		std::uniform_int_distribution<int> distribution_resource(40, 200);
		std::uniform_int_distribution<int> distribution_tiles(16, 784);
		int quantity = 0;

		while(quantity < distribution_resource(rng))
		{
			int x = distribution_tiles(rng);
			int y = distribution_tiles(rng);
			double n = pn.noise((double)x / (double)784, (double)y / (double)784, 0);

			if(tiles[x / 16][y / 16]->get_pathable() == true)
			{
				if(n < 0.4)
				{
					add_resource(new resource(x, y, resourcetype::food));
					quantity += 1;
				}
				else if(n < 0.525)
				{
					add_resource(new resource(x, y, resourcetype::tree));
					quantity += 1;
				}
				else if(n < 0.625)
				{
					add_resource(new resource(x, y, resourcetype::stone));
					quantity += 1;
				}
				else
				{
					add_resource(new resource(x, y, resourcetype::ore));
					quantity += 1;
				}
			}
		}

		// Add a water resource on each tile adjacent to a water tile
		for(int i = 0; i < 50; ++i)
		{
			for(int j = 0; j < 50; ++j)
			{
				if(tiles[i][j]->get_type() == tiletype::water)
				{
					if(i - 1 >= 0 && tiles[i - 1][j]->get_type() != tiletype::water)
					{
						add_resource(new resource(((i - 1) * 16) + 8, (j * 16) + 8, resourcetype::water));
					}

					if(i + 1 < 50 && tiles[i + 1][j]->get_type() != tiletype::water)
					{
						add_resource(new resource(((i + 1) * 16) + 8, (j * 16) + 8, resourcetype::water));
					}

					if(j - 1 >= 0 && tiles[i][j - 1]->get_type() != tiletype::water)
					{
						add_resource(new resource((i * 16) + 8, ((j - 1) * 16) + 8, resourcetype::water));
					}

					if(j + 1 < 50 && tiles[i][j + 1]->get_type() != tiletype::water)
					{
						add_resource(new resource((i * 16) + 8, ((j + 1) * 16) + 8, resourcetype::water));
					}
				}
			}
		}
	}

	/**
	 * Adds the building to the map.
	 * @param value - The building to add.
	 */
	void map::add_building(building* value)
	{
		buildings.push_back(std::unique_ptr<building>(value));
	}

	/**
	 * Removes the building from the map.
	 * @param value - The building to remove.
	 */
	void map::remove_building(building* value)
	{
		// Loop through each building in the vector
		// If we've found the target building, remove it from the vector and stop the loop
		for(std::vector<std::unique_ptr<building>>::iterator iterator = buildings.begin(); iterator != buildings.end(); ++iterator)
		{
			if(iterator->get() == value)
			{
				buildings.erase(iterator);
				break;
			}
		}
	}

	/**
	 * Adds the resource to the map.
	 * @param value - The resource to add.
	 */
	void map::add_resource(resource* value)
	{
		resources.push_back(std::unique_ptr<resource>(value));
	}

	/**
	 * Removes the resource from the map.
	 * @param value - The resource to remove.
	 */
	void map::remove_resource(resource* value)
	{
		// Loop through each resource in the vector
		// If we've found the target resource, remove it from the vector and stop the loop
		for(std::vector<std::unique_ptr<resource>>::iterator iterator = resources.begin(); iterator != resources.end(); ++iterator)
		{
			if(iterator->get() == value)
			{
				resources.erase(iterator);
				break;
			}
		}
	}

	/**
	 * Adds the villager to the map.
	 * @param value - The villager to add.
	 */
	void map::add_villager(villager* value)
	{
		villagers.push_back(std::unique_ptr<villager>(value));
	}

	/**
	 * Removes the villager from the map.
	 * @param value - The villager to remove.
	 */
	void map::remove_villager(villager* value)
	{
		// Loop through each villager in the vector
		// If we've found the target villager, remove it from the vector and stop the loop
		for(std::vector<std::unique_ptr<villager>>::iterator iterator = villagers.begin(); iterator != villagers.end(); ++iterator)
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
	std::vector<building*> map::get_buildings()
	{
		std::vector<building*> target;

		// Loop through each building in the vector, and pushes to a new vector
		// Returns the new list of pointers
		for(std::vector<std::unique_ptr<building>>::const_iterator iterator = buildings.begin(); iterator != buildings.end(); ++iterator)
		{
			target.push_back(iterator->get());
		}

		return target;
	}

	/**
	 * Gets the vector of resources.
	 * @return - The resource vector.
	 */
	std::vector<resource*> map::get_resources()
	{
		std::vector<resource*> target;

		// Loop through each item in the vector, and pushes to a new vector
		// Returns the new list of pointers
		for(std::vector<std::unique_ptr<resource>>::const_iterator iterator = resources.begin(); iterator != resources.end(); ++iterator)
		{
			target.push_back(iterator->get());
		}

		return target;
	}

	/**
	 * Gets the vector of villagers.
	 * @return The villager vector.
	 */
	std::vector<villager*> map::get_villagers()
	{
		std::vector<villager*> target;

		// Loop through each item in the vector, and pushes to a new vector
		// Returns the new list of pointers
		for(std::vector<std::unique_ptr<villager>>::const_iterator iterator = villagers.begin(); iterator != villagers.end(); ++iterator)
		{
			target.push_back(iterator->get());
		}

		return target;
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
	 * Gets the coordinates of the given tile.
	 * @param value - The tile to find.
	 * @return The coordinates of the tile (0, 0 if not found).
	 */
	std::pair<int, int> map::get_tile_coords(tile* value)
	{
		// Loops through each tile in the 2D array, and returns the tile if found
		for(int x = 0; x < 50; ++x)
		{
			for(int y = 0; y < 50; ++y)
			{
				if(tiles[x][y].get() == value)
				{
					return std::make_pair(x, y);
				}
			}
		}

		return std::make_pair(0, 0);
	}

	/**
	 * Gets the tiles that are neighbours to the target tile.
	 * @param value - The tile to check for neighbours.
	 * @return The neighbouring tiles.
	 */
	std::vector<tile*> map::get_neighbour_tiles(int x, int y)
	{
		std::vector<tile*> neighbors;

		if(y - 1 > 0 && tiles[x][y - 1]->get_pathable() == true) // North
		{
			neighbors.push_back(tiles[x][y - 1].get());
		}

		if(x - 1 > 0 && tiles[x - 1][y]->get_pathable() == true) // West
		{
			neighbors.push_back(tiles[x - 1][y].get());
		}

		if(x + 1 < 50 && tiles[x + 1][y]->get_pathable() == true) // East
		{
			neighbors.push_back(tiles[x + 1][y].get());
		}

		if(y + 1 < 50 && tiles[x][y + 1]->get_pathable() == true) // South
		{
			neighbors.push_back(tiles[x][y + 1].get());
		}

		if(x - 1 > 0 && y - 1 > 0 && tiles[x - 1][y - 1]->get_pathable() == true) // North-West
		{
			neighbors.push_back(tiles[x - 1][y - 1].get());
		}

		if(x + 1 < 50 && y - 1 > 0 && tiles[x + 1][y - 1]->get_pathable() == true) // North-East
		{
			neighbors.push_back(tiles[x + 1][y - 1].get());
		}

		if(x - 1 < 0 && y + 1 < 50 && tiles[x - 1][y + 1]->get_pathable() == true) // South-West
		{
			neighbors.push_back(tiles[x - 1][y + 1].get());
		}

		if(x + 1 < 50 && y + 1 < 50 && tiles[x + 1][y + 1]->get_pathable() == true) // South-East
		{
			neighbors.push_back(tiles[x + 1][y + 1].get());
		}

		return neighbors;
	}
}
