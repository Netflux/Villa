#ifndef INCLUDE_DISPLAY_MANAGER_H_
#define INCLUDE_DISPLAY_MANAGER_H_

#include <iostream>
#include <memory>
#include <string>
#include <resource_manager.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace villa
{
	/**
	 * Display Manager class.
	 * Handles all display updates for the application.
	 */
	class display_manager
	{
		public:
			display_manager(SDL_Renderer& renderer, std::shared_ptr<resource_manager> resources);
			void render_texture(int x, int y, std::string name);
			void render_text(int x, int y, std::string text, std::string font, int size);

		private:
			SDL_Renderer& renderer;
			std::shared_ptr<resource_manager> resources;
	};
}

#endif /* INCLUDE_DISPLAY_MANAGER_H_ */
