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
	void inventory::add_item(std::shared_ptr<item> value)
	{
		items.push_back(value);
	}

	/**
	 * Adds the item to the inventory.
	 * @param value - The item to add.
	 * @param quantity - The number of items to add.
	 */
	void inventory::add_item(std::shared_ptr<item> value, int quantity)
	{
		for(int i = 0; i < quantity; ++i)
		{
			items.push_back(value);
		}
	}

	/**
	 * Removes the item from the inventory.
	 * @param value - The item to remove.
	 */
	void inventory::remove_item(std::shared_ptr<item> value)
	{
		for(std::vector<std::shared_ptr<item>>::iterator iterator = items.begin(); iterator != items.end(); ++iterator)
		{
			std::shared_ptr<item> target = *iterator;

			if(target == value)
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
	void inventory::remove_item(std::shared_ptr<item> value, int quantity)
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
	std::shared_ptr<item> inventory::get_item(itemtype type)
	{
		for(std::vector<std::shared_ptr<item>>::iterator iterator = items.begin(); iterator != items.end(); ++iterator)
		{
			std::shared_ptr<item> target = *iterator;

			if(target->get_type() == type)
			{
				return target;
			}
		}

		return nullptr;
	}

	/**
	 * Gets the vector of items.
	 * @return The item vector.
	 */
	std::vector<std::shared_ptr<item>> inventory::get_items()
	{
		return items;
	}

	/**
	 * Gets the tool of the given type with the highest efficiency value.
	 * @param type - The tool type.
	 * @return The tool.
	 */
	std::shared_ptr<tool> inventory::get_tool_highest_efficiency(itemtype type)
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

		std::shared_ptr<tool> target = nullptr;
		int efficiency_highest = 0;

		for(std::vector<std::shared_ptr<item>>::iterator iterator = items.begin(); iterator != items.end(); ++iterator)
		{
			std::shared_ptr<item> temp = *iterator;

			// The pointer needs to be down-casted to the tool class since the list stores the pointers as its base class type.
			// We already ensured that the item is a tool based on its itemtype in the switch.
			if(temp->get_type() == type && std::static_pointer_cast<tool>(temp)->get_efficiency() > efficiency_highest)
			{
				efficiency_highest = std::static_pointer_cast<tool>(temp)->get_efficiency();
				target = std::static_pointer_cast<tool>(temp);
			}
		}

		return target;
	}
}
