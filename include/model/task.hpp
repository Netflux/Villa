#ifndef INCLUDE_MODEL_TASK_HPP_
#define INCLUDE_MODEL_TASK_HPP_

#include <unordered_map>
#include "entity.hpp"
#include "item.hpp"
#include "tile.hpp"

namespace villa
{
	/**
	 * Task data union.
	 */
	struct taskdata
	{
		taskdata(std::pair<int, int> target_coords);
		taskdata(std::pair<int, int> target_coords, entity* target_entity);
		taskdata(std::pair<int, int> target_coords, std::pair<entity*, item*> target_item);
		taskdata(std::pair<int, int> target_coords, unsigned int time);

		std::pair<int, int> target_coords;
		union
		{
			entity* target_entity;
			std::pair<entity*, item*> target_item;
			unsigned int time;
		};
	};

	/**
	 * Task type enumeration.
	 */
	enum class tasktype
	{
		idle,      //!< idle
		move,      //!< move
		harvest,   //!< harvest
		take_item, //!< take_item
		store_item,//!< store_item
		eat,       //!< eat
		rest       //!< rest
	};

	/**
	 * Task class.
	 * Represents a task that can be carried out by villagers.
	 */
	class task
	{
		public:
			task(tasktype type, taskdata data);
			tasktype get_type();
			taskdata get_data();

		private:
			std::pair<tasktype, taskdata> data;
	};
}

#endif /* INCLUDE_MODEL_TASK_HPP_ */
