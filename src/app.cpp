#include "app.hpp"

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
		if(init())
		{
			std::cout << "Starting application..." << std::endl;

			// Load resources/UI and push initial application state
			load_resources();
			load_ui();
			state.push(appstate::menu_main);

			// Loop until the user exits the application
			while(state.top() != appstate::exit)
			{
				handle_input();
				update_display();
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

		// Initialize the UI manager
		user_interface.reset(new ui_manager());

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

		// Load UI images
		resources->load_texture("buttonLong_brown", "assets/images/ui/buttonLong_brown.png");
		resources->load_texture("buttonLong_brown_pressed", "assets/images/ui/buttonLong_brown_pressed.png");

		// Load fonts
		resources->load_font("KenPixel Blocks", "assets/fonts/kenpixel_blocks.ttf", 160);
		resources->load_font("KenPixel Square", "assets/fonts/kenpixel_square.ttf", 24);
	}

	void app::load_ui()
	{
		// Load UI elements for main menu
		user_interface->add_element("Start Button", new ui_element(305, 550, 190, 49));
		user_interface->add_element("Options Button", new ui_element(305, 625, 190, 49));
		user_interface->add_element("Quit Button", new ui_element(305, 700, 190, 49));
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
							// Revert to the previous application state (quit state)
							state.pop();
						}
						break;

					default :
						break;
				}
			}
			// Handle mouse press input
			else if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				// Get coords of mouse
				int x, y;
				SDL_GetMouseState(&x, &y);

				// Simulate click on user interface
				user_interface->click_at(x, y);
			}
			// Handle mouse release input
			else if(event.type == SDL_MOUSEBUTTONUP)
			{
				// Get coords of mouse
				int x, y;
				SDL_GetMouseState(&x, &y);

				// Check the current application state
				switch(state.top())
				{
					case appstate::menu_main :
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							std::string target = user_interface->click_at(x, y);

							if(target == "Start Button")
							{

							}
							else if(target == "Options Button")
							{

							}
							else if(target == "Quit Button")
							{
								// Revert to the previous application state (quit state)
								state.pop();
							}
						}
						break;

					default :
						break;
				}

				// Reset the currently clicked UI element
				user_interface->reset_element_clicked();
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

		// Check the current application state
		switch(state.top())
		{
			case appstate::menu_main :
				display_menu_main();
				break;

			default :
				break;
		}

		SDL_RenderPresent(renderer);
	}

	/**
	 * Displays the main menu of the application.
	 */
	void app::display_menu_main()
	{
		// Render menu background
		resources->render_texture(0, 0, "background");

		// Render title heading and sub-heading
		resources->render_text(110, 0, "Villa", "KenPixel Blocks", 160, {64, 64, 64});
		resources->render_text(168, 211, "A Humble Village AI Simulation", "KenPixel Square", 24, {64, 64, 64});

		// Render menu buttons and button text
		if(user_interface->get_element_clicked() == "Start Button")
		{
			resources->render_texture(305, 554, "buttonLong_brown_pressed");
			resources->render_text(357, 557, "Start", "KenPixel Square", 24, {224, 224, 224});
		}
		else
		{
			resources->render_texture(305, 550, "buttonLong_brown");
			resources->render_text(357, 553, "Start", "KenPixel Square", 24, {224, 224, 224});
		}

		if(user_interface->get_element_clicked() == "Options Button")
		{
			resources->render_texture(305, 629, "buttonLong_brown_pressed");
			resources->render_text(342, 632, "Options", "KenPixel Square", 24, {224, 224, 224});
		}
		else
		{
			resources->render_texture(305, 625, "buttonLong_brown");
			resources->render_text(342, 628, "Options", "KenPixel Square", 24, {224, 224, 224});
		}

		if(user_interface->get_element_clicked() == "Quit Button")
		{
			resources->render_texture(305, 704, "buttonLong_brown_pressed");
			resources->render_text(369, 707, "Quit", "KenPixel Square", 24, {224, 224, 224});
		}
		else
		{
			resources->render_texture(305, 700, "buttonLong_brown");
			resources->render_text(369, 703, "Quit", "KenPixel Square", 24, {224, 224, 224});
		}
	}
}