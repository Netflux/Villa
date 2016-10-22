#ifndef INCLUDE_UI_ELEMENT_HPP_
#define INCLUDE_UI_ELEMENT_HPP_

#include <string>

namespace villa
{
	/**
	 * UI Element class.
	 * Represents all interactive user-interface elements.
	 */
	class ui_element
	{
		public:
			ui_element(int x, int y, int width, int height);
			bool is_at(int x, int y);

		private:
			int x;
			int y;
			int width;
			int height;
	};
}

#endif /* INCLUDE_UI_ELEMENT_HPP_ */
