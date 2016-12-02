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
	 * Checks whether the building is within range of the specified location.
	 * @param x - The x-coord of the target.
	 * @param y - The y-coord of the target.
	 * @return Boolean representing whether the building is within range of the specified location.
	 */
	bool building::is_at(double x, double y)
	{
		// If entity is within 8 pixels (1/2 tile) of the building, return true
		return x >= this->x - 8 && x <= (this->x + (this->width * 16) + 8) && y >= (this->y - (this->height * 16) - 8) && y <= this->y + 8;
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
