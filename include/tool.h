#ifndef INCLUDE_TOOL_H_
#define INCLUDE_TOOL_H_

#include <item.h>

namespace villa
{
	/**
	 * Tool class.
	 * Represents items that provide bonuses when carrying out certain actions.
	 */
	class tool : public item
	{
		public:
			tool();
			tool(itemtype type, int efficiency);
			int get_efficiency();
			void set_efficiency(int value);

		private:
			int efficiency;
	};
}

#endif /* INCLUDE_TOOL_H_ */
