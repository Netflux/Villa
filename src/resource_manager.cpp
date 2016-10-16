#include <resource_manager.h>

namespace villa
{
	/**
	 * Constructor for the Resource Manager class.
	 * @param renderer - The application renderer.
	 */
	resource_manager::resource_manager(SDL_Renderer& renderer) : renderer(renderer) { }

	/**
	 * Destructor for the Resource Manager class.
	 */
	resource_manager::~resource_manager()
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
		}
	}

	/**
	 * Gets the texture with the associated name.
	 * @param value - The name of the texture.
	 * @return The texture (nullptr if not found).
	 */
	SDL_Texture* resource_manager::get_texture(std::string value)
	{
		// Search through the loaded textures for the target texture
		std::unordered_map<std::string, SDL_Texture*>::iterator target = textures.find(value);

		// If the target texture is found, return its pointer
		if(target != textures.end())
		{
			return target->second;
		}

		std::cerr << "Attempted to use texture that has not been loaded: " << value << std::endl;
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
	SDL_Texture* resource_manager::get_texture_text(std::string text, std::string font, int size)
	{
		TTF_Font* fontface = this->get_font(font, size);
		SDL_Color color = {0, 0, 0};

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
					textures_text.insert(std::make_pair(text + font, texture));
					return texture;
				}

				// Free the temporary surface from memory
				SDL_FreeSurface(surface);
			}
		}

		std::cerr << "Attempted to create texture from invalid text/font: \nText: " << text << "\nFont: " << font << "\nSize: " << std::to_string(size) << std::endl;
		return nullptr;
	}

	/**
	 * Gets the font with the associated name.
	 * @param value - The name of the font.
	 * @param size - The font size.
	 * @return The font (nullptr if not found).
	 */
	TTF_Font* resource_manager::get_font(std::string value, int size)
	{
		// Search through the loaded fonts for the target font
		std::unordered_map<std::string, TTF_Font*>::iterator target = fonts.find(value + std::to_string(size));

		// If the target font is found, return its pointer
		if(target != fonts.end())
		{
			return target->second;
		}

		std::cerr << "Attempted to use font that has not been loaded: \nFont: " << value << "\nSize: " << std::to_string(size) << std::endl;
		return nullptr;
	}
}
