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
			// Ignore if the current task is to move, idle or build
			if(!(*iterator)->is_at(data.target_coords.first, data.target_coords.second) && current_task->get_type() != tasktype::move && current_task->get_type() != tasktype::idle && current_task->get_type() != tasktype::build)
			{
				// If the target is within a single tile distance, move directly towards it
				if(abs((*iterator)->get_x() - data.target_coords.first) <= 16 && abs((*iterator)->get_y() - data.target_coords.second) <= 16)
				{
					(*iterator)->add_task(new task(tasktype::move, taskdata(std::make_pair(data.target_coords.first, data.target_coords.second))));
				}
				else
				{
					//std::cout << "Target X: " << data.target_coords.first << ". Target Y: " << data.target_coords.second << ". Current X: " << (*iterator)->get_x() << ". Current Y: " << (*iterator)->get_y() << std::endl;
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
			}

			// Perform the appropriate action according to task type
			switch(current_task->get_type())
			{
				case tasktype::idle : // Check current status and perform a new task
					//std::cout << "Task: Idle." << std::endl;
					handle_task_idle(*iterator);
					break;

				case tasktype::move : // Move towards the target coordinates
					//std::cout << "Task: Move." << std::endl;
					handle_task_move(*iterator);
					break;

				case tasktype::build : // Build a building
					if((*iterator)->is_at(data.target_coords.first, data.target_coords.second))
					{
						handle_task_build(*iterator);
						break;
					}

				case tasktype::harvest : // Take all the items from the target resource, resting between each harvest cycle
					if((*iterator)->is_at(data.target_coords.first, data.target_coords.second))
					{
						//std::cout << "Task: Harvest." << std::endl;
						handle_task_harvest(*iterator);
						break;
					}

				case tasktype::take_item : // Take the item from the target entity
					if((*iterator)->is_at(data.target_coords.first, data.target_coords.second))
					{
						//std::cout << "Task: Take Item." << std::endl;
						handle_task_take_item(*iterator);
						break;
					}

				case tasktype::store_item : // Store the item into the target entity
					if((*iterator)->is_at(data.target_coords.first, data.target_coords.second))
					{
						//std::cout << "Task: Store Item." << std::endl;
						handle_task_store_item(*iterator);
						break;
					}

				case tasktype::rest : // Wait until the set duration has passed
					//std::cout << "Task: Rest." << std::endl;
					handle_task_rest(*iterator);
					break;

				default:
					break;
			}
		}
	}

	/**
	 * Handles the idle task for the villager.
	 * @param value - The villager.
	 */
	void ai_manager::handle_task_idle(villager* value)
	{
		// Manage fatigue, hunger and thirst. If already handled, proceed with idle task.
		if(!handle_villager_needs(value))
		{
			// Scale down random number while preserving uniform distribution
			std::uniform_int_distribution<int> distribution(1, 100);
			int target_action = distribution(rng);
			//std::cout << "Target Action: " << target_action << std::endl;
			if(target_action <= 30) // Rest for a short duration (30% chance)
			{
				value->add_task(new task(tasktype::rest, taskdata(std::make_pair(value->get_x(), value->get_y()), SDL_GetTicks() + 2500)));
				value->set_fatigue(value->get_fatigue() - 4);
			}
			else if(target_action <= 80) // Harvest the closest resource (50% chance)
			{
				std::vector<resource*> resources = simulation_map->get_resources();
				std::pair<int, resource*> target(9999, nullptr);

				// Scale down random number while preserving uniform distribution
				std::uniform_int_distribution<int> distribution(1, 4);

				if(distribution(rng) == 1) // 25% chance to search for water resource
				{
					// Loop through each resource in the vector
					for(std::vector<resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
					{
						if((*it)->get_harvestable() == true && (*it)->get_type() == resourcetype::water)
						{
							int distance = abs(value->get_x() - (*it)->get_x()) + abs(value->get_y() - (*it)->get_y());

							if(distance < target.first)
							{
								target.first = distance;
								target.second = (*it);
							}
						}
					}
				}
				else // 75% chance to search for resource that is not water
				{
					// Loop through each resource in the vector
					for(std::vector<resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
					{
						if((*it)->get_harvestable() == true && (*it)->get_type() != resourcetype::water)
						{
							int distance = abs(value->get_x() - (*it)->get_x()) + abs(value->get_y() - (*it)->get_y());

							if(distance < target.first)
							{
								target.first = distance;
								target.second = (*it);
							}
						}
					}
				}

				// If a target resource is found, harvest it
				if(target.second != nullptr)
				{
					value->add_task(new task(tasktype::harvest, taskdata(std::make_pair(target.second->get_x(), target.second->get_y()), target.second)));
				}
				else
				{
					value->add_task(new task(tasktype::rest, taskdata(std::make_pair(value->get_x(), value->get_y()), SDL_GetTicks() + 2500)));
					value->set_fatigue(value->get_fatigue() - 4);
				}
			}
			else if(target_action <= 95) // Store some items (15% chance)
			{
				if(value->get_inventory()->get_item_count() > 25)
				{
					std::vector<building*> buildings = simulation_map->get_buildings();
					std::pair<int, building*> target(9999, nullptr);

					// Loop through each building in the vector
					for(std::vector<building*>::const_iterator it = buildings.begin(); it != buildings.end(); ++it)
					{
						int distance = abs(value->get_x() - (*it)->get_x()) + abs(value->get_y() - (*it)->get_y());

						if(distance < target.first)
						{
							target.first = distance;
							target.second = (*it);
						}
					}

					// If a target building is found, store some items in it
					if(target.second != nullptr)
					{
						// Scale down random number while preserving uniform distribution
						std::uniform_int_distribution<int> distribution(1, value->get_inventory()->get_item_count());
						int quantity = distribution(rng);

						for(int i = 0; i < quantity; ++i)
						{
							value->add_task(new task(tasktype::store_item, taskdata(std::make_pair(target.second->get_x(), target.second->get_y()), std::make_pair(target.second, value->get_inventory()->get_items()[i]))));
						}
					}
					else
					{
						value->add_task(new task(tasktype::rest, taskdata(std::make_pair(value->get_x(), value->get_y()), SDL_GetTicks() + 2500)));
						value->set_fatigue(value->get_fatigue() - 4);
					}
				}
			}
			else if(target_action <= 100) // Build a building (5% chance)
			{
				// Scale down random number while preserving uniform distribution
				std::uniform_int_distribution<int> distribution_type(1, 6);
				std::uniform_int_distribution<int> distribution(17, 783);
				int i = distribution(rng), j = distribution(rng), count = 0;

				buildingtype type = buildingtype::town_hall;
				bool result = false;

				switch(distribution_type(rng))
				{
					case 1 :
						type = buildingtype::town_hall;
						break;

					case 2 :
						type = buildingtype::house;
						break;

					case 3 :
						type = buildingtype::house_small;
						break;

					case 4 :
						type = buildingtype::farmhouse;
						break;

					case 5 :
						type = buildingtype::blacksmith;
						break;

					case 6 :
						type = buildingtype::stall;
						break;
				}

				// Try to place the building 5 times, then rest (longer duration if success)
				while(!result && count < 5)
				{
					i = distribution(rng);
					j = distribution(rng);

					result = simulation_map->get_available_space(i, j, type);
					count += 1;
				}

				if(result == true)
				{
					value->add_task(new task(tasktype::build, taskdata(std::make_pair(value->get_x(), value->get_y()), new building(i, j, type))));
				}
				else
				{
					value->add_task(new task(tasktype::rest, taskdata(std::make_pair(value->get_x(), value->get_y()), SDL_GetTicks() + 5000)));
					value->set_fatigue(value->get_fatigue() - 8);
				}
			}
		}
	}

	/**
	 * Handles the move task for the villager.
	 * @param value - The villager.
	 */
	void ai_manager::handle_task_move(villager* value)
	{
		taskdata data = value->get_task()->get_data();

		value->move(data.target_coords.first, data.target_coords.second);

		if(value->is_at(data.target_coords.first, data.target_coords.second))
		{
			value->remove_task();
		}
	}

	/**
	 * Handles the build task for the villager.
	 * @param value - The villager.
	 */
	void ai_manager::handle_task_build(villager* value)
	{
		// Manage fatigue, hunger and thirst. If already handled, proceed with idle task.
		if(!handle_villager_needs(value))
		{
			taskdata data = value->get_task()->get_data();

			// Scale down random number while preserving uniform distribution
			std::uniform_int_distribution<int> distribution(1, 2);

			if(value->get_inventory()->get_item_count(itemtype::lumber) + value->get_inventory()->get_item_count(itemtype::stone) >= 40)
			{
				for(int i = 0; i < 20; ++i)
				{
					value->get_inventory()->remove_item(itemtype::lumber);
					value->get_inventory()->remove_item(itemtype::stone);
				}

				simulation_map->add_building(data.target_building);
				value->remove_task();
				value->add_task(new task(tasktype::rest, taskdata(std::make_pair(data.target_building->get_x() + 8, data.target_building->get_y() + 16), SDL_GetTicks() + 10000)));
				value->set_hunger(value->get_hunger() + 5);
				value->set_thirst(value->get_thirst() + 5);
				value->set_fatigue(value->get_fatigue() + 5);

				// Scale down random number while preserving uniform distribution
				std::uniform_int_distribution<int> distribution_item(1, 4);
				std::uniform_int_distribution<int> distribution_efficiency(1, 100);

				// Add a random number of villagers at the new building
				for(int count = 0; count < distribution(rng); ++count)
				{
					villager* target = new villager(data.target_building->get_x() + 8, data.target_building->get_y() + 16);
					tool* target_tool = nullptr;

					switch(distribution_item(rng))
					{
						case 1 :
							target_tool = new tool(itemtype::axe, distribution_efficiency(rng));
							break;

						case 2 :
							target_tool = new tool(itemtype::bucket, distribution_efficiency(rng));
							break;

						case 3 :
							target_tool = new tool(itemtype::pickaxe, distribution_efficiency(rng));
							break;

						default :
							break;
					}

					if(target_tool != nullptr)
					{
						target->get_inventory()->add_item(target_tool);
					}

					simulation_map->add_villager(target);
				}
			}
			else
			{
				if(distribution(rng) == 1 && value->get_inventory()->get_item_count(itemtype::lumber) < 20)
				{
					// Look for a building that contains lumber
					std::pair<building*, item*> target_building = get_item_in_building(value->get_x(), value->get_y(), itemtype::lumber);

					if(target_building.first != nullptr && target_building.second != nullptr)
					{
						value->add_task(new task(tasktype::take_item, taskdata(std::make_pair(target_building.first->get_x(), target_building.first->get_y()), std::make_pair(target_building.first, target_building.second))));
					}
					else // If no buildings contain lumber, look for a lumber resource
					{
						resource* target_resource = get_closest_resource(value->get_x(), value->get_y(), resourcetype::tree);

						if(target_resource != nullptr)
						{
							value->add_task(new task(tasktype::harvest, taskdata(std::make_pair(target_resource->get_x(), target_resource->get_y()), target_resource)));
						}
						else
						{
							value->add_task(new task(tasktype::rest, taskdata(std::make_pair(value->get_x(), value->get_y()), SDL_GetTicks() + 2500)));
							value->set_fatigue(value->get_fatigue() - 4);
						}
					}
				}
				else if(value->get_inventory()->get_item_count(itemtype::stone) < 20)
				{
					// Look for a building that contains stone
					std::pair<building*, item*> target_building = get_item_in_building(value->get_x(), value->get_y(), itemtype::stone);

					if(target_building.first != nullptr && target_building.second != nullptr)
					{
						value->add_task(new task(tasktype::take_item, taskdata(std::make_pair(target_building.first->get_x(), target_building.first->get_y()), std::make_pair(target_building.first, target_building.second))));
					}
					else // If no buildings contain stone, look for a stone resource
					{
						resource* target_resource = get_closest_resource(value->get_x(), value->get_y(), resourcetype::stone);

						if(target_resource != nullptr)
						{
							value->add_task(new task(tasktype::harvest, taskdata(std::make_pair(target_resource->get_x(), target_resource->get_y()), target_resource)));
						}
						else
						{
							target_resource = get_closest_resource(value->get_x(), value->get_y(), resourcetype::ore);

							if(target_resource != nullptr)
							{
								value->add_task(new task(tasktype::harvest, taskdata(std::make_pair(target_resource->get_x(), target_resource->get_y()), target_resource)));
							}
							else
							{
								value->add_task(new task(tasktype::rest, taskdata(std::make_pair(value->get_x(), value->get_y()), SDL_GetTicks() + 2500)));
								value->set_fatigue(value->get_fatigue() - 4);
							}
						}
					}
				}
			}
		}
	}

	/**
	 * Handles the harvest task for the villager.
	 * @param value - The villager.
	 */
	void ai_manager::handle_task_harvest(villager* value)
	{
		taskdata data = value->get_task()->get_data();

		if(data.target_entity->get_inventory()->get_item_count() > 0)
		{
			//std::cout << "Harvesting." << std::endl;
			value->harvest(SDL_GetTicks());
		}
		else
		{
			//std::cout << "Stop harvesting." << std::endl;
			value->remove_task();
		}
	}

	/**
	 * Handles the take item task for the villager.
	 * @param value - The villager.
	 */
	void ai_manager::handle_task_take_item(villager* value)
	{
		taskdata data = value->get_task()->get_data();

		inventory* inv = value->get_inventory();
		inventory* target_inv = data.target_item.first->get_inventory();

		std::unique_ptr<item> target_item = target_inv->take_item(data.target_item.second);

		if(target_item.get() != nullptr)
		{
			inv->add_item(std::move(target_item));
		}

		value->remove_task();
		value->add_task(new task(tasktype::rest, taskdata(std::make_pair(value->get_x(), value->get_y()), SDL_GetTicks() + 500)));
	}

	/**
	 * Handles the store item task for the villager.
	 * @param value - The villager.
	 */
	void ai_manager::handle_task_store_item(villager* value)
	{
		taskdata data = value->get_task()->get_data();

		inventory* inv = value->get_inventory();
		inventory* target_inv = data.target_item.first->get_inventory();

		std::unique_ptr<item> target_item = inv->take_item(data.target_item.second);

		if(target_item.get() != nullptr)
		{
			target_inv->add_item(std::move(target_item));
		}

		value->remove_task();
		value->add_task(new task(tasktype::rest, taskdata(std::make_pair(value->get_x(), value->get_y()), SDL_GetTicks() + 500)));
	}

	/**
	 * Handles the rest task for the villager.
	 * @param value - The villager.
	 */
	void ai_manager::handle_task_rest(villager* value)
	{
		// Scale down random number while preserving uniform distribution
		std::uniform_int_distribution<int> distribution_chance(1, 500);

		// Roll for a chance to move positions
		if(distribution_chance(rng) == 1)
		{
			std::uniform_int_distribution<int> distribution_position(0, 48);
			int i = distribution_position(rng) - 16, j = distribution_position(rng) - 16;

			// Check that the new position is valid
			if(value->get_x() + i >= 0 && value->get_x() + i <= 800 && value->get_y() + j >= 0 && value->get_y() + j <= 800 && simulation_map->get_tile_at((value->get_x() + i) / 16, (value->get_y() + j) / 16) != nullptr && simulation_map->get_tile_at((value->get_x() + i) / 16, (value->get_y() + j) / 16)->get_pathable() == true)
			{
				value->remove_task();
				value->add_task(new task(tasktype::rest, taskdata(std::make_pair(value->get_x() + i, value->get_y() + j), SDL_GetTicks() + 1000)));
			}
		}

		value->rest(SDL_GetTicks());
	}

	/**
	 * Handles the villagers needs (fatigue, hunger, thirst).
	 * @param value - The villager.
	 */
	bool ai_manager::handle_villager_needs(villager* value)
	{
		// Scale down random number while preserving uniform distribution
		std::uniform_int_distribution<int> distribution(1, 2);

		if(value->get_fatigue() >= 60)
		{
			// Rest to reduce fatigue
			value->add_task(new task(tasktype::rest, taskdata(std::make_pair(value->get_x(), value->get_y()), SDL_GetTicks() + 10000)));
			value->set_fatigue(value->get_fatigue() - 16);

			return true;
		}
		else if(distribution(rng) == 1 && value->get_hunger() >= 60)
		{
			// Look for some food to consume
			item* food = value->get_inventory()->get_item(itemtype::food);

			if(food != nullptr)
			{
				value->get_inventory()->remove_item(food);
				value->set_hunger(value->get_hunger() - 4);
			}
			else // If the villager does not have food, look for food in a building
			{
				std::pair<building*, item*> target_building = get_item_in_building(value->get_x(), value->get_y(), itemtype::food);

				if(target_building.first != nullptr && target_building.second != nullptr)
				{
					value->add_task(new task(tasktype::take_item, taskdata(std::make_pair(target_building.first->get_x() + 8, target_building.first->get_y()), std::make_pair(target_building.first, target_building.second))));
				}
				else // If no buildings contain food, look for a food resource
				{
					resource* target_resource = get_closest_resource(value->get_x(), value->get_y(), resourcetype::food);

					if(target_resource != nullptr)
					{
						value->add_task(new task(tasktype::harvest, taskdata(std::make_pair(target_resource->get_x(), target_resource->get_y()), target_resource)));
					}
					else
					{
						value->add_task(new task(tasktype::rest, taskdata(std::make_pair(value->get_x(), value->get_y()), SDL_GetTicks() + 2500)));
						value->set_fatigue(value->get_fatigue() - 4);
					}
				}
			}

			return true;
		}
		else if(value->get_thirst() >= 60)
		{
			// Look for some water to consume in the villagers inventory
			item* water = value->get_inventory()->get_item(itemtype::water);

			if(water != nullptr)
			{
				value->get_inventory()->remove_item(water);
				value->set_thirst(value->get_thirst() - 4);
			}
			else // If the villager does not have water, look for water in a building
			{
				std::pair<building*, item*> target_building = get_item_in_building(value->get_x(), value->get_y(), itemtype::water);

				if(target_building.first != nullptr && target_building.second != nullptr)
				{
					value->add_task(new task(tasktype::take_item, taskdata(std::make_pair(target_building.first->get_x() + 8, target_building.first->get_y()), std::make_pair(target_building.first, target_building.second))));
				}
				else // If no buildings contain water, look for a water resource
				{
					resource* target_resource = get_closest_resource(value->get_x(), value->get_y(), resourcetype::water);

					if(target_resource != nullptr)
					{
						value->add_task(new task(tasktype::harvest, taskdata(std::make_pair(target_resource->get_x(), target_resource->get_y()), target_resource)));
					}
					else
					{
						value->add_task(new task(tasktype::rest, taskdata(std::make_pair(value->get_x(), value->get_y()), SDL_GetTicks() + 2500)));
						value->set_fatigue(value->get_fatigue() - 4);
					}
				}
			}

			return true;
		}

		return false;
	}

	/**
	 * Gets the closest building to the target coords that contains the item type.
	 * @param x - The x-coords of the target.
	 * @param y - The y-coords of the target.
	 * @param type - The item type to search for.
	 * @return The closest building to the target coords.
	 */
	std::pair<building*, item*> ai_manager::get_item_in_building(int x, int y, itemtype type)
	{
		std::vector<building*> buildings = simulation_map->get_buildings();
		std::pair<int, building*> target(9999, nullptr);

		// Loop through each building in the vector for a food item
		for(std::vector<building*>::const_iterator it = buildings.begin(); it != buildings.end(); ++it)
		{
			if((*it)->get_inventory()->get_item(type) != nullptr)
			{
				int distance = abs(x - (*it)->get_x()) + abs(y - (*it)->get_y());

				if(distance < target.first)
				{
					target.first = distance;
					target.second = (*it);
				}
			}
		}

		// If a target building containing the item type is found, return it
		if(target.second != nullptr && target.second->get_inventory()->get_item(type) != nullptr)
		{
			return std::make_pair(target.second, target.second->get_inventory()->get_item(type));
		}

		return std::make_pair(nullptr, nullptr);
	}

	/**
	 * Gets the closest resource to the target coords that contains the resource type.
	 * @param x - The x-coords of the target.
	 * @param y - The y-coords of the target.
	 * @param type - The resource type to search for.
	 * @return The closest resource to the target coords.
	 */
	resource* ai_manager::get_closest_resource(int x, int y, resourcetype type)
	{
		std::vector<resource*> resources = simulation_map->get_resources();
		std::pair<int, resource*> target(9999, nullptr);

		// Scale down random number while preserving uniform distribution
		std::uniform_int_distribution<int> distribution(1, 10);

		// Loop through each resource in the vector
		for(std::vector<resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
		{
			if((*it)->get_type() == type && (*it)->get_harvestable() == true)
			{
				int distance = abs(x - (*it)->get_x()) + abs(y - (*it)->get_y()) + distribution(rng);

				if(distance < target.first)
				{
					target.first = distance;
					target.second = (*it);
				}
			}
		}

		// If a target resource of the type is found, return it
		if(target.second != nullptr)
		{
			return target.second;
		}

		return nullptr;
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
				double new_cost = (cost_so_far[current] + (it - neighbours.begin() < 4 ? 1 : 1.414));
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
