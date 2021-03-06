#include "villager.hpp"

namespace villa
{
	/**
	 * Constructor for the Villager class.
	 */
	villager::villager(double x, double y) : entity(x, y, new inventory()), speed(100), health(100), hunger(0), thirst(0), fatigue(0)
	{
		add_task(new task(tasktype::idle, taskdata(std::make_pair(x, y))));
	}

	/**
	 * Moves the villager toward the target coordinates.
	 * @param x - The x-coords of the target.
	 * @param y - The y-coords of the target.
	 * @param value - The time scale multiplier.
	 */
	void villager::move(double x, double y, double value)
	{
		double distance = (speed / 60) * value;

		if(this->x > x)
		{
			this->x -= distance;
		}
		else if(this->x < x)
		{
			this->x += distance;
		}

		if(this->y > y)
		{
			this->y -= distance;
		}
		else if(this->y < y)
		{
			this->y += distance;
		}
	}

	/**
	 * Rests for the target duration.
	 * @param value - The time scale multiplier.
	 */
	void villager::rest(double value)
	{
		taskdata data = get_task()->get_data();
		int target_x = data.target_coords.first, target_y = data.target_coords.second;
		double duration = data.time - ((1000 / 60) * value);

		remove_task();

		// If the task still has time left, re-add the task with the updated duration
		if(duration > 0)
		{
			add_task(new task(tasktype::rest, taskdata(std::make_pair(target_x, target_y), (int)duration)));
		}
	}

	/**
	 * Harvests the target resource.
	 */
	void villager::harvest()
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

		add_task(new task(tasktype::rest, taskdata(std::make_pair(x, y), pause_time)));
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
		if(value != nullptr)
		{
			this->tasks.push(std::unique_ptr<task>(value));
		}
	}

	/**
	 * Removes the current task of the villager.
	 */
	void villager::remove_task()
	{
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
