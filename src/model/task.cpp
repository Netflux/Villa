#include "task.hpp"

namespace villa
{
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
		return data.first;
	}

	/**
	 * Gets the data associated with the task.
	 * @return The task data.
	 */
	taskdata task::get_data()
	{
		return data.second;
	}
}
