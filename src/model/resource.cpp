#include "resource.hpp"

namespace villa
{
	/**
	 * Constructor for the Resource class.
	 * @param type - The resource type.
	 * @param x - The x-coords of the resource.
	 * @param y - The y-coords of the resource.
	 * @param storage - The resource inventory.
	 * @param harvestable - Boolean representing whether the resource is harvestable.
	 */
	resource::resource(int x, int y, resourcetype type, inventory* storage, bool harvestable) : entity(x, y, storage), type(type), harvestable(harvestable) { }

	/**
	 * Gets whether the resource is harvestable.
	 * @return Boolean representing whether the resource is harvestable.
	 */
	bool resource::get_harvestable()
	{
		return harvestable;
	}

	/**
	 * Sets whether the resource is harvestable.
	 * @param value - Boolean representing whether the resource is harvestable.
	 */
	void resource::set_harvestable(bool value)
	{
		harvestable = value;
	}
}
