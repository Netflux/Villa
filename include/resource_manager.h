#ifndef INCLUDE_RESOURCE_MANAGER_H_
#define INCLUDE_RESOURCE_MANAGER_H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace villa
{
	/**
	 * Resource Manager class.
	 * Handles all resource files used by the application.
	 */
	class resource_manager
	{
		public:
			resource_manager(SDL_Renderer& renderer);
			~resource_manager();
			void load_texture(std::string name, std::string path);
			void load_font(std::string name, std::string path, int size);
			void render_texture(int x, int y, std::string name);
			void render_text(int x, int y, std::string text, std::string font, int size);
			SDL_Texture* get_texture(std::string value);
			SDL_Texture* get_texture_text(std::string text, std::string font, int size);
			TTF_Font* get_font(std::string value, int size);

		private:
			SDL_Renderer& renderer;
			std::unordered_map<std::string, SDL_Texture*> textures;
			std::unordered_map<std::string, SDL_Texture*> textures_text;
			std::unordered_map<std::string, TTF_Font*> fonts;
	};
}

#endif /* INCLUDE_RESOURCE_MANAGER_H_ */
