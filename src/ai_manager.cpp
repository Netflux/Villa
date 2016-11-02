#include "ai_manager.hpp"
#include <iostream>

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

				// Check if there is a valid path to the target
				if(!path.empty())
				{
					// Add a move task for each point towards the target location
					for(std::vector<std::pair<int, int>>::const_iterator it = path.begin(); it != path.end(); ++it)
					{
						(*iterator)->add_task(new task(tasktype::move, taskdata(std::make_pair((it->first * 16) + 8, (it->second * 16) + 8))));
					}
				}
				// If there is no valid path to the target, assume the task is invalid and remove it
				else
				{
					(*iterator)->remove_task();
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
		tile* start = simulation_map->get_tile_at(x / 16, y / 16);
		tile* goal = simulation_map->get_tile_at(target_x / 16, target_y / 16);
		std::unordered_map<tile*, tile*> came_from;
		std::unordered_map<tile*, double> cost_so_far;
		PriorityQueue<tile*, double> frontier;

		frontier.put(start, 0);
		came_from[start] = start;
		cost_so_far[start] = 0;

		// Keep searching until the the goal is found, or the entire map has been checked
		while(!frontier.empty())
		{
			tile* current = frontier.get();

			if(current == goal)
			{
				break;
			}

			std::pair<int, int> current_coords = simulation_map->get_tile_coords(current);
			std::vector<tile*> neighbours = simulation_map->get_neighbour_tiles(current_coords.first, current_coords.second);

			// Check each neighbouring tile and calculate the movement cost
			for(std::vector<tile*>::const_iterator it = neighbours.begin(); it != neighbours.end(); ++it )
			{
				// The additional movement cost is lower if there's a road (1 vs 5)
				// Additional movement cost is added for diagonal movement (1 vs 1.414)
				// We use 1.414 (square root of two) as it represent the actual diagonal distance covered
				double new_cost = (cost_so_far[current] + ((*it)->get_has_road() ? 1 : 5)) + (it - neighbours.begin() < 4 ? 1 : 1.414);
				if(!cost_so_far.count(*it) || new_cost < cost_so_far[*it])
				{
					cost_so_far[*it] = new_cost;
					double priority = new_cost + abs((x / 16) - (target_x / 16)) + abs((y / 16) - (target_y / 16));
					frontier.put(*it, priority);
					came_from[*it] = current;
				}
			}
		}

		std::vector<std::pair<int, int>> path;
		path.push_back(std::make_pair(target_x / 16, target_y / 16));
		tile* current = goal;
		int count = 0;

		// Get the path from the start to the goal by checking backwards from the goal tile
		// The count is used to ensure that the pathfinder will stop if a path cannot be found
		while(current != start && count < 200)
		{
			std::cout << "X: " << simulation_map->get_tile_coords(current).first << " Y: " << simulation_map->get_tile_coords(current).second << std::endl;
			current = came_from[current];
			path.push_back(simulation_map->get_tile_coords(current));
			++count;
		}

		// If a valid path was found within 200 checks, return it. Otherwise, return an empty vector
		return count < 200 ? path : std::vector<std::pair<int, int>>();
	}
}
