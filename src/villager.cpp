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

	/**
	 * Gets the speed of the villager.
	 * @return The villager speed.
	 */
	int villager::get_speed()
	{
		return speed;
	}

	/**
	 * Sets the speed of the villager.
	 * @param value - The villager speed.
	 */
	void villager::set_speed(int value)
	{
		speed = value;
	}

	/**
	 * Gets the health of the villager.
	 * @return The villager health.
	 */
	int villager::get_health()
	{
		return health;
	}

	/**
	 * Sets the health of the villager.
	 * @param value - The villager health.
	 */
	void villager::set_health(int value)
	{
		health = value;
	}

	/**
	 * Gets the hunger of the villager.
	 * @return The villager hunger.
	 */
	int villager::get_hunger()
	{
		return hunger;
	}

	/**
	 * Sets the hunger of the villager.
	 * @param value - The villager hunger.
	 */
	void villager::set_hunger(int value)
	{
		hunger = value;
	}

	/**
	 * Gets the thirst of the villager.
	 * @return The villager thirst.
	 */
	int villager::get_thirst()
	{
		return thirst;
	}

	/**
	 * Sets the thirst of the villager.
	 * @param value - The villager thirst.
	 */
	void villager::set_thirst(int value)
	{
		thirst = value;
	}

	/**
	 * Gets the fatigue of the villager.
	 * @return The villager fatigue.
	 */
	int villager::get_fatigue()
	{
		return fatigue;
	}

	/**
	 * Sets the fatigue of the villager.
	 * @param value - The villager fatigue.
	 */
	void villager::set_fatigue(int value)
	{
		fatigue = value;
	}

	/**
	 * Gets the inventory of the villager.
	 * @return The villager inventory.
	 */
	std::shared_ptr<inventory> villager::get_inventory()
	{
		return storage;
	}
}
