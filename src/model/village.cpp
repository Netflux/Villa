#include "village.hpp"

namespace villa
{
	/**
	 * Adds the villager to the village.
	 * @param value - The villager to add.
	 */
	void village::add_villager(std::shared_ptr<villager> value)
	{
		villagers.push_back(value);
	}

	/**
	 * Removes the villager from the village.
	 * @param value - The villager to remove.
	 */
	void village::remove_villager(std::shared_ptr<villager> value)
	{
		// Loop through each villager in the vector
		// If we've found the target villager, remove it from the vector and stop the loop
		for(std::vector<std::shared_ptr<villager>>::iterator iterator = villagers.begin(); iterator != villagers.end(); ++iterator)
		{
			if(*iterator == value)
			{
				villagers.erase(iterator);
				break;
			}
		}
	}

	/**
	 * Adds the building to the village.
	 * @param value - The building to add.
	 */
	void village::add_building(std::shared_ptr<building> value)
	{
		buildings.push_back(value);
	}

	/**
	 * Removes the building from the village.
	 * @param value - The building to remove.
	 */
	void village::remove_building(std::shared_ptr<building> value)
	{
		// Loop through each building in the vector
		// If we've found the target building, remove it from the vector and stop the loop
		for(std::vector<std::shared_ptr<building>>::iterator iterator = buildings.begin(); iterator != buildings.end(); ++iterator)
		{
			if(*iterator == value)
			{
				buildings.erase(iterator);
				break;
			}
		}
	}

	/**
	 * Gets the vector of villagers.
	 * @return The villager vector.
	 */
	std::vector<std::shared_ptr<villager>> village::get_villagers()
	{
		return villagers;
	}

	/**
	 * Gets the vector of buildings.
	 * @return The building vector.
	 */
	std::vector<std::shared_ptr<building>> village::get_buildings()
	{
		return buildings;
	}
}