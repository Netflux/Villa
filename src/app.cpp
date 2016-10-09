#include <app.h>

namespace villa
{
	/**
	 * Constructor for the App class.
	 */
	app::app() : state(appstate::menu_main), window(nullptr), renderer(nullptr) { }

	/**
	 * Destructor for the App class.
	 */
	app::~app()
	{
		// Clear the window from memory
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		// Close PNG and TTF loaders
		//IMG_Quit();
		//TTF_Quit();

		// Quit SDL subsystems
		SDL_Quit();
	}

	/**
	 * Initializes the application (SDL and its subsystems).
	 * @return The success/failure for initialization.
	 */
	bool app::init()
	{
		// Print current status
		std::cout << "Initializing application instance..." << std::endl;

		if(SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			// Print error if SDL fails to initialize
			std::cerr << "SDL failed to initialize! SDL_Error: " << SDL_GetError() << std::endl;
			return false;
		}

		// Print success in console
		std::cout << "SDL successfully initialized." << std::endl;

		// Initialize application window
		window = SDL_CreateWindow("Villa", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);

		if(window == nullptr)
		{
			// Print error if application window fails to initialize
			std::cerr << "Application window failed to initialize! SDL_Error: " << SDL_GetError() << std::endl;
			return false;
		}

		// Print success in console
		std::cout << "Application window successfully initialized." << std::endl;

		// Initialize 2D renderer
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		if(renderer == nullptr)
		{
			// Print error if 2D renderer fails to initialize
			std::cerr << "2D Renderer failed to initialize! SDL_Error: " << SDL_GetError() << std::endl;
			return false;
		}

		// Print success in console
		std::cout << "2D Renderer successfully initialized." << std::endl;

		// Initialize PNG loader
		if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG))
		{
			// Print error if PNG loader fails to initialize
			std::cerr << "PNG Loader failed to initialize! SDL_Error: " << SDL_GetError() << std::endl;
			return false;
		}

		// Print success in console
		std::cout << "PNG Loader successfully initialized." << std::endl;

		// Initialize TTF loader
		if(TTF_Init() == -1)
		{
			// Print error if PNG loader fails to initialize
			std::cerr << "TTF Loader failed to initialize! SDL_Error: " << TTF_GetError() << std::endl;
			return false;
		}

		// Print success in console
		std::cout << "TTF Loader successfully initialized." << std::endl;

		// Print overall success in console
		std::cout << "Application instance successfully initialized." << std::endl;

		return true;
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
			// Print current status
			std::cout << "Starting application..." << std::endl;

			while(state != appstate::exit)
			{
				// Handle user input
				this->handle_input();

				// Update the display
				this->update_display();
			}
		}
	}

	/**
	 * Processes user input and carries out the appropriate action(s).
	 */
	void app::handle_input()
	{

	}

	/**
	 * Updates the display to reflect the current state.
	 * Re-renders all textures on screen.
	 */
	void app::update_display()
	{

	}
}
