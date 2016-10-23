#include "villager.hpp"

namespace villa
{
	/**
	 * Constructor for the Villager class.
	 */
	villager::villager() : entity(), speed(100), health(100), hunger(0), thirst(0), fatigue(0) { }

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
}
