#include "app.hpp"
#include "inventory.hpp"
#include <iostream>

using namespace villa;

int main(int argc, char *argv[])
{
	// Sample tests for inventory and items
	inventory* inv_test = new inventory();

	// Test adding items into the inventory
	for(int i = 0; i < 20; ++i) inv_test->add_item(new item(itemtype::water));
	for(int i = 0; i < 5; ++i) inv_test->add_item(new item(itemtype::food));
	for(int i = 0; i < 75; ++i) inv_test->add_item(new tool(itemtype::pickaxe, 50));
	for(int i = 0; i < 3; ++i) inv_test->add_item(new tool(itemtype::bucket, 79));
	for(int i = 0; i < 1; ++i) inv_test->add_item(new tool(itemtype::bucket, 100));
	for(int i = 0; i < 100; ++i) inv_test->add_item(new tool(itemtype::axe, 25));

	// Test removing items from the inventory
	item* remove = new item(itemtype::null);
	inv_test->add_item(remove);
	inv_test->remove_item(remove);

	std::vector<item*> items = inv_test->get_items();

	// Loop through items and print results
	for(std::vector<item*>::const_iterator iterator = items.begin(); iterator != items.end(); ++iterator)
	{
		item* temp = *iterator;

		if(temp->get_type() == itemtype::water)
		{
			std::cout << "Type : Water" << std::endl;
		}
		else if(temp->get_type() == itemtype::food)
		{
			std::cout << "Type : Food" << std::endl;
		}
		else if(temp->get_type() == itemtype::pickaxe)
		{
			std::cout << "Type : Pickaxe" << std::endl;
			std::cout << "Efficiency : " << static_cast<tool*>(temp)->get_efficiency() << std::endl;
		}
		else if(temp->get_type() == itemtype::axe)
		{
			std::cout << "Type : Axe" << std::endl;
			std::cout << "Efficiency : " << static_cast<tool*>(temp)->get_efficiency() << std::endl;
		}
		else if(temp->get_type() == itemtype::null)
		{
			std::cout << "Type : Null" << std::endl;
		}
	}

	// Test finding highest efficiency tool
	std::cout << "Highest Efficiency Bucket : " << inv_test->get_tool_highest_efficiency(itemtype::bucket)->get_efficiency() << std::endl;

	// Test initializing and starting application
	std::shared_ptr<app> application(new app());
	application->start();

	return 0;
}
