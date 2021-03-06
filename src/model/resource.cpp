#include "resource.hpp"

namespace villa
{
	/**
	 * Constructor for the Resource class.
	 * @param x - The x-coords of the resource.
	 * @param y - The y-coords of the resource.
	 * @param type - The resource type.
	 */
	resource::resource(double x, double y, resourcetype type) : entity(x, y, new inventory()), type(type), harvestable(false), harvestable_time(1) { }

	/**
	 * Gets the type of the resource.
	 * @return - The resource type.
	 */
	resourcetype resource::get_type()
	{
		return this->type;
	}

	/**
	 * Sets the type of the resource.
	 * @param type - The resource type.
	 */
	void resource::set_type(resourcetype type)
	{
		this->type = type;
	}

	/**
	 * Gets whether the resource is harvestable.
	 * @return Boolean representing whether the resource is harvestable.
	 */
	bool resource::get_harvestable()
	{
		return this->harvestable;
	}

	/**
	 * Sets whether the resource is harvestable.
	 * @param value - Boolean representing whether the resource is harvestable.
	 */
	void resource::set_harvestable(bool value)
	{
		this->harvestable = value;
	}

	/**
	 * Gets the time when the resource should become harvestable.
	 * @return The time.
	 */
	unsigned int resource::get_harvestable_time()
	{
		return this->harvestable_time;
	}

	/**
	 * Sets the time when the resource should become harvestable.
	 * @param value - The time.
	 */
	void resource::set_harvestable_time(unsigned int value)
	{
		this->harvestable_time = value;
	}
}
