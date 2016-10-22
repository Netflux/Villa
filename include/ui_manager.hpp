#ifndef INCLUDE_UI_MANAGER_HPP_
#define INCLUDE_UI_MANAGER_HPP_

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include "ui_element.hpp"

namespace villa
{
	/**
	 * UI Manager class.
	 * Handles all UI elements used by the application.
	 */
	class ui_manager
	{
		public:
			std::string click_at(int x, int y);
			void reset_element_clicked();
			void add_element(std::string name, ui_element* element);
			void remove_element(std::string name);
			ui_element* get_element(std::string name);
			std::string get_element_clicked();

		private:
			std::string element_clicked;
			std::unordered_map<std::string, std::unique_ptr<ui_element>> elements;
	};
}

#endif /* INCLUDE_UI_MANAGER_HPP_ */
