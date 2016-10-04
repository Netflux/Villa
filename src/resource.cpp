#include <resource.h>

namespace villa
{
	/**
	 * Constructor for the Resource class.
	 * @param type - The resource type.
	 * @param x - The x-coords of the resource.
	 * @param y - The y-coords of the resource.
	 */
	resource::resource(int x, int y, resourcetype type) : x(x), y(y), type(type), harvestable(true) { }

	/**
	 * Gets the x-coord of the resource.
	 * @return The x-coord of the resource.
	 */
	int resource::get_x()
	{
		return x;
	}

	/**
	 * Sets the x-coord of the resource.
	 * @param value - The x-coord of the resource.
	 */
	void resource::set_x(int value)
	{
		x = value;
	}

	/**
	 * Gets the y-coord of the resource.
	 * @return The y-coord of the resource.
	 */
	int resource::get_y()
	{
		return y;
	}

	/**
	 * Sets the y-coord of the resource.
	 * @param value - The y-coord of the resource.
	 */
	void resource::set_y(int value)
	{
		y = value;
	}

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
