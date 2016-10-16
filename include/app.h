#ifndef INCLUDE_APP_H_
#define INCLUDE_APP_H_

#include <iostream>
#include <memory>
#include <stack>
#include <resource_manager.h>
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
			void handle_input();
			void update_display();
			std::stack<appstate> state;
			std::shared_ptr<resource_manager> resources;
			SDL_Window* window;
			SDL_Renderer* renderer;
			SDL_Event event;
	};
}

#endif /* INCLUDE_APP_H_ */
