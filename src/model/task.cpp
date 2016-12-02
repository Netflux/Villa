#include "task.hpp"

namespace villa
{
	/**
	 * Constructor for the Task Data struct.
	 * @param target_coords - The x and y coords to conduct the task.
	 */
	taskdata::taskdata(std::pair<int, int> target_coords) : target_coords(target_coords) { }

	/**
	 * Constructor for the Task Data struct.
	 * @param target_coords - The x and y coords to conduct the task.
	 * @param target_entity - The target entity.
	 */
	taskdata::taskdata(std::pair<int, int> target_coords, entity* target_entity) : target_coords(target_coords), target_entity(target_entity) { }

	/**
	 * Constructor for the Task Data struct.
	 * @param target_coords - The x and y coords to conduct the task.
	 * @param target_building - The target building.
	 */
	taskdata::taskdata(std::pair<int, int> target_coords, building* target_building) : target_coords(target_coords), target_building(target_building) { }

	/**
	 * Constructor for the Task Data struct.
	 * @param target_coords - The x and y coords to conduct the task.
	 * @param target_item - The target item.
	 */
	taskdata::taskdata(std::pair<int, int> target_coords, std::pair<entity*, item*> target_item) : target_coords(target_coords), target_item(target_item) { }

	/**
	 * Constructor for the Task Data struct.
	 * @param target_coords - The x and y coords to conduct the task.
	 * @param time - The amount of time to wait.
	 */
	taskdata::taskdata(std::pair<int, int> target_coords, int time) : target_coords(target_coords), time(time) { }

	/**
	 * Constructor for the Task class.
	 * @param type - The task type.
	 * @param data - The task data.
	 */
	task::task(tasktype type, taskdata data) : data(std::make_pair(type, data)) { }

	/**
	 * Gets the type of the task.
	 * @return The task type.
	 */
	tasktype task::get_type()
	{
		return this->data.first;
	}

	/**
	 * Gets the data associated with the task.
	 * @return The task data.
	 */
	taskdata task::get_data()
	{
		return this->data.second;
	}
}
