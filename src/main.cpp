#include <SDL.h>
#include <inventory.h>
#include <iostream>

using namespace villa;

int main(int argc, char *argv[])
{
	// Sample tests for inventory and items
	inventory* test = new inventory();

	std::shared_ptr<item> water(new item(itemtype::water));
	std::shared_ptr<item> food(new item(itemtype::food));
	std::shared_ptr<tool> pickaxe(new tool(itemtype::pickaxe, 50));
	std::shared_ptr<tool> axe(new tool(itemtype::axe, 25));

	test->add_item(water, 20);
	test->add_item(food, 5);
	test->add_item(pickaxe, 75);
	test->add_item(std::shared_ptr<tool>(new tool(itemtype::bucket, 79)), 3);
	test->add_item(std::shared_ptr<tool>(new tool(itemtype::bucket, 100)), 1);
	test->add_item(axe, 100);

	std::vector<std::shared_ptr<item>> items = test->get_items();

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

	std::cout << "Highest Efficiency Bucket : " << test->get_tool_highest_efficiency(itemtype::bucket)->get_efficiency() << std::endl;

	std::cout << "Exiting program..." << std::endl;
	return 0;
}
