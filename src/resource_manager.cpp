#include "resource_manager.hpp"

namespace villa
{
	/**
	 * Constructor for the Resource Manager class.
	 * @param renderer - The application renderer.
	 */
	resource_manager::resource_manager(SDL_Renderer& renderer) : renderer(renderer) { }

	/**
	 * Unloads all resources in the resource manager.
	 */
	void resource_manager::unload_resources()
	{
		// Loop through each loaded texture and clean from memory
		for(std::unordered_map<std::string, SDL_Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
		{
			SDL_DestroyTexture(it->second);
		}

		// Loop through each loaded font and clean from memory
		for(std::unordered_map<std::string, TTF_Font*>::iterator it = fonts.begin(); it != fonts.end(); ++it)
		{
			TTF_CloseFont(it->second);
		}
	}

	/**
	 * Loads the specified PNG as a texture.
	 * @param name - The name of the new texture.
	 * @param path - The filepath of the PNG.
	 */
	void resource_manager::load_texture(std::string name, std::string path)
	{
		SDL_Surface* surface = IMG_Load(path.c_str());

		if(surface == nullptr)
		{
			std::cerr << "Failed to load PNG: " << path << "\nSDL_image Error: " << IMG_GetError() << std::endl;
		}
		else
		{
			// Create a texture from the surface (supports hardware-based rendering)
			SDL_Texture* texture = SDL_CreateTextureFromSurface(&renderer, surface);

			if(texture == nullptr)
			{
				std::cerr << "Failed to create texture from PNG: " << path << "\nSDL_Error: " << SDL_GetError() << std::endl;
			}
			else
			{
				textures.insert(std::make_pair(name, texture));
				std::cout << "Successfully loaded PNG: " << path << std::endl;
			}

			// Free the temporary surface from memory
			SDL_FreeSurface(surface);
		}
	}

	/**
	 * Loads the specified font.
	 * @param name - The name of the new font.
	 * @param path - The filepath of the font.
	 * @param size - The font size.
	 */
	void resource_manager::load_font(std::string name, std::string path, int size)
	{
		TTF_Font* font = TTF_OpenFont(path.c_str(), size);

		if(font == nullptr)
		{
			std::cerr << "Failed to load font: " << path << "\nSDL_TTF Error: " << TTF_GetError() << std::endl;
		}
		else
		{
			fonts.insert(std::make_pair(name + std::to_string(size), font));
			std::cout << "Successfully loaded font: " << path << std::endl;
		}
	}

	/**
	 * Renders a texture on screen.
	 * @param x - The x-coord of the texture.
	 * @param y - The y-coord of the texture.
	 * @param name - The texture name.
	 */
	void resource_manager::render_texture(int x, int y, std::string name)
	{
		SDL_Texture* texture = this->get_texture(name);

		if(texture != nullptr)
		{
			// Set the x, y, width and height of the texture
			// The width and height are automatically queried from the texture
			SDL_Rect texture_rect;
			texture_rect.x = x;
			texture_rect.y = y;
			SDL_QueryTexture(texture, nullptr, nullptr, &texture_rect.w, &texture_rect.h);

			SDL_RenderCopy(&renderer, texture, nullptr, &texture_rect);
		}
	}

	/**
	 * Renders a string of text on screen.
	 * @param x - The x-coord of the text.
	 * @param y - The y-coord of the text.
	 * @param text - The text to render.
	 * @param font - The font name.
	 * @param size - The font size.
	 */
	void resource_manager::render_text(int x, int y, std::string text, std::string font, int size, SDL_Color color)
	{
		int new_x = x;

		// Loop through each character in the string
		for(char& c : text)
		{
			SDL_Texture* texture = this->get_texture_text(std::string(1, c), font, size, color);

			if(texture != nullptr)
			{
				// Set the x, y, width and height of the texture
				// The width and height are automatically queried from the texture
				SDL_Rect texture_rect;
				texture_rect.x = new_x;
				texture_rect.y = y;
				SDL_QueryTexture(texture, nullptr, nullptr, &texture_rect.w, &texture_rect.h);

				SDL_RenderCopy(&renderer, texture, nullptr, &texture_rect);

				// Set the new x-coord for the next letter
				new_x += texture_rect.w;
			}
		}
	}

	/**
	 * Gets the texture with the associated name.
	 * @param name - The name of the texture.
	 * @return The texture (nullptr if not found).
	 */
	SDL_Texture* resource_manager::get_texture(std::string name)
	{
		// Search through the loaded textures for the target texture
		std::unordered_map<std::string, SDL_Texture*>::iterator target = textures.find(name);

		// If the target texture is found, return its pointer
		if(target != textures.end())
		{
			return target->second;
		}

		std::cerr << "Attempted to use texture that has not been loaded: " << name << std::endl;
		return nullptr;
	}

	/**
	 * Gets the texture that displays the associated text.
	 * If the texture does not exist, it is created.
	 * @param text - The text to display.
	 * @param font - The font name.
	 * @param size - The font size.
	 * @return The texture (nullptr if texture creation fails).
	 */
	SDL_Texture* resource_manager::get_texture_text(std::string text, std::string font, int size, SDL_Color color)
	{
		// Search through the loaded textures for the target texture
		std::unordered_map<std::string, SDL_Texture*>::iterator target = textures_text.find(text + font + std::to_string(color.r) + std::to_string(color.g) + std::to_string(color.b));

		// If the target texture is found, return its pointer
		if(target != textures_text.end())
		{
			return target->second;
		}
		// If the target texture is not found, create it and return its pointer
		else
		{
			TTF_Font* fontface = this->get_font(font, size);

			// If there's text and the font is loaded...
			if(!text.empty() && fontface != nullptr)
			{
				// Create a surface that displays the text
				SDL_Surface* surface = TTF_RenderText_Blended(fontface, text.c_str(), color);

				if(surface == nullptr)
				{
					std::cerr << "Failed to create surface from text: \nText: " << text << "\nFont: " << font << "\nSDL_TTF Error: " << TTF_GetError() << std::endl;
				}
				else
				{
					// Create a texture from the surface (supports hardware-based rendering)
					SDL_Texture* texture = SDL_CreateTextureFromSurface(&renderer, surface);

					if(texture == nullptr)
					{
						std::cerr << "Failed to create texture from text: \nText: " << text << "\nFont: " << font << "\nSDL_TTF Error: " << TTF_GetError() << std::endl;
					}
					else
					{
						textures_text.insert(std::make_pair(text + font + std::to_string(color.r) + std::to_string(color.g) + std::to_string(color.b), texture));
						return texture;
					}

					// Free the temporary surface from memory
					SDL_FreeSurface(surface);
				}
			}

			std::cerr << "Attempted to create texture from invalid text/font: \nText: " << text << "\nFont: " << font << "\nSize: " << std::to_string(size) << std::endl;
			return nullptr;
		}
	}

	/**
	 * Gets the font with the associated name.
	 * @param name - The name of the font.
	 * @param size - The font size.
	 * @return The font (nullptr if not found).
	 */
	TTF_Font* resource_manager::get_font(std::string name, int size)
	{
		// Search through the loaded fonts for the target font
		std::unordered_map<std::string, TTF_Font*>::iterator target = fonts.find(name + std::to_string(size));

		// If the target font is found, return its pointer
		if(target != fonts.end())
		{
			return target->second;
		}

		std::cerr << "Attempted to use font that has not been loaded: \nFont: " << name << "\nSize: " << std::to_string(size) << std::endl;
		return nullptr;
	}
}
