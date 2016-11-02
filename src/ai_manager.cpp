#include "ai_manager.hpp"
#include "pathfinder.cpp"
#include <tuple>

namespace villa
{
	/**
	 * Constructor for the Villager AI class.
	 * @param simulation_map - The map of the simulation.
	 */
	ai_manager::ai_manager(map* simulation_map, std::mt19937& rng) : simulation_map(simulation_map), rng(rng) { }

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
				std::vector<std::pair<int, int>> path = get_path((*iterator)->get_x(), (*iterator)->get_y(), data.target_coords.first, data.target_coords.second);

				// Add a move task for each point towards the target location
				for(std::vector<std::pair<int, int>>::const_iterator it = path.begin(); it != path.end(); ++it)
				{
					(*iterator)->add_task(new task(tasktype::move, taskdata(std::make_pair((it->first * 16) + 8, (it->second * 16) + 8))));
				}
			}

			// Perform the appropriate action according to task type
			switch(current_task->get_type())
			{
				case tasktype::idle : // Roll for a chance to perform a new task
					break;

				case tasktype::move : // Move towards the target coordinates
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
						int rest_time = 0;

						for(std::vector<item*>::iterator it = target_inv->get_items().begin(); it != target_inv->get_items().end(); ++it)
						{
							inv->add_item(std::move(target_inv->take_item(*it)));
							rest_time += 2000;
						}

						(*iterator)->remove_task();
						(*iterator)->add_task(new task(tasktype::rest, taskdata(std::make_pair((*iterator)->get_x(), (*iterator)->get_y()), SDL_GetTicks() + rest_time)));
						break;
					}

				case tasktype::take_item : // Take the item from the target entity
					{
						inventory* inv = (*iterator)->get_inventory();
						inventory* target_inv = data.target_item.first->get_inventory();

						std::unique_ptr<item> target_item = target_inv->take_item(data.target_item.second);

						if(target_item.get() != nullptr)
						{
							inv->add_item(std::move(target_item));
						}

						(*iterator)->remove_task();
						(*iterator)->add_task(new task(tasktype::rest, taskdata(std::make_pair((*iterator)->get_x(), (*iterator)->get_y()), SDL_GetTicks() + 250)));
						break;
					}

				case tasktype::store_item : // Store the item into the target entity
					{
						inventory* inv = (*iterator)->get_inventory();
						inventory* target_inv = data.target_item.first->get_inventory();

						std::unique_ptr<item> target_item = inv->take_item(data.target_item.second);

						if(target_item.get() != nullptr)
						{
							target_inv->add_item(std::move(target_item));
						}

						(*iterator)->remove_task();
						(*iterator)->add_task(new task(tasktype::rest, taskdata(std::make_pair((*iterator)->get_x(), (*iterator)->get_y()), SDL_GetTicks() + 250)));
						break;
					}

				case tasktype::rest : // Wait until the set duration has passed
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
	 * Gets a path to the target x and y coords.
	 * @param x - The x-coord of the target.
	 * @param y - The y-coord of the target.
	 * @return Path to the target.
	 */
	std::vector<std::pair<int, int>> ai_manager::get_path(int x, int y, int target_x, int target_y)
	{
		GridWithWeights grid(50, 50);
		SquareGrid::Location start{x, y};
		SquareGrid::Location goal{target_x, target_y};
		std::unordered_map<SquareGrid::Location, SquareGrid::Location> came_from;
		std::unordered_map<SquareGrid::Location, double> cost_so_far;

		// Add walls to the grid for tiles that are not pathable
		for(int x = 0; x < 50; ++x)
		{
			for(int y = 0; y < 50; ++y)
			{
				tile* target = simulation_map->get_tile_at(x, y);

				if(target->get_pathable() == false)
				{
					grid.walls.insert(SquareGrid::Location { x, y });
				}
			}
		}

		a_star_search(grid, start, goal, came_from, cost_so_far);
		std::vector<SquareGrid::Location> path = reconstruct_path(start, goal, came_from);
		std::vector<std::pair<int, int>> target;

		// Loop through each coordinate in the vector
		for(std::vector<SquareGrid::Location>::const_iterator iterator = path.begin(); iterator != path.end(); ++iterator)
		{
			target.push_back(std::make_pair(std::get<0>(*iterator), std::get<1>(*iterator)));
		}

		return target;
	}
}
