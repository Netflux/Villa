#ifndef INCLUDE_MODEL_TASK_HPP_
#define INCLUDE_MODEL_TASK_HPP_

#include <unordered_map>
#include "building.hpp"
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
		taskdata(std::pair<int, int> target_coords, building* target_building);
		taskdata(std::pair<int, int> target_coords, std::pair<entity*, item*> target_item);
		taskdata(std::pair<int, int> target_coords, int time);

		std::pair<int, int> target_coords;
		union
		{
			entity* target_entity;
			building* target_building;
			std::pair<entity*, item*> target_item;
			int time;
		};
	};

	/**
	 * Task type enumeration.
	 */
	enum class tasktype
	{
		idle,      //!< idle
		move,      //!< move
		build,     //!< build
		harvest,   //!< harvest
		take_item, //!< take_item
		store_item,//!< store_item
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
