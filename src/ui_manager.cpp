#include "ui_manager.hpp"

namespace villa
{
	/**
	 * Simulates a click at the specified x and y coords.
	 * @param x - The x-coord of the click.
	 * @param y - The y-coord of the click.
	 * @return The name of the UI element that was clicked (empty string if none clicked).
	 */
	std::string ui_manager::click_at(int x, int y)
	{
		// Reset the currently clicked UI element
		reset_element_clicked();

		// Loop through each UI element
		for(std::unordered_map<std::string, std::unique_ptr<ui_element>>::iterator it = elements.begin(); it != elements.end(); ++it)
		{
			if(it->second->is_at(x, y))
			{
				element_clicked = it->first;
				return element_clicked;
			}
		}

		return "";
	}

	/**
	 * Resets the currently clicked UI element.
	 */
	void ui_manager::reset_element_clicked()
	{
		element_clicked = "";
	}

	/**
	 * Adds the specified UI element to the user interface.
	 * @param name - The name of the UI element.
	 * @param element - The UI element to add.
	 */
	void ui_manager::add_element(std::string name, ui_element* element)
	{
		elements.insert(std::make_pair(name, std::unique_ptr<ui_element>(element)));
	}

	/**
	 * Removes the specified UI element from the user interface.
	 * @param name - The name of the UI element.
	 */
	void ui_manager::remove_element(std::string name)
	{
		elements.erase(name);
	}

	/**
	 * Gets the UI element with the associated name.
	 * @param name - The name of the UI element.
	 * @return The UI element (nullptr if not found).
	 */
	ui_element* ui_manager::get_element(std::string name)
	{
		// Search through the loaded textures for the target texture
		std::unordered_map<std::string, std::unique_ptr<ui_element>>::iterator target = elements.find(name);

		// If the target element is found, return its pointer
		if(target != elements.end())
		{
			return target->second.get();
		}

		std::cerr << "Attempted to use UI element that has not been loaded: " << name << std::endl;
		return nullptr;
	}

	/**
	 * Gets the last element that was clicked.
	 * @return The UI element (empty string if none clicked).
	 */
	std::string ui_manager::get_element_clicked()
	{
		return element_clicked;
	}
}
