#include "building.hpp"

namespace villa
{
	/**
	 * Constructor for the Building class.
	 */
	building::building() : building(0, 0, buildingtype::null) { }

	/**
	 * Constructor for the Building class.
	 * @param x - The x-coord (grid) of the building.
	 * @param y - The y-coord (grid) of the building.
	 * @param type - The building type.
	 */
	building::building(int x, int y, buildingtype type) : entity(x, y, new inventory()), type(type)
	{
		switch(type)
		{
			case buildingtype::town_hall :
				width = 2;
				height = 3;
				break;

			case buildingtype::house :
				width = 2;
				height = 2;
				break;

			case buildingtype::house_small :
				width = 1;
				height = 1;
				break;

			case buildingtype::farmhouse :
				width = 2;
				height = 2;
				break;

			case buildingtype::blacksmith :
				width = 2;
				height = 2;
				break;

			case buildingtype::stall :
				width = 2;
				height = 1;
				break;

			default :
				break;
		}
	}

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
