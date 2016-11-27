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
				this->tiles[i][j].reset(new tile(tiletype::water, false, false));
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
					this->tiles[i][j].reset(new tile(tiletype::water, false, false));
				}
				else if(n < 0.475)
				{
					this->tiles[i][j].reset(new tile(tiletype::sand, true, false));
				}
				else if(n < 0.6)
				{
					this->tiles[i][j].reset(new tile(tiletype::grass, true, false));
				}
				else
				{
					this->tiles[i][j].reset(new tile(tiletype::dirt, true, false));
				}
			}
		}

		// Scale down random number while preserving uniform distribution
		std::uniform_int_distribution<int> distribution_resource(40, 200);
		std::uniform_int_distribution<int> distribution_tiles(17, 783);
		int quantity = 0;

		while(quantity < distribution_resource(rng))
		{
			int x = distribution_tiles(rng);
			int y = distribution_tiles(rng);
			double n = pn.noise((double)x / (double)783, (double)y / (double)783, 0);

			if(this->tiles[x / 16][y / 16]->get_pathable() == true)
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
				if(this->tiles[i][j]->get_type() == tiletype::water)
				{
					if(i - 1 >= 0 && this->tiles[i - 1][j]->get_type() != tiletype::water)
					{
						add_resource(new resource(((i - 1) * 16) + 8, (j * 16) + 8, resourcetype::water));
					}

					if(i + 1 < 50 && this->tiles[i + 1][j]->get_type() != tiletype::water)
					{
						add_resource(new resource(((i + 1) * 16) + 8, (j * 16) + 8, resourcetype::water));
					}

					if(j - 1 >= 0 && this->tiles[i][j - 1]->get_type() != tiletype::water)
					{
						add_resource(new resource((i * 16) + 8, ((j - 1) * 16) + 8, resourcetype::water));
					}

					if(j + 1 < 50 && this->tiles[i][j + 1]->get_type() != tiletype::water)
					{
						add_resource(new resource((i * 16) + 8, ((j + 1) * 16) + 8, resourcetype::water));
					}
				}
			}
		}

		// Scale down random number while preserving uniform distribution
		std::uniform_int_distribution<int> distribution(17, 783);
		int i = distribution(rng), j = distribution(rng);

		// Place the town hall in a valid spot
		while(!add_building(new building(i, j, buildingtype::town_hall)))
		{
			i = distribution(rng);
			j = distribution(rng);
		}

		// Add 5 villagers to the map at the start
		for(int count = 0; count < 5; ++count)
		{
			i = distribution(rng);
			j = distribution(rng);

			while(!add_villager(new villager(i, j)))
			{
				i = distribution(rng);
				j = distribution(rng);
			}
		}
	}

	/**
	 * Adds the building to the map.
	 * @param value - The building to add.
	 */
	bool map::add_building(building* value)
	{
		bool result = false;

		if(value != nullptr)
		{
			result = get_available_space(value->get_x(), value->get_y(), value->get_type());

			if(result == true)
			{
				for(int i = value->get_x(); i <= (value->get_x() + (value->get_width() * 16)); i += 16)
				{
					for(int j = (value->get_y() - (value->get_height() * 16)); j <= value->get_y(); j += 16)
					{
						get_tile_at(i / 16, j / 16)->set_pathable(false);
					}
				}
				this->buildings.push_back(std::unique_ptr<building>(value));
			}
			else
			{
				delete value;
			}
		}

		return result;
	}

	/**
	 * Removes the building from the map.
	 * @param value - The building to remove.
	 */
	void map::remove_building(building* value)
	{
		// Loop through each building in the vector
		// If we've found the target building, remove it from the vector and stop the loop
		for(std::vector<std::unique_ptr<building>>::iterator iterator = this->buildings.begin(); iterator != this->buildings.end(); ++iterator)
		{
			if(iterator->get() == value)
			{
				this->buildings.erase(iterator);
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
		if(value != nullptr)
		{
			this->resources.push_back(std::unique_ptr<resource>(value));
		}
	}

	/**
	 * Removes the resource from the map.
	 * @param value - The resource to remove.
	 */
	void map::remove_resource(resource* value)
	{
		// Loop through each resource in the vector
		// If we've found the target resource, remove it from the vector and stop the loop
		for(std::vector<std::unique_ptr<resource>>::iterator iterator = this->resources.begin(); iterator != this->resources.end(); ++iterator)
		{
			if(iterator->get() == value)
			{
				this->resources.erase(iterator);
				break;
			}
		}
	}

	/**
	 * Adds the villager to the map.
	 * @param value - The villager to add.
	 */
	bool map::add_villager(villager* value)
	{
		bool result = false;

		if(value != nullptr && value->get_x() > 0 && value->get_x() <= 800 && value->get_y() > 0 && value->get_y() <= 800)
		{
			result = get_tile_at(value->get_x() / 16, value->get_y() / 16)->get_pathable();

			if(result == true)
			{
				this->villagers.push_back(std::unique_ptr<villager>(value));
			}
			else
			{
				delete value;
			}
		}

		return result;
	}

	/**
	 * Removes the villager from the map.
	 * @param value - The villager to remove.
	 */
	void map::remove_villager(villager* value)
	{
		// Loop through each villager in the vector
		// If we've found the target villager, remove it from the vector and stop the loop
		for(std::vector<std::unique_ptr<villager>>::iterator iterator = this->villagers.begin(); iterator != this->villagers.end(); ++iterator)
		{
			if(iterator->get() == value)
			{
				this->villagers.erase(iterator);
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
		for(std::vector<std::unique_ptr<building>>::const_iterator iterator = this->buildings.begin(); iterator != this->buildings.end(); ++iterator)
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
		for(std::vector<std::unique_ptr<resource>>::const_iterator iterator = this->resources.begin(); iterator != this->resources.end(); ++iterator)
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
		for(std::vector<std::unique_ptr<villager>>::const_iterator iterator = this->villagers.begin(); iterator != this->villagers.end(); ++iterator)
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
		if(x >= 0 && x < 50 && y >= 0 && y < 50)
		{
			return this->tiles[x][y].get();
		}

		return nullptr;
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
				if(this->tiles[x][y].get() == value)
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

		if(y - 1 > 0 && this->tiles[x][y - 1]->get_pathable() == true) // North
		{
			neighbors.push_back(this->tiles[x][y - 1].get());
		}

		if(x - 1 > 0 && this->tiles[x - 1][y]->get_pathable() == true) // West
		{
			neighbors.push_back(this->tiles[x - 1][y].get());
		}

		if(x + 1 < 50 && this->tiles[x + 1][y]->get_pathable() == true) // East
		{
			neighbors.push_back(this->tiles[x + 1][y].get());
		}

		if(y + 1 < 50 && this->tiles[x][y + 1]->get_pathable() == true) // South
		{
			neighbors.push_back(this->tiles[x][y + 1].get());
		}

		if(x - 1 > 0 && y - 1 > 0 && this->tiles[x - 1][y - 1]->get_pathable() == true) // North-West
		{
			neighbors.push_back(this->tiles[x - 1][y - 1].get());
		}

		if(x + 1 < 50 && y - 1 > 0 && this->tiles[x + 1][y - 1]->get_pathable() == true) // North-East
		{
			neighbors.push_back(this->tiles[x + 1][y - 1].get());
		}

		if(x - 1 < 0 && y + 1 < 50 && this->tiles[x - 1][y + 1]->get_pathable() == true) // South-West
		{
			neighbors.push_back(this->tiles[x - 1][y + 1].get());
		}

		if(x + 1 < 50 && y + 1 < 50 && this->tiles[x + 1][y + 1]->get_pathable() == true) // South-East
		{
			neighbors.push_back(this->tiles[x + 1][y + 1].get());
		}

		return neighbors;
	}

	/**
	 * Gets whether there is available space for the building type at a specific location
	 * @param value - The type of building.
	 * @return Boolean representing whether there is available space for the building type.
	 */
	bool map::get_available_space(int x, int y, buildingtype value)
	{
		int width = 0, height = 0;

		switch(value)
		{
			case buildingtype::town_hall :
				width = 2;
				height = 3;
				break;

			case buildingtype::house :
				width = 2;
				height = 2;
				break;

			case buildingtype::house_small :
				width = 1;
				height = 1;
				break;

			case buildingtype::farmhouse :
				width = 2;
				height = 2;
				break;

			case buildingtype::blacksmith :
				width = 2;
				height = 2;
				break;

			case buildingtype::stall :
				width = 2;
				height = 1;
				break;

			case buildingtype::null :
				return false;

			default :
				break;
		}

		bool result = true;

		for(int i = x; i <= (x + (width * 16)) && result == true; i += 16)
		{
			for(int j = (y - (height * 16)); j <= y && result == true; j += 16)
			{
				if(i < 0 || i > 800 || j < 0 || j > 800 || get_tile_at(i / 16, j / 16)->get_pathable() == false)
				{
					result = false;
				}
			}
		}

		return result;
	}
}
