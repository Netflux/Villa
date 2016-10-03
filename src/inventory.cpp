#include <inventory.h>

namespace villa
{
	/**
	 * Constructor for the Inventory class.
	 */
	inventory::inventory() : items({}) { }

	/**
	 * Gets the number of items in the inventory.
	 * @return The number of items in the inventory.
	 */
	int inventory::get_item_count()
	{
		return items.size();
	}

	/**
	 * Adds the item to the inventory.
	 * @param value - The item to add.
	 */
	void inventory::add_item(item* value)
	{
		items.push_front(value);
	}

	/**
	 * Adds the item to the inventory.
	 * @param value - The item to add.
	 * @param quantity - The number of items to add.
	 */
	void inventory::add_item(item* value, int quantity)
	{
		for(int i = 0; i < quantity; ++i)
		{
			items.push_front(value);
		}
	}

	/**
	 * Removes the item from the inventory.
	 * @param value - The item to remove.
	 */
	void inventory::remove_item(item* value)
	{
		item* target;

		for(std::list<item*>::iterator iterator = items.begin(); iterator != items.end(); ++iterator)
		{
			target = *iterator;

			if(target->get_type() == value->get_type())
			{
				items.erase(iterator);
				break;
			}
		}
	}

	/**
	 * Removes the item from the inventory.
	 * @param value - The item to remove.
	 * @param quantity - The number of items to remove.
	 */
	void inventory::remove_item(item* value, int quantity)
	{
		for(int i = 0; i < quantity; ++i)
		{
			this->remove_item(value);
		}
	}

	/**
	 * Gets the first item of the given item type.
	 * @param type - The item type.
	 * @return The item.
	 */
	item* inventory::get_item(itemtype type)
	{
		item* target = nullptr;

		for(std::list<item*>::iterator iterator = items.begin(); iterator != items.end(); ++iterator)
		{
			item* temp = *iterator;

			if(temp->get_type() == type)
			{
				target = temp;
				break;
			}
		}

		return target;
	}

	/**
	 * Gets the list of items.
	 * @return The item list.
	 */
	std::list<item*> inventory::get_items()
	{
		return items;
	}

	/**
	 * Sets the list of items.
	 * @param items - The item list.
	 */
	void inventory::set_items(std::list<item*> items)
	{
		this->items = items;
	}

	tool* inventory::get_tool_highest_efficiency(itemtype type)
	{
		// If the item is not a tool, return a null pointer
		switch(type)
		{
			case itemtype::null :
			case itemtype::water :
			case itemtype::food :
			case itemtype::lumber :
			case itemtype::stone :
			case itemtype::ore :
				return nullptr;
			default :
				break;
		}

		item* target = nullptr;
		int efficiency_highest = 0;

		for(std::list<item*>::iterator iterator = items.begin(); iterator != items.end(); ++iterator)
		{
			item* temp = *iterator;

			if(temp->get_type() == type && static_cast<tool*>(temp)->get_efficiency() > efficiency_highest)
			{
				efficiency_highest = static_cast<tool*>(temp)->get_efficiency();
				target = temp;
			}
		}

		return static_cast<tool*>(target);
	}
}
