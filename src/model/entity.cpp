#include <entity.h>

namespace villa
{
	/**
	 * Constructor for the Entity class.
	 */
	entity::entity() : entity(0, 0) { }

	/**
	 * Constructor for the Entity class.
	 * @param x - The x-coord of the entity.
	 * @param y - The y-coord of the entity.
	 */
	entity::entity(int x, int y) : x(x), y(y) { }

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
}
