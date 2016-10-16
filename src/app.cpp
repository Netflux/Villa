#include <app.h>

namespace villa
{
	/**
	 * Constructor for the App class.
	 */
	app::app() : window(nullptr), renderer(nullptr)
	{
		// Set initial state to exit
		state.push(appstate::exit);
	}

	/**
	 * Destructor for the App class.
	 */
	app::~app()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		//IMG_Quit();
		//TTF_Quit();

		SDL_Quit();
	}

	/**
	 * Starts the application.
	 * Contains the application loop and all simulation logic.
	 */
	void app::start()
	{
		// If the application initializes correctly, start the application
		if(this->init())
		{
			std::cout << "Starting application..." << std::endl;

			// Load resources and push initial application state
			this->load_resources();
			state.push(appstate::menu_main);

			// Loop until the user exits the application
			while(state.top() != appstate::exit)
			{
				this->handle_input();
				this->update_display();
			}

			std::cout << "Exiting application..." << std::endl;
		}
	}

	/**
	 * Initializes the application (SDL and its subsystems).
	 * @return The success/failure for initialization.
	 */
	bool app::init()
	{
		std::cout << "Initializing application instance..." << std::endl;

		// Check if SDL initializes successfully
		if(SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cerr << "SDL failed to initialize! SDL_Error: " << SDL_GetError() << std::endl;
			return false;
		}

		std::cout << "SDL successfully initialized." << std::endl;

		// Initialize the application window
		window = SDL_CreateWindow("Villa", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);

		// Check if the application window initializes successfully
		if(window == nullptr)
		{
			std::cerr << "Application window failed to initialize! SDL_Error: " << SDL_GetError() << std::endl;
			return false;
		}

		std::cout << "Application window successfully initialized." << std::endl;

		// Initialize the 2D renderer
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		// Check if the 2D renderer initializes successfully
		if(renderer == nullptr)
		{
			std::cerr << "2D Renderer failed to initialize! SDL_Error: " << SDL_GetError() << std::endl;
			return false;
		}

		std::cout << "2D Renderer successfully initialized." << std::endl;

		// Initialize the resource manager
		resources.reset(new resource_manager(*renderer));

		// Check if the PNG loader initializes successfully
		if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG))
		{
			std::cerr << "PNG Loader failed to initialize! SDL_Error: " << SDL_GetError() << std::endl;
			return false;
		}

		std::cout << "PNG Loader successfully initialized." << std::endl;

		// Check if the TTF loader initializes successfully
		if(TTF_Init() == -1)
		{
			std::cerr << "TTF Loader failed to initialize! SDL_Error: " << TTF_GetError() << std::endl;
			return false;
		}

		std::cout << "TTF Loader successfully initialized." << std::endl;

		std::cout << "Application instance successfully initialized." << std::endl;

		return true;
	}

	/**
	 * Loads all resources used by the application.
	 */
	void app::load_resources()
	{
		// Load images
		resources->load_texture("background", "assets/images/background.png");

		// Load fonts
		resources->load_font("KenPixel Square", "assets/fonts/kenpixel_square.ttf", 12);
	}

	/**
	 * Processes user input and carries out the appropriate action(s).
	 * The result of the user input is based on the application state.
	 */
	void app::handle_input()
	{
		// Cycle through events on queue
		while(SDL_PollEvent(&event) != 0)
		{
			// Handle keyboard input
			if(event.type == SDL_KEYDOWN)
			{
				// Check the current application state
				switch(state.top())
				{
					case appstate::menu_main :
						if(event.key.keysym.sym == SDLK_ESCAPE)
						{
							// Revert to the previous application state
							state.pop();
						}
						break;

					default :
						break;
				}
			}
			// If the user quits the application, set application state to exit
			else if(event.type == SDL_QUIT)
			{
				state.push(appstate::exit);
			}
		}
	}

	/**
	 * Updates the display to reflect the current state.
	 * Re-renders all textures on screen based on the application state.
	 */
	void app::update_display()
	{
		SDL_RenderClear(renderer);

		resources->render_texture(0, 0, "background");

		SDL_RenderPresent(renderer);
	}
}
