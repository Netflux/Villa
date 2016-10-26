#ifndef INCLUDE_APP_H_
#define INCLUDE_APP_H_

#include <iostream>
#include <memory>
#include <stack>
#include "map.hpp"
#include "resource_manager.hpp"
#include "ui_manager.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace villa
{
	/**
	 * Application state enumeration.
	 */
	enum class appstate {
		menu_main,
		simulation,
		exit
	};

	/**
	 * App class.
	 * Represents the complete program.
	 */
	class app
	{
		public:
			app();
			~app();
			void start();

		private:
			bool init();
			void load_resources();
			void load_ui();
			void handle_input();
			void update_display();
			void display_menu_main();
			void display_simulation();
			void display_simulation_tile(int x, int y, tiletype type, std::string name);
			std::stack<appstate> state;
			std::unique_ptr<resource_manager> resources;
			std::unique_ptr<ui_manager> user_interface;
			std::unique_ptr<map> simulation_map;
			SDL_Window* window;
			SDL_Renderer* renderer;
			SDL_Event event;
	};
}

#endif /* INCLUDE_APP_H_ */
