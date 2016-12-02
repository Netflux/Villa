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
	building::building(double x, double y, buildingtype type) : entity(x, y, new inventory()), type(type)
	{
		switch(this->type)
		{
			case buildingtype::town_hall :
				this->width = 2;
				this->height = 3;
				break;

			case buildingtype::house :
				this->width = 2;
				this->height = 2;
				break;

			case buildingtype::house_small :
				this->width = 1;
				this->height = 1;
				break;

			case buildingtype::farmhouse :
				this->width = 2;
				this->height = 2;
				break;

			case buildingtype::blacksmith :
				this->width = 2;
				this->height = 2;
				break;

			case buildingtype::stall :
				this->width = 2;
				this->height = 1;
				break;

			default :
				this->width = 0;
				this->height = 0;
				break;
		}
	}

	/**
	 * Gets the type of the building.
	 * @return The building type.
	 */
	buildingtype building::get_type()
	{
		return this->type;
	}

	/**
	 * Sets the type of the building.
	 * @param value - The building type.
	 */
	void building::set_type(buildingtype value)
	{
		this->type = value;
	}

	/**
	 * Gets the width (grid) of the building.
	 * @return The building width.
	 */
	int building::get_width()
	{
		return this->width;
	}

	/**
	 * Gets the height (grid) of the building.
	 * @return The building height.
	 */
	int building::get_height()
	{
		return this->height;
	}
}
