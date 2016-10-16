#include <display_manager.h>

namespace villa
{
	/**
	 * Constructor for the Display Manager class.
	 * @param renderer - The application renderer.
	 * @param resources - The application resource manager.
	 */
	display_manager::display_manager(SDL_Renderer& renderer, std::shared_ptr<resource_manager> resources) : renderer(renderer), resources(resources) { }

	/**
	 * Renders a texture on screen.
	 * @param x - The x-coord of the texture.
	 * @param y - The y-coord of the texture.
	 * @param name - The texture name.
	 */
	void display_manager::render_texture(int x, int y, std::string name)
	{
		SDL_Texture* texture = resources->get_texture(name);

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
	void display_manager::render_text(int x, int y, std::string text, std::string font, int size)
	{
		int new_x = x;

		// Loop through each character in the string
		for(char& c : text)
		{
			SDL_Texture* texture = resources->get_texture_text(std::string(&c), font, size);

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
}
