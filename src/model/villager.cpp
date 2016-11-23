#include "villager.hpp"
#include <iostream>
namespace villa
{
	/**
	 * Constructor for the Villager class.
	 */
	villager::villager() : entity(), speed(100), health(100), hunger(0), thirst(0), fatigue(0)
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

	void villager::rest()
	{

	}

	void villager::harvest()
	{

	}

	/**
	 * Adds a task to the villager
	 * @param value - The task to add.
	 */
	void villager::add_task(task* value)
	{
		switch(value->get_type())
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
		}
		tasks.push(std::unique_ptr<task>(value));
		std::cout << "Task Count: " << tasks.size() << " . Fatigue: " << fatigue << " . Thirst: " << thirst << " . Hunger: " << hunger << std::endl;
	}

	/**
	 * Removes the current task of the villager.
	 */
	void villager::remove_task()
	{
		tasks.pop();
	}

	/**
	 * Gets the current task of the villager.
	 * @return The current task.
	 */
	task* villager::get_task()
	{
		return tasks.top().get();
	}

	/**
	 * Gets the speed of the villager.
	 * @return The villager speed.
	 */
	int villager::get_speed()
	{
		return speed;
	}

	/**
	 * Sets the speed of the villager.
	 * @param value - The villager speed.
	 */
	void villager::set_speed(int value)
	{
		speed = value;
	}

	/**
	 * Gets the health of the villager.
	 * @return The villager health.
	 */
	int villager::get_health()
	{
		return health;
	}

	/**
	 * Sets the health of the villager.
	 * @param value - The villager health.
	 */
	void villager::set_health(int value)
	{
		health = value;
	}

	/**
	 * Gets the hunger of the villager.
	 * @return The villager hunger.
	 */
	int villager::get_hunger()
	{
		return hunger;
	}

	/**
	 * Sets the hunger of the villager.
	 * @param value - The villager hunger.
	 */
	void villager::set_hunger(int value)
	{
		hunger = value;
	}

	/**
	 * Gets the thirst of the villager.
	 * @return The villager thirst.
	 */
	int villager::get_thirst()
	{
		return thirst;
	}

	/**
	 * Sets the thirst of the villager.
	 * @param value - The villager thirst.
	 */
	void villager::set_thirst(int value)
	{
		thirst = value;
	}

	/**
	 * Gets the fatigue of the villager.
	 * @return The villager fatigue.
	 */
	int villager::get_fatigue()
	{
		return fatigue;
	}

	/**
	 * Sets the fatigue of the villager.
	 * @param value - The villager fatigue.
	 */
	void villager::set_fatigue(int value)
	{
		fatigue = value;
	}
}
