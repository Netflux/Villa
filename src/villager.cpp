#include <villager.h>

namespace villa
{
	/**
	 * Constructor for the Villager class.
	 */
	villager::villager() : x(0), y(0), speed(100), health(100), hunger(0), thirst(0), fatigue(0), storage(std::shared_ptr<inventory>(new inventory())) { }

	/**
	 * Destructor for the Villager class.
	 */
	villager::~villager() { }

	void villager::move()
	{

	}

	void villager::rest()
	{

	}

	void villager::harvest()
	{

	}

	/**
	 * Gets the x-coord of the villager.
	 * @return The x-coord of the villager.
	 */
	int villager::get_x()
	{
		return x;
	}

	/**
	 * Sets the x-coord of the villager.
	 * @param value - The x-coord of the villager.
	 */
	void villager::set_x(int value)
	{
		x = value;
	}

	/**
	 * Gets the y-coord of the villager.
	 * @return The y-coord of the villager.
	 */
	int villager::get_y()
	{
		return y;
	}

	/**
	 * Sets the y-coord of the villager.
	 * @param value - The y-coord of the villager.
	 */
	void villager::set_y(int value)
	{
		y = value;
	}
}
