#include "entity.hpp"

namespace villa
{
	/**
	 * Constructor for the Entity class.
	 */
	entity::entity() : entity(0, 0, new inventory()) { }

	/**
	 * Constructor for the Entity class.
	 * @param x - The x-coord of the entity.
	 * @param y - The y-coord of the entity.
	 */
	entity::entity(int x, int y, inventory* storage) : x(x), y(y), storage(storage) { }

	/**
	 * Checks whether the entity is within range of the specified location.
	 * @param x - The x-coord of the target.
	 * @param y - The y-coord of the target.
	 * @return Boolean representing whether the entity is within range of the specified location.
	 */
	bool entity::is_at(int x, int y)
	{
		// If entity is within 6 pixels of the target, return true
		return abs(this->x - x) <= 6 && abs(this->y - y) <= 6;
	}

	/**
	 * Gets the x-coord of the entity.
	 * @return The x-coord of the entity.
	 */
	int entity::get_x()
	{
		return x;
	}

	/**
	 * Sets the x-coord of the entity.
	 * @param value - The x-coord of the entity.
	 */
	void entity::set_x(int value)
	{
		x = value;
	}

	/**
	 * Gets the y-coord of the entity.
	 * @return The y-coord of the entity.
	 */
	int entity::get_y()
	{
		return y;
	}

	/**
	 * Sets the y-coord of the entity.
	 * @param value - The y-coord of the entity.
	 */
	void entity::set_y(int value)
	{
		y = value;
	}

	/**
	 * Gets the inventory of the entity.
	 * @return The entity inventory.
	 */
	inventory* entity::get_inventory()
	{
		return storage.get();
	}
}
