#include "app.hpp"

namespace villa
{
	/**
	 * Constructor for the App class.
	 */
	app::app() : app_time(0), window(nullptr), renderer(nullptr)
	{
		// Set initial state to exit
		state.push(appstate::exit);

		// Seed the random number generator
		rng.seed(time(nullptr));
	}

	/**
	 * Destructor for the App class.
	 */
	app::~app()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		resources->unload_resources();

		IMG_Quit();
		TTF_Quit();

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

			// Initialize time-related variables
			const int UPDATE_TIME = 1000 / 60;
			unsigned int accumulator = 0, simulation_time = 0;

			// Loop until the user exits the application
			while(state.top() != appstate::exit)
			{
				accumulator += SDL_GetTicks() - app_time;
				app_time = SDL_GetTicks();

				while(accumulator >= UPDATE_TIME)
				{
					handle_input();

					// If running the simulation, update the AI
					if(state.top() == appstate::simulation)
					{
						update_simulation();
						simulation_ai->think();
					}

					accumulator -= UPDATE_TIME;
					simulation_time += UPDATE_TIME;
				}

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
		// Load background image
		resources->load_texture("background", "assets/images/background.png");

		// Load tile images
		resources->load_texture("grass_c", "assets/images/tiles/grass_c.png");

		resources->load_texture("water_nw", "assets/images/tiles/water_nw.png");
		resources->load_texture("water_n", "assets/images/tiles/water_n.png");
		resources->load_texture("water_ne", "assets/images/tiles/water_ne.png");
		resources->load_texture("water_w", "assets/images/tiles/water_w.png");
		resources->load_texture("water_c", "assets/images/tiles/water_c.png");
		resources->load_texture("water_e", "assets/images/tiles/water_e.png");
		resources->load_texture("water_sw", "assets/images/tiles/water_sw.png");
		resources->load_texture("water_s", "assets/images/tiles/water_s.png");
		resources->load_texture("water_se", "assets/images/tiles/water_se.png");

		resources->load_texture("water_corner_nw", "assets/images/tiles/water_corner_nw.png");
		resources->load_texture("water_corner_ne", "assets/images/tiles/water_corner_ne.png");
		resources->load_texture("water_corner_sw", "assets/images/tiles/water_corner_sw.png");
		resources->load_texture("water_corner_se", "assets/images/tiles/water_corner_se.png");

		resources->load_texture("dirt_nw", "assets/images/tiles/dirt_nw.png");
		resources->load_texture("dirt_n", "assets/images/tiles/dirt_n.png");
		resources->load_texture("dirt_ne", "assets/images/tiles/dirt_ne.png");
		resources->load_texture("dirt_w", "assets/images/tiles/dirt_w.png");
		resources->load_texture("dirt_c", "assets/images/tiles/dirt_c.png");
		resources->load_texture("dirt_e", "assets/images/tiles/dirt_e.png");
		resources->load_texture("dirt_sw", "assets/images/tiles/dirt_sw.png");
		resources->load_texture("dirt_s", "assets/images/tiles/dirt_s.png");
		resources->load_texture("dirt_se", "assets/images/tiles/dirt_se.png");

		resources->load_texture("dirt_corner_nw", "assets/images/tiles/dirt_corner_nw.png");
		resources->load_texture("dirt_corner_ne", "assets/images/tiles/dirt_corner_ne.png");
		resources->load_texture("dirt_corner_sw", "assets/images/tiles/dirt_corner_sw.png");
		resources->load_texture("dirt_corner_se", "assets/images/tiles/dirt_corner_se.png");

		resources->load_texture("sand_nw", "assets/images/tiles/sand_nw.png");
		resources->load_texture("sand_n", "assets/images/tiles/sand_n.png");
		resources->load_texture("sand_ne", "assets/images/tiles/sand_ne.png");
		resources->load_texture("sand_w", "assets/images/tiles/sand_w.png");
		resources->load_texture("sand_c", "assets/images/tiles/sand_c.png");
		resources->load_texture("sand_e", "assets/images/tiles/sand_e.png");
		resources->load_texture("sand_sw", "assets/images/tiles/sand_sw.png");
		resources->load_texture("sand_s", "assets/images/tiles/sand_s.png");
		resources->load_texture("sand_se", "assets/images/tiles/sand_se.png");

		resources->load_texture("sand_corner_nw", "assets/images/tiles/sand_corner_nw.png");
		resources->load_texture("sand_corner_ne", "assets/images/tiles/sand_corner_ne.png");
		resources->load_texture("sand_corner_sw", "assets/images/tiles/sand_corner_sw.png");
		resources->load_texture("sand_corner_se", "assets/images/tiles/sand_corner_se.png");

		resources->load_texture("stone_nw", "assets/images/tiles/stone_nw.png");
		resources->load_texture("stone_n", "assets/images/tiles/stone_n.png");
		resources->load_texture("stone_ne", "assets/images/tiles/stone_ne.png");
		resources->load_texture("stone_w", "assets/images/tiles/stone_w.png");
		resources->load_texture("stone_c", "assets/images/tiles/stone_c.png");
		resources->load_texture("stone_e", "assets/images/tiles/stone_e.png");
		resources->load_texture("stone_sw", "assets/images/tiles/stone_sw.png");
		resources->load_texture("stone_s", "assets/images/tiles/stone_s.png");
		resources->load_texture("stone_se", "assets/images/tiles/stone_se.png");

		resources->load_texture("stone_corner_nw", "assets/images/tiles/stone_corner_nw.png");
		resources->load_texture("stone_corner_ne", "assets/images/tiles/stone_corner_ne.png");
		resources->load_texture("stone_corner_sw", "assets/images/tiles/stone_corner_sw.png");
		resources->load_texture("stone_corner_se", "assets/images/tiles/stone_corner_se.png");

		// Load building images
		resources->load_texture("roof_small_front", "assets/images/buildings/roof_small_front.png");
		resources->load_texture("roof_small_back", "assets/images/buildings/roof_small_back.png");
		resources->load_texture("wall_small_base", "assets/images/buildings/wall_small_base.png");

		// Load entity images
		resources->load_texture("villager", "assets/images/entities/villager.png");
		resources->load_texture("villager_alt", "assets/images/entities/villager_alt.png");
		resources->load_texture("villager_blacksmith", "assets/images/entities/villager_blacksmith.png");
		resources->load_texture("villager_miner", "assets/images/entities/villager_miner.png");


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

					case appstate::simulation :
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
								state.push(appstate::simulation);

								simulation_map.reset(new map(rng));

								simulation_map->add_villager(new villager());
								simulation_map->add_building(new building(25, 25, 1, 2, buildingtype::house_small));

								simulation_map->get_villagers()[0]->set_x(400);
								simulation_map->get_villagers()[0]->set_y(400);

								simulation_ai.reset(new ai_manager(simulation_map.get(), rng));
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

					case appstate::simulation :
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							simulation_map->get_villagers()[0]->add_task(new task(tasktype::move, {std::make_pair(x, y)}));
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
	 * Updates the entities within the simulation.
	 * Handles time-based state changes.
	 */
	void app::update_simulation()
	{
		std::vector<resource*> resources = simulation_map->get_resources();

		// Loop through each resource in the vector
		for(std::vector<resource*>::const_iterator iterator = resources.begin(); iterator != resources.end(); ++iterator)
		{
			// Once the time has passed the resource timeout duration, set it as harvestable
			// Also resets the inventory with a new set of items
			if((*iterator)->get_harvestable() == false && SDL_GetTicks() > (*iterator)->get_harvestable_time())
			{
				(*iterator)->set_harvestable(true);

				// Scale down random number while preserving uniform distribution
				std::uniform_int_distribution<int> distribution(1, 5);
				int quantity = distribution(rng);

				switch((*iterator)->get_type())
				{
					case resourcetype::water :
						for(int i = 0; i < quantity; ++i)
						{
							(*iterator)->get_inventory()->add_item(new item(itemtype::water));
						}
						break;

					case resourcetype::food :
						for(int i = 0; i < quantity; ++i)
						{
							(*iterator)->get_inventory()->add_item(new item(itemtype::food));
						}
						break;

					case resourcetype::tree :
						for(int i = 0; i < quantity; ++i)
						{
							(*iterator)->get_inventory()->add_item(new item(itemtype::lumber));
						}
						break;

					case resourcetype::stone :
						for(int i = 0; i < quantity; ++i)
						{
							(*iterator)->get_inventory()->add_item(new item(itemtype::stone));
						}
						break;

					case resourcetype::ore :
						for(int i = 0; i < quantity; ++i)
						{
							(*iterator)->get_inventory()->add_item(new item(itemtype::ore));
						}
						break;
				}
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

			case appstate::simulation :
				display_simulation();
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

	/**
	 * Displays the running simulation of the application.
	 */
	void app::display_simulation()
	{
		// Render the map
		for(int i = 0; i < 50; ++i)
		{
			for(int j = 0; j < 50; ++j)
			{
				tile* target = simulation_map->get_tile_at(i, j);

				if(target->get_type() == tiletype::water)
				{
					display_simulation_tile(i, j, tiletype::water, "water");
				}
				else
				{
					// Render a grass tile as the default underlying texture
					// We do not need an if statement for grass tiles due to this
					resources->render_texture(i * 16, j * 16, "grass_c");

					if(target->get_type() == tiletype::dirt)
					{
						display_simulation_tile(i, j, tiletype::dirt, "dirt");
					}
					else if(target->get_type() == tiletype::sand)
					{
						display_simulation_tile(i, j, tiletype::sand, "sand");
					}
				}
			}
		}

		// Render the buildings
		std::vector<building*> buildings = simulation_map->get_buildings();

		// Loop through each building in the vector
		for(std::vector<building*>::const_iterator iterator = buildings.begin(); iterator != buildings.end(); ++iterator)
		{
			int x = (*iterator)->get_x(), y = (*iterator)->get_y();

			// Display the appropriate building according to type
			switch((*iterator)->get_type())
			{
				case buildingtype::house_small :
					resources->render_texture(x * 16, y * 16, "wall_small_base");
					resources->render_texture(x * 16, (y * 16) - 16, "roof_small_back");
					resources->render_texture(x * 16, (y * 16) - 8, "roof_small_front");
					break;

				default :
					break;
			}
		}

		// Render the villagers
		std::vector<villager*> villagers = simulation_map->get_villagers();

		// Loop through each villager in the vector
		for(std::vector<villager*>::const_iterator iterator = villagers.begin(); iterator != villagers.end(); ++iterator)
		{
			int x = (*iterator)->get_x(), y = (*iterator)->get_y();

			resources->render_texture(x - 12, y - 24, "villager");
		}
	}

	/**
	 * Displays the specified tile in the simulation.
	 * Textures must be named using the following formats :
	 * 1. (filename)_(direction).png
	 * 2. (filename)_corner_(direction).png
	 *
	 * Directions are declared as the first letters of north, south, east or west.
	 * Two combinations of letters can be used to specify diagonal directions.
	 * Example : texture_nw.png, texture_corner_nw.png
	 *
	 * @param x - The x-coord (grid) of the target tile.
	 * @param y - The y-coord (grid) of the target tile.
	 * @param type - The tile type to search for.
	 * @param name - The name of the texture (without direction).
	 */
	void app::display_simulation_tile(int x, int y, tiletype type, std::string name)
	{
		// We store the directions which have the specified tile type.
		// Allows for custom textures to be used for tile edges.
		int direction = 0;

		// Check the tile north of current tile
		if((y - 1) >= 0 && simulation_map->get_tile_at(x, y - 1)->get_type() != type)
		{
			direction += 1;
		}

		// Check the tile west of current tile
		if((x - 1) >= 0 && simulation_map->get_tile_at(x - 1, y)->get_type() != type)
		{
			direction += 2;
		}

		// Check the tile east of current tile
		if((x + 1) < 50 && simulation_map->get_tile_at(x + 1, y)->get_type() != type)
		{
			direction += 4;
		}

		// Check the tile south of current tile
		if((y + 1) < 50 && simulation_map->get_tile_at(x, y + 1)->get_type() != type)
		{
			direction += 8;
		}

		switch(direction)
		{
			case 1 : // 1 tile (north)
				resources->render_texture(x * 16, y * 16, name + "_n");
				break;

			case 2 : // 1 tile (west)
				resources->render_texture(x * 16, y * 16, name + "_w");
				break;

			case 3 : // 2 tiles (north, west)
				resources->render_texture(x * 16, y * 16, name + "_nw");
				break;

			case 4 : // 1 tile (east)
				resources->render_texture(x * 16, y * 16, name + "_e");
				break;

			case 5 : // 2 tiles (north, east)
				resources->render_texture(x * 16, y * 16, name + "_ne");
				break;

			case 8 : // 1 tile (south)
				resources->render_texture(x * 16, y * 16, name + "_s");
				break;

			case 10 : // 2 tiles (south, west)
				resources->render_texture(x * 16, y * 16, name + "_sw");
				break;

			case 12 : // 2 tiles (south, east)
				resources->render_texture(x * 16, y * 16, name + "_se");
				break;

			default :
				// Check the tile in the northwest corner
				if((x - 1) >= 0 && (y - 1) >= 0 && simulation_map->get_tile_at(x - 1, y - 1)->get_type() != type)
				{
					resources->render_texture(x * 16, y * 16, name + "_corner_se");
				}
				// Check the tile in the northeast corner
				else if((x - 1) >= 0 && (y + 1) < 50 && simulation_map->get_tile_at(x - 1, y + 1)->get_type() != type)
				{
					resources->render_texture(x * 16, y * 16, name + "_corner_ne");
				}
				// Check the tile in the southwest corner
				else if((x + 1) < 50 && (y - 1) >= 0 && simulation_map->get_tile_at(x + 1, y - 1)->get_type() != type)
				{
					resources->render_texture(x * 16, y * 16, name + "_corner_nw");
				}
				// Check the tile in the southeast corner
				else if((x + 1) < 50 && (y + 1) < 50 && simulation_map->get_tile_at(x + 1, y + 1)->get_type() != type)
				{
					resources->render_texture(x * 16, y * 16, name + "_corner_sw");
				}
				else
				{
					resources->render_texture(x * 16, y * 16, name + "_c");
				}
				break;
		}
	}
}
