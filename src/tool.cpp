#include <tool.h>

namespace villa
{
	/**
	 * Constructor for the Tool class.
	 */
	tool::tool() : item(), efficiency(0) { }

	/**
	 * Constructor for the Tool class.
	 * @param type - The item type.
	 * @param efficiency - The tool efficiency.
	 */
	tool::tool(itemtype type, int efficiency) : item(type), efficiency(efficiency) { }

	/**
	 * Gets the efficiency of the tool.
	 * @return The tool efficiency.
	 */
	int tool::get_efficiency()
	{
		return efficiency;
	}

	/**
	 * Sets the efficiency of the tool.
	 * @param value - The tool efficiency.
	 */
	void tool::set_efficiency(int value)
	{
		efficiency = value;
	}
}
