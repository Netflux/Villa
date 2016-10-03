#ifndef INCLUDE_TOOL_H_
#define INCLUDE_TOOL_H_

#include <item.h>

namespace villa
{
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
