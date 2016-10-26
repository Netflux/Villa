#include "building.hpp"

namespace villa
{
	/**
	 * Constructor for the Building class.
	 */
	building::building() : building(0, 0, 1, 1, buildingtype::null, new inventory()) { }

	/**
	 * Constructor for the Building class.
	 * @param x - The x-coord (grid) of the building.
	 * @param y - The y-coord (grid) of the building.
	 * @param width - The width (grid) of the building.
	 * @param height - The height (grid) of the building.
	 * @param type - The building type.
	 * @param storage - The building inventory.
	 */
	building::building(int x, int y, int width, int height, buildingtype type, inventory* storage) : entity(x, y, storage), type(type), width(width), height(height) { }

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
	 * Gets the width (grid) of the building.
	 * @return The building width.
	 */
	int building::get_width()
	{
		return width;
	}

	/**
	 * Gets the height (grid) of the building.
	 * @return The building height.
	 */
	int building::get_height()
	{
		return height;
	}
}
