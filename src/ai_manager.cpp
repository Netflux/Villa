#include "ai_manager.hpp"

namespace villa
{
	/**
	 * Constructor for the Villager AI class.
	 * @param simulation_map - The map of the simulation.
	 */
	ai_manager::ai_manager(map* simulation_map) : simulation_map(simulation_map), time(0) { }

	/**
	 * Executes the current task of each villager.
	 */
	void ai_manager::think()
	{
		std::vector<villager*> villagers = simulation_map->get_villagers();

		// Loop through each villager in the vector
		for(std::vector<villager*>::const_iterator iterator = villagers.begin(); iterator != villagers.end(); ++iterator)
		{
			task* current_task = (*iterator)->get_task();
			taskdata data = current_task->get_data();

			// If the villager is not within range to perform the target action,
			// move until close enough before performing the action
			// Ignore if the current task is to move or idle
			if(!(*iterator)->is_at(data.target_coords.first, data.target_coords.second) && current_task->get_type() != tasktype::move && current_task->get_type() != tasktype::idle)
			{
				(*iterator)->add_task(new task(tasktype::move, taskdata(std::make_pair(data.target_coords.first, data.target_coords.second))));
			}

			// Perform the appropriate action according to task type
			switch(current_task->get_type())
			{
				case tasktype::move :
					(*iterator)->move(data.target_coords.first, data.target_coords.second);

					if((*iterator)->is_at(data.target_coords.first, data.target_coords.second))
					{
						(*iterator)->remove_task();
					}
					break;

				case tasktype::harvest : // Take all the items from the target resource, then rest
					{
						inventory* inv = (*iterator)->get_inventory();
						inventory* target_inv = data.target_entity->get_inventory();

						for(std::vector<item*>::iterator it = target_inv->get_items().begin(); it != target_inv->get_items().end(); ++it)
						{
							inv->add_item(std::move(target_inv->take_item(*it)));
						}

						(*iterator)->add_task(new task(tasktype::rest, taskdata(std::make_pair((*iterator)->get_x(), (*iterator)->get_y()), SDL_GetTicks() + 5000)));
						break;
					}

				case tasktype::rest :
					if(SDL_GetTicks() > data.time)
					{
						(*iterator)->remove_task();
					}
					break;

				default:
					break;
			}
		}
	}

	/**
	 * Gets the last update time.
	 * @return The last update time.
	 */
	unsigned int ai_manager::get_time()
	{
		return time;
	}

	/**
	 * Sets the last update time.
	 * @param time - The last update time.
	 */
	void ai_manager::set_time(unsigned int time)
	{
		this->time = time;
	}
}
