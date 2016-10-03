#ifndef INCLUDE_INVENTORY_H_
#define INCLUDE_INVENTORY_H_

#include <item.h>
#include <tool.h>
#include <list>

namespace villa
{
	/**
	 * Inventory class.
	 * Represents a list of items stored by the parent entity.
	 */
	class inventory
	{
		public:
			inventory();
			int get_item_count();
			void add_item(item* value);
			void add_item(item* value, int quantity);
			void remove_item(item* value);
			void remove_item(item* value, int quantity);
			item* get_item(itemtype type);
			std::list<item*> get_items();
			void set_items(std::list<item*> items);
			tool* get_tool_highest_efficiency(itemtype type);

		private:
			std::list<item*> items;
	};
}

#endif /* INCLUDE_INVENTORY_H_ */
