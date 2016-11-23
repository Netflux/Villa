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
				std::cout << "Target X: " << data.target_coords.first << ". Target Y: " << data.target_coords.second << std::endl;
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
				case tasktype::idle : // Check current status and perform a new task
					//std::cout << "Task: Idle." << std::endl;
					if((*iterator)->get_fatigue() >= 75)
					{
						// Rest to reduce fatigue
						(*iterator)->add_task(new task(tasktype::rest, taskdata(std::make_pair((*iterator)->get_x(), (*iterator)->get_y()), SDL_GetTicks() + 10000)));
						(*iterator)->set_fatigue((*iterator)->get_fatigue() - 8);
					}
					else if((*iterator)->get_hunger() >= 75)
					{
						// Look for some food to consume
						item* food = (*iterator)->get_inventory()->get_item(itemtype::food);

						if(food != nullptr)
						{
							(*iterator)->get_inventory()->remove_item(food);
							(*iterator)->set_hunger((*iterator)->get_hunger() - 3);
						}
						else
						{
							int found_item = 0;
							std::vector<building*> buildings = simulation_map->get_buildings();

							// Loop through each building in the vector for a food item
							for(std::vector<building*>::const_iterator it = buildings.begin(); it != buildings.end(); ++it)
							{
								item* target = (*it)->get_inventory()->get_item(itemtype::food);

								if(target != nullptr)
								{
									(*iterator)->add_task(new task(tasktype::take_item, taskdata(std::make_pair(((*it)->get_x() * 16) + 8, (*it)->get_y() * 16), std::make_pair(*it, target))));
									found_item = 1;
									break;
								}
							}

							// If no buildings contain food, look for a food resource
							if(found_item == 0)
							{
								std::vector<resource*> resources = simulation_map->get_resources();

								// Loop through each building in the vector
								for(std::vector<resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
								{
									if((*it)->get_type() == resourcetype::food && (*it)->get_harvestable() == true)
									{
										(*iterator)->add_task(new task(tasktype::harvest, taskdata(std::make_pair((*it)->get_x(), (*it)->get_y()), *it)));
										found_item = 1;
										break;
									}
								}
							}

							// If no resources contain food, rest for a few seconds
							if(found_item == 0)
							{
								(*iterator)->add_task(new task(tasktype::rest, taskdata(std::make_pair((*iterator)->get_x(), (*iterator)->get_y()), SDL_GetTicks() + 2500)));
								(*iterator)->set_fatigue((*iterator)->get_fatigue() - 2);
							}
						}
					}
					else if((*iterator)->get_thirst() >= 75)
					{
						// Look for some water to consume
						item* water = (*iterator)->get_inventory()->get_item(itemtype::water);

						if(water != nullptr)
						{
							(*iterator)->get_inventory()->remove_item(water);
							(*iterator)->set_thirst((*iterator)->get_thirst() - 3);
						}
						else
						{
							int found_item = 0;
							std::vector<building*> buildings = simulation_map->get_buildings();

							// Loop through each building in the vector for a water item
							for(std::vector<building*>::const_iterator it = buildings.begin(); it != buildings.end(); ++it)
							{
								item* target = (*it)->get_inventory()->get_item(itemtype::water);

								if(target != nullptr)
								{
									(*iterator)->add_task(new task(tasktype::take_item, taskdata(std::make_pair(((*it)->get_x() * 16) + 8, (*it)->get_y() * 16), std::make_pair(*it, target))));
									found_item = 1;
									break;
								}
							}

							// If no buildings contain water, look for a water resource
							if(found_item == 0)
							{
								std::vector<resource*> resources = simulation_map->get_resources();

								// Loop through each building in the vector
								for(std::vector<resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
								{
									if((*it)->get_type() == resourcetype::water && (*it)->get_harvestable() == true)
									{
										(*iterator)->add_task(new task(tasktype::harvest, taskdata(std::make_pair((*it)->get_x(), (*it)->get_y()), *it)));
										found_item = 1;
										break;
									}
								}
							}

							// If no resources contain water, rest for a few seconds
							if(found_item == 0)
							{
								(*iterator)->add_task(new task(tasktype::rest, taskdata(std::make_pair((*iterator)->get_x(), (*iterator)->get_y()), SDL_GetTicks() + 2500)));
								(*iterator)->set_fatigue((*iterator)->get_fatigue() - 2);
							}
						}
					}
					else
					{
						// Scale down random number while preserving uniform distribution
						std::uniform_int_distribution<int> distribution(1, 2);
						int target_action = distribution(rng);
						std::cout << "Target Action: " << target_action << std::endl;
						if(target_action == 1) // Rest for a short duration
						{
							(*iterator)->add_task(new task(tasktype::rest, taskdata(std::make_pair((*iterator)->get_x(), (*iterator)->get_y()), SDL_GetTicks() + 2500)));
							(*iterator)->set_fatigue((*iterator)->get_fatigue() - 2);
						}
						else if(target_action == 2) // Harvest the closest resource
						{
							std::vector<resource*> resources = simulation_map->get_resources();
							std::pair<int, resource*> target(9999, nullptr);

							// Loop through each resource in the vector
							for(std::vector<resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
							{
								if((*it)->get_harvestable() == true && (*it)->get_type() != resourcetype::water)
								{
									int distance = abs((*iterator)->get_x() - (*it)->get_x()) + abs((*iterator)->get_y() - (*it)->get_y());

									if(distance < target.first)
									{
										target.first = distance;
										target.second = (*it);
									}
								}
							}

							// If a target resource is found, harvest it
							if(target.second != nullptr)
							{
								(*iterator)->add_task(new task(tasktype::harvest, taskdata(std::make_pair(target.second->get_x(), target.second->get_y()), target.second)));
							}
							else
							{
								(*iterator)->add_task(new task(tasktype::rest, taskdata(std::make_pair((*iterator)->get_x(), (*iterator)->get_y()), SDL_GetTicks() + 2500)));
								(*iterator)->set_fatigue((*iterator)->get_fatigue() - 2);
							}
						}
					}
					break;

				case tasktype::move : // Move towards the target coordinates
					//std::cout << "Task: Move." << std::endl;
					(*iterator)->move(data.target_coords.first, data.target_coords.second);

					if((*iterator)->is_at(data.target_coords.first, data.target_coords.second))
					{
						(*iterator)->remove_task();
					}
					break;

				case tasktype::harvest : // Take all the items from the target resource, resting between each harvest cycle
					if(!(*iterator)->is_at(data.target_coords.first, data.target_coords.second))
					{
						(*iterator)->move(data.target_coords.first, data.target_coords.second);
					}
					else
					{
						//std::cout << "Task: Harvest." << std::endl;
						inventory* inv = (*iterator)->get_inventory();
						inventory* target_inv = data.target_entity->get_inventory();

						if(target_inv->get_item_count() > 0)
						{
							std::cout << "Harvesting." << std::endl;
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

							(*iterator)->add_task(new task(tasktype::rest, taskdata(std::make_pair((*iterator)->get_x(), (*iterator)->get_y()), SDL_GetTicks() + pause_time)));
							(*iterator)->set_hunger((*iterator)->get_hunger() + 1);
							(*iterator)->set_thirst((*iterator)->get_thirst() + 2);
							(*iterator)->set_fatigue((*iterator)->get_fatigue() + 3);
							(*iterator)->add_task(new task(tasktype::take_item, taskdata(std::make_pair((*iterator)->get_x(), (*iterator)->get_y()), std::make_pair(data.target_entity, target_inv->get_items().back()))));
						}
						else
						{
							std::cout << "Stop harvesting." << std::endl;
							(*iterator)->remove_task();
						}
						break;
					}

				case tasktype::take_item : // Take the item from the target entity
					if(!(*iterator)->is_at(data.target_coords.first, data.target_coords.second))
					{
						(*iterator)->move(data.target_coords.first, data.target_coords.second);
					}
					else
					{
						//std::cout << "Task: Take Item." << std::endl;
						inventory* inv = (*iterator)->get_inventory();
						inventory* target_inv = data.target_item.first->get_inventory();

						std::unique_ptr<item> target_item = target_inv->take_item(data.target_item.second);

						if(target_item.get() != nullptr)
						{
							inv->add_item(std::move(target_item));
						}

						(*iterator)->remove_task();
						(*iterator)->add_task(new task(tasktype::rest, taskdata(std::make_pair((*iterator)->get_x(), (*iterator)->get_y()), SDL_GetTicks() + 500)));
						break;
					}

				case tasktype::store_item : // Store the item into the target entity
					if(!(*iterator)->is_at(data.target_coords.first, data.target_coords.second))
					{
						(*iterator)->move(data.target_coords.first, data.target_coords.second);
					}
					else
					{
						//std::cout << "Task: Store Item." << std::endl;
						inventory* inv = (*iterator)->get_inventory();
						inventory* target_inv = data.target_item.first->get_inventory();

						std::unique_ptr<item> target_item = inv->take_item(data.target_item.second);

						if(target_item.get() != nullptr)
						{
							target_inv->add_item(std::move(target_item));
						}

						(*iterator)->remove_task();
						(*iterator)->add_task(new task(tasktype::rest, taskdata(std::make_pair((*iterator)->get_x(), (*iterator)->get_y()), SDL_GetTicks() + 500)));
						break;
					}

				case tasktype::rest : // Wait until the set duration has passed
					//std::cout << "Task: Rest." << std::endl;
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
			current = came_from[current];
			path.push_back(simulation_map->get_tile_coords(current));
			++count;
		}

		// If a valid path was found within 200 checks, return it. Otherwise, return an empty vector
		return count < 200 ? path : std::vector<std::pair<int, int>>();
	}
}
