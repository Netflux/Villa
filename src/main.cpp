#include "app.hpp"
#include "inventory.hpp"
#include <iostream>

using namespace villa;

int main(int argc, char *argv[])
{
	// Sample tests for inventory and items
	inventory* inv_test = new inventory();

	// Test creating items
	item* water = new item(itemtype::water);
	item* food = new item(itemtype::food);
	tool* pickaxe = new tool(itemtype::pickaxe, 50);
	tool* axe = new tool(itemtype::axe, 25);

	// Test adding items into the inventory
	inv_test->add_item(water, 20);
	inv_test->add_item(food, 5);
	inv_test->add_item(pickaxe, 75);
	inv_test->add_item(new tool(itemtype::bucket, 79), 3);
	inv_test->add_item(new tool(itemtype::bucket, 100), 1);
	inv_test->add_item(axe, 100);

	// Test removing items from the inventory
	inv_test->remove_item(pickaxe, 71);
	inv_test->remove_item(inv_test->get_item(itemtype::axe), 98);

	std::vector<std::shared_ptr<item>> items = inv_test->get_items();

	// Loop through items and print results
	for(std::vector<std::shared_ptr<item>>::iterator iterator = items.begin(); iterator != items.end(); ++iterator)
	{
		std::shared_ptr<item> temp = *iterator;

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
			std::cout << "Efficiency : " << std::static_pointer_cast<tool>(temp)->get_efficiency() << std::endl;
		}
		else if(temp->get_type() == itemtype::axe)
		{
			std::cout << "Type : Axe" << std::endl;
			std::cout << "Efficiency : " << std::static_pointer_cast<tool>(temp)->get_efficiency() << std::endl;
		}
	}

	// Test finding highest efficiency tool
	std::cout << "Highest Efficiency Bucket : " << inv_test->get_tool_highest_efficiency(itemtype::bucket)->get_efficiency() << std::endl;

	// Test initializing and starting application
	std::shared_ptr<app> application(new app());
	application->start();

	return 0;
}
