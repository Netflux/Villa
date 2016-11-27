#include "inventory.hpp"

namespace villa
{
	/**
	 * Gets the number of items in the inventory.
	 * @return The number of items in the inventory.
	 */
	int inventory::get_item_count()
	{
		return this->items.size();
	}

	/**
	 * Gets the number of items in the inventory.
	 * @param type - The type of the item.
	 * @return The number of items in the inventory.
	 */
	int inventory::get_item_count(itemtype type)
	{
		int quantity = 0;

		// Loop through each item in the vector
		for(std::vector<std::unique_ptr<item>>::const_iterator iterator = this->items.begin(); iterator != this->items.end(); ++iterator)
		{
			if((*iterator)->get_type() == type)
			{
				quantity += 1;
			}
		}

		return quantity;
	}

	/**
	 * Adds the item to the inventory.
	 * @param value - The item to add.
	 */
	void inventory::add_item(item* value)
	{
		if(value != nullptr)
		{
			this->items.push_back(std::unique_ptr<item>(value));
		}
	}

	/**
	 * Adds the item to the inventory.
	 * @param value - The item to add.
	 */
	void inventory::add_item(std::unique_ptr<item> value)
	{
		if(value != nullptr)
		{
			this->items.push_back(std::move(value));
		}
	}

	/**
	 * Gets the specific item from the inventory.
	 * @param value - The item to take.
	 * @return The item (nullptr if not found).
	 */
	std::unique_ptr<item> inventory::take_item(item* value)
	{
		std::unique_ptr<item> target;

		// Loop through each item in the vector
		// If we've found the target item, remove it from the vector, stop the loop and return the value
		for(std::vector<std::unique_ptr<item>>::iterator iterator = this->items.begin(); iterator != this->items.end(); ++iterator)
		{
			if(iterator->get() == value)
			{
				target = std::move(*iterator);
				this->items.erase(iterator);
				break;
			}
		}

		return std::move(target);
	}

	/**
	 * Removes the item from the inventory.
	 * @param value - The item to remove.
	 */
	void inventory::remove_item(item* value)
	{
		// Loop through each item in the vector
		// If we've found the target item, remove it from the vector and stop the loop
		for(std::vector<std::unique_ptr<item>>::iterator iterator = this->items.begin(); iterator != this->items.end(); ++iterator)
		{
			if(iterator->get() == value)
			{
				this->items.erase(iterator);
				break;
			}
		}
	}

	/**
	 * Gets the first item of the given item type.
	 * @param type - The item type.
	 * @return The item (nullptr if not found).
	 */
	item* inventory::get_item(itemtype type)
	{
		// Loop through each item in the vector
		// If we've found the target item, return its pointer (null pointer if none exists)
		for(std::vector<std::unique_ptr<item>>::iterator iterator = this->items.begin(); iterator != this->items.end(); ++iterator)
		{
			if((*iterator)->get_type() == type)
			{
				return iterator->get();
			}
		}

		return nullptr;
	}

	/**
	 * Gets the vector of items.
	 * @return The item vector.
	 */
	std::vector<item*> inventory::get_items()
	{
		std::vector<item*> target;

		// Loop through each item in the vector, and pushes to a new vector
		// Returns the new list of pointers
		for(std::vector<std::unique_ptr<item>>::const_iterator iterator = this->items.begin(); iterator != this->items.end(); ++iterator)
		{
			target.push_back(iterator->get());
		}

		return target;
	}

	/**
	 * Gets the tool of the given type with the highest efficiency value.
	 * @param type - The tool type.
	 * @return The tool (nullptr if not found).
	 */
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

		tool* target = nullptr;
		int efficiency_highest = 0;

		// Loop through each item in the vector
		// Once we've found the highest efficiency tool, return its pointer (null pointer if none exists)
		for(std::vector<std::unique_ptr<item>>::iterator iterator = this->items.begin(); iterator != this->items.end(); ++iterator)
		{
			item* temp = iterator->get();

			// The pointer needs to be down-casted to the tool class since the vector stores the pointers as its base class type.
			// We already ensured that the item is a tool based on its itemtype in the switch.
			if(temp->get_type() == type && static_cast<tool*>(temp)->get_efficiency() > efficiency_highest)
			{
				efficiency_highest = static_cast<tool*>(temp)->get_efficiency();
				target = static_cast<tool*>(temp);
			}
		}

		return target;
	}
}
