#include "item.hpp"

namespace villa
{
	/**
	 * Constructor for the Item class.
	 */
	item::item() : type(itemtype::null) { }

	/**
	 * Constructor for the Item class.
	 * @param type - The item type.
	 */
	item::item(itemtype type) : type(type) { }

	/**
	 * Gets the type of the item.
	 * @return The item type.
	 */
	itemtype item::get_type()
	{
		return type;
	}

	/**
	 * Sets the type of the item.
	 * @param value - The item type.
	 */
	void item::set_type(itemtype value)
	{
		type = value;
	}
}
