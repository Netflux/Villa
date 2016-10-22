#include "ui_element.hpp"

namespace villa
{
	/**
	 * Constructor for the UI Element class.
	 * @param x - The x-coord of the element.
	 * @param y - The y-coord of the element.
	 * @param width - The width of the element.
	 * @param height - The height of the element.
	 * @param texture - The name of the texture.
	 */
	ui_element::ui_element(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) { }

	/**
	 * Checks whether the element is at the x and y coords.
	 * @param x - The x-coord to check.
	 * @param y - The y-coord to check.
	 * @return
	 */
	bool ui_element::is_at(int x, int y)
	{
		// If the x and y coords are within the boundaries of the element, return true
		if(x >= this->x && x <= (this->x + width) && y >= this->y && y <= (this->y + height))
		{
			return true;
		}

		return false;
	}
}
