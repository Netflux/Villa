#include "villager.hpp"
#include <iostream>
namespace villa
{
	/**
	 * Constructor for the Villager class.
	 */
	villager::villager(int x, int y) : entity(x, y, new inventory()), speed(100), health(100), hunger(0), thirst(0), fatigue(0)
	{
		add_task(new task(tasktype::idle, taskdata(std::make_pair(x, y))));
	}

	/**
	 * Destructor for the Villager class.
	 */
	villager::~villager() { }

	void villager::move(int x, int y)
	{
		if(this->x > x)
		{
			this->x -= speed / 60;
		}
		else if(this->x < x)
		{
			this->x += speed / 60;
		}

		if(this->y > y)
		{
			this->y -= speed / 60;
		}
		else if(this->y < y)
		{
			this->y += speed / 60;
		}
	}

	/**
	 * Rests for the target duration.
	 * @param value - The current time.
	 */
	void villager::rest(unsigned int value)
	{
		taskdata data = get_task()->get_data();

		if(value > data.time)
		{
			remove_task();
		}
	}

	/**
	 * Harvests the target resource.
	 * @param value - The current time.
	 */
	void villager::harvest(unsigned int value)
	{
		taskdata data = get_task()->get_data();

		inventory* inv = get_inventory();
		inventory* target_inv = data.target_entity->get_inventory();

		tool* best_tool = nullptr;
		int pause_time = 1500;

		switch(static_cast<resource*>(data.target_entity)->get_type())
		{
			case resourcetype::water :
				best_tool = inv->get_tool_highest_efficiency(itemtype::bucket);
				break;

			case resourcetype::tree :
				best_tool = inv->get_tool_highest_efficiency(itemtype::axe);
				break;

			case resourcetype::stone :
			case resourcetype::ore :
				best_tool = inv->get_tool_highest_efficiency(itemtype::pickaxe);
				break;

			default :
				break;
		}

		if(best_tool != nullptr)
		{
			pause_time -= best_tool->get_efficiency() * 10;
		}

		add_task(new task(tasktype::rest, taskdata(std::make_pair(x, y), value + pause_time)));
		set_hunger(get_hunger() + 1);
		set_thirst(get_thirst() + 2);
		set_fatigue(get_fatigue() + 3);
		add_task(new task(tasktype::take_item, taskdata(std::make_pair(x, y), std::make_pair(data.target_entity, target_inv->get_items().back()))));
	}

	/**
	 * Adds a task to the villager.
	 * @param value - The task to add.
	 */
	void villager::add_task(task* value)
	{
		/*switch(value->get_type())
		{
			case tasktype::harvest :
				std::cout << "Current Task: Harvest" << std::endl;break;
			case tasktype::idle :
				std::cout << "Current Task: Idle" << std::endl;break;
			case tasktype::move :
				std::cout << "Current Task: Move" << std::endl;break;
			case tasktype::rest :
				std::cout << "Current Task: Rest" << std::endl;break;
			case tasktype::store_item :
				std::cout << "Current Task: Store Item" << std::endl;break;
			case tasktype::take_item :
				std::cout << "Current Task: Take Item" << std::endl;break;
			case tasktype::build :
				std::cout << "Current Task: Build" << std::endl;break;
		}*/
		if(value != nullptr)
		{
			this->tasks.push(std::unique_ptr<task>(value));
		}
		//std::cout << "Task Count: " << tasks.size() << " . Fatigue: " << fatigue << " . Thirst: " << thirst << " . Hunger: " << hunger << " . Inventory Count: " << storage->get_item_count() << std::endl;
	}

	/**
	 * Removes the current task of the villager.
	 */
	void villager::remove_task()
	{
		/*switch(tasks.top()->get_type())
		{
			case tasktype::harvest :
				std::cout << "Removing Task: Harvest" << std::endl;break;
			case tasktype::idle :
				std::cout << "Removing Task: Idle" << std::endl;break;
			case tasktype::move :
				std::cout << "Removing Task: Move" << std::endl;break;
			case tasktype::rest :
				std::cout << "Removing Task: Rest" << std::endl;break;
			case tasktype::store_item :
				std::cout << "Removing Task: Store Item" << std::endl;break;
			case tasktype::take_item :
				std::cout << "Removing Task: Take Item" << std::endl;break;
			case tasktype::build :
				std::cout << "Removing Task: Build" << std::endl;break;
		}*/
		this->tasks.pop();
	}

	/**
	 * Gets the current task of the villager.
	 * @return The current task.
	 */
	task* villager::get_task()
	{
		return this->tasks.top().get();
	}

	/**
	 * Gets the task count of the villager.
	 * @return The task count.
	 */
	int villager::get_task_count()
	{
		return this->tasks.size();
	}

	/**
	 * Gets the speed of the villager.
	 * @return The villager speed.
	 */
	int villager::get_speed()
	{
		return this->speed;
	}

	/**
	 * Sets the speed of the villager.
	 * @param value - The villager speed.
	 */
	void villager::set_speed(int value)
	{
		this->speed = value;
	}

	/**
	 * Gets the health of the villager.
	 * @return The villager health.
	 */
	int villager::get_health()
	{
		return this->health;
	}

	/**
	 * Sets the health of the villager.
	 * @param value - The villager health.
	 */
	void villager::set_health(int value)
	{
		this->health = value;
	}

	/**
	 * Gets the hunger of the villager.
	 * @return The villager hunger.
	 */
	int villager::get_hunger()
	{
		return this->hunger;
	}

	/**
	 * Sets the hunger of the villager.
	 * @param value - The villager hunger.
	 */
	void villager::set_hunger(int value)
	{
		this->hunger = value;
	}

	/**
	 * Gets the thirst of the villager.
	 * @return The villager thirst.
	 */
	int villager::get_thirst()
	{
		return this->thirst;
	}

	/**
	 * Sets the thirst of the villager.
	 * @param value - The villager thirst.
	 */
	void villager::set_thirst(int value)
	{
		this->thirst = value;
	}

	/**
	 * Gets the fatigue of the villager.
	 * @return The villager fatigue.
	 */
	int villager::get_fatigue()
	{
		return this->fatigue;
	}

	/**
	 * Sets the fatigue of the villager.
	 * @param value - The villager fatigue.
	 */
	void villager::set_fatigue(int value)
	{
		this->fatigue = value;
	}
}
