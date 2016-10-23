#ifndef INCLUDE_INVENTORY_H_
#define INCLUDE_INVENTORY_H_

#include "item.hpp"
#include "tool.hpp"
#include <vector>
#include <memory>

namespace villa
{
	/**
	 * Inventory class.
	 * Represents a collection of items stored by the parent entity.
	 */
	class inventory
	{
		public:
			int get_item_count();
			void add_item(item* value);
			void add_item(item* value, int quantity);
			void remove_item(item* value);
			void remove_item(item* value, int quantity);
			item* get_item(itemtype type);
			std::vector<std::shared_ptr<item>> get_items();
			tool* get_tool_highest_efficiency(itemtype type);

		private:
			std::vector<std::shared_ptr<item>> items;
	};
}

#endif /* INCLUDE_INVENTORY_H_ */
