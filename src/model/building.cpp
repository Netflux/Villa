#include <building.h>

namespace villa
{
	/**
	 * Constructor for the Building class.
	 */
	building::building() : building(0, 0, buildingtype::null) { }

	/**
	 * Constructor for the Building class.
	 * @param x - The x-coord of the building.
	 * @param y - The y-coord of the building.
	 * @param type - The building type.
	 */
	building::building(int x, int y, buildingtype type) : entity(x, y), type(type), storage(std::shared_ptr<inventory>(new inventory())) { }

	/**
	 * Gets the type of the building.
	 * @return The building type.
	 */
	buildingtype building::get_type()
	{
		return type;
	}

	/**
	 * Sets the type of the building.
	 * @param value - The building type.
	 */
	void building::set_type(buildingtype value)
	{
		type = value;
	}

	/**
	 * Gets the inventory of the building.
	 * @return The building inventory.
	 */
	std::shared_ptr<inventory> building::get_inventory()
	{
		return storage;
	}
}
