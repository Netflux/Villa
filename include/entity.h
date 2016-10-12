#ifndef INCLUDE_ENTITY_H_
#define INCLUDE_ENTITY_H_

namespace villa
{
	/**
	 * Entity class.
	 * Represents all entities that physically appear in the simulation.
	 */
	class entity
	{
		public:
			int get_x();
			void set_x(int value);
			int get_y();
			void set_y(int value);

		protected:
			entity();
			entity(int x, int y);
			int x;
			int y;
	};
}

#endif /* INCLUDE_ENTITY_H_ */
