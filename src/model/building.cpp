#include "building.hpp"

namespace villa
{
	/**
	 * Constructor for the Building class.
	 */
	building::building() : building(0, 0, buildingtype::null, new inventory()) { }

	/**
	 * Constructor for the Building class.
	 * @param x - The x-coord of the building.
	 * @param y - The y-coord of the building.
	 * @param type - The building type.
	 * @param storage - The building inventory.
	 */
	building::building(int x, int y, buildingtype type, inventory* storage) : entity(x, y, storage), type(type) { }

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
}
