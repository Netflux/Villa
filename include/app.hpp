#ifndef INCLUDE_APP_H_
#define INCLUDE_APP_H_

#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <stack>
#include <time.h>
#include "ai_manager.hpp"
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
		simulation_end,
		exit
	};

	/**
	 * Application timer struct.
	 */
	struct timer_struct
	{
		unsigned int app;
		unsigned int simulation_time;
		unsigned int simulation_start;
		unsigned int simulation_end;
		unsigned int villager_health;
		unsigned int villager_health_regen;
		unsigned int villager_needs;
		double timescale;
	};

	/**
	 * App class.
	 * Represents the complete program.
	 */
	class app
	{
		public:
			std::mt19937 rng;
			app();
			~app();
			void start();

		private:
			bool init();
			void load_resources();
			void load_ui();
			void handle_input();
			void update_simulation();
			void update_display();
			void display_menu_main();
			void display_simulation();
			void display_simulation_tile(int x, int y, tiletype type, std::string name);
			timer_struct timers;
			std::stack<appstate> state;
			std::unique_ptr<resource_manager> resources;
			std::unique_ptr<ui_manager> user_interface;
			std::unique_ptr<map> simulation_map;
			std::unique_ptr<ai_manager> simulation_ai;
			SDL_Window* window;
			SDL_Renderer* renderer;
			SDL_Event event;
	};
}

#endif /* INCLUDE_APP_H_ */
