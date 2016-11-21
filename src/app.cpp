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
		resources->load_texture("door_small", "assets/images/buildings/door_small.png");
		resources->load_texture("door_small_alt", "assets/images/buildings/door_small_alt.png");
		resources->load_texture("roof_small_front", "assets/images/buildings/roof_small_front.png");
		resources->load_texture("roof_small_back", "assets/images/buildings/roof_small_back.png");
		resources->load_texture("wall_small_base", "assets/images/buildings/wall_small_base.png");

		resources->load_texture("door_large1", "assets/images/buildings/door_large1.png");
		resources->load_texture("door_large2", "assets/images/buildings/door_large2.png");
		resources->load_texture("roof_large_front1", "assets/images/buildings/roof_large_front1.png");
		resources->load_texture("roof_large_front2", "assets/images/buildings/roof_large_front2.png");
		resources->load_texture("roof_large_front1_alt", "assets/images/buildings/roof_large_front1_alt.png");
		resources->load_texture("roof_large_front2_alt", "assets/images/buildings/roof_large_front2_alt.png");
		resources->load_texture("roof_large_middle1", "assets/images/buildings/roof_large_middle1.png");
		resources->load_texture("roof_large_middle2", "assets/images/buildings/roof_large_middle2.png");
		resources->load_texture("roof_large_back1", "assets/images/buildings/roof_large_back1.png");
		resources->load_texture("roof_large_back2", "assets/images/buildings/roof_large_back2.png");
		resources->load_texture("roof_large_back1_alt", "assets/images/buildings/roof_large_back1_alt.png");
		resources->load_texture("roof_large_back2_alt", "assets/images/buildings/roof_large_back2_alt.png");
		resources->load_texture("wall_large_middle1", "assets/images/buildings/wall_large_middle1.png");
		resources->load_texture("wall_large_middle2", "assets/images/buildings/wall_large_middle2.png");
		resources->load_texture("wall_large_base1", "assets/images/buildings/wall_large_base1.png");
		resources->load_texture("wall_large_base2", "assets/images/buildings/wall_large_base2.png");

		resources->load_texture("blacksmith_forge", "assets/images/buildings/blacksmith_forge.png");
		resources->load_texture("roof_flat_back1", "assets/images/buildings/roof_flat_back1.png");
		resources->load_texture("roof_flat_back2", "assets/images/buildings/roof_flat_back2.png");

		resources->load_texture("stall_roof1", "assets/images/buildings/stall_roof1.png");
		resources->load_texture("stall_roof2", "assets/images/buildings/stall_roof2.png");
		resources->load_texture("stall_roof1_alt", "assets/images/buildings/stall_roof1_alt.png");
		resources->load_texture("stall_roof2_alt", "assets/images/buildings/stall_roof2_alt.png");
		resources->load_texture("stall_base1", "assets/images/buildings/stall_base1.png");
		resources->load_texture("stall_base2", "assets/images/buildings/stall_base2.png");
		resources->load_texture("stall_base1_alt", "assets/images/buildings/stall_base1_alt.png");
		resources->load_texture("stall_base2_alt", "assets/images/buildings/stall_base2_alt.png");

		// Load resource images
		resources->load_texture("food_1", "assets/images/resources/food_1.png");
		resources->load_texture("food_2", "assets/images/resources/food_2.png");
		resources->load_texture("food_3", "assets/images/resources/food_3.png");
		resources->load_texture("food_harvested", "assets/images/resources/food_harvested.png");

		resources->load_texture("ore_1", "assets/images/resources/ore_1.png");
		resources->load_texture("ore_2", "assets/images/resources/ore_2.png");
		resources->load_texture("ore_3", "assets/images/resources/ore_3.png");
		resources->load_texture("ore_4", "assets/images/resources/ore_4.png");
		resources->load_texture("ore_5", "assets/images/resources/ore_5.png");
		resources->load_texture("ore_6", "assets/images/resources/ore_6.png");

		resources->load_texture("stone_1", "assets/images/resources/stone_1.png");
		resources->load_texture("stone_2", "assets/images/resources/stone_2.png");
		resources->load_texture("stone_3", "assets/images/resources/stone_3.png");
		resources->load_texture("stone_4", "assets/images/resources/stone_4.png");
		resources->load_texture("stone_5", "assets/images/resources/stone_5.png");
		resources->load_texture("stone_6", "assets/images/resources/stone_6.png");

		resources->load_texture("tree_small", "assets/images/resources/tree_small.png");
		resources->load_texture("tree_small_harvested", "assets/images/resources/tree_small_harvested.png");
		resources->load_texture("tree_top", "assets/images/resources/tree_top.png");
		resources->load_texture("tree_top_alt", "assets/images/resources/tree_top_alt.png");
		resources->load_texture("tree_top_harvested", "assets/images/resources/tree_top_harvested.png");
		resources->load_texture("tree_bottom", "assets/images/resources/tree_bottom.png");
		resources->load_texture("tree_bottom_alt", "assets/images/resources/tree_bottom_alt.png");
		resources->load_texture("tree_bottom_harvested", "assets/images/resources/tree_bottom_harvested.png");

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
								simulation_map->add_building(new building(25, 25, 1, 2, buildingtype::blacksmith));

								simulation_map->get_villagers()[0]->set_x(200);
								simulation_map->get_villagers()[0]->set_y(200);

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
							simulation_map->get_villagers()[0]->add_task(new task(tasktype::rest, {std::make_pair(x, y)}));
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

		// Render the resources
		std::vector<resource*> map_resources = simulation_map->get_resources();

		// Loop through each resource in the vector
		for(std::vector<resource*>::const_iterator iterator = map_resources.begin(); iterator != map_resources.end(); ++iterator)
		{
			int x = (*iterator)->get_x(), y = (*iterator)->get_y();

			// Display the appropriate resource according to type
			switch((*iterator)->get_type())
			{
				case resourcetype::food :
					if((*iterator)->get_harvestable() == true)
					{
						int texture_variant = std::hash<resource*>{}(*iterator) % 3;

						if(texture_variant == 0)
						{
							resources->render_texture(x - 8, y - 16, "food_1");
						}
						else if(texture_variant == 1)
						{
							resources->render_texture(x - 8, y - 16, "food_2");
						}
						else if(texture_variant == 2)
						{
							resources->render_texture(x - 8, y - 16, "food_3");
						}
					}
					else
					{
						resources->render_texture(x - 8, y - 16, "food_harvested");
					}
					break;

				case resourcetype::tree :
					{
						int texture_variant = std::hash<resource*>{}(*iterator) % 3;

						if((*iterator)->get_harvestable() == true)
						{
							if(texture_variant == 0)
							{
								resources->render_texture(x - 8, y - 16, "tree_small");
							}
							else if(texture_variant == 1)
							{
								resources->render_texture(x - 8, y - 16, "tree_bottom");
								resources->render_texture(x - 8, y - 32, "tree_top");
							}
							else if(texture_variant == 2)
							{
								resources->render_texture(x - 8, y - 16, "tree_bottom_alt");
								resources->render_texture(x - 8, y - 32, "tree_top_alt");
							}
						}
						else
						{
							if(texture_variant == 0)
							{
								resources->render_texture(x - 8, y - 16, "tree_small_harvested");
							}
							else if(texture_variant == 1 || texture_variant == 2)
							{
								resources->render_texture(x - 8, y - 16, "tree_bottom_harvested");
								resources->render_texture(x - 8, y - 32, "tree_top_harvested");
							}
						}
					}
					break;

				case resourcetype::stone :
					if((*iterator)->get_harvestable() == true)
					{
						int texture_variant = std::hash<resource*>{}(*iterator) % 6;

						if(texture_variant == 0)
						{
							resources->render_texture(x - 8, y - 16, "stone_1");
						}
						else if(texture_variant == 1)
						{
							resources->render_texture(x - 8, y - 16, "stone_2");
						}
						else if(texture_variant == 2)
						{
							resources->render_texture(x - 8, y - 16, "stone_3");
						}
						else if(texture_variant == 3)
						{
							resources->render_texture(x - 8, y - 16, "stone_4");
						}
						else if(texture_variant == 4)
						{
							resources->render_texture(x - 8, y - 16, "stone_5");
						}
						else if(texture_variant == 5)
						{
							resources->render_texture(x - 8, y - 16, "stone_6");
						}
					}
					break;

				case resourcetype::ore :
					if((*iterator)->get_harvestable() == true)
					{
						int texture_variant = std::hash<resource*>{}(*iterator) % 6;

						if(texture_variant == 0)
						{
							resources->render_texture(x - 8, y - 16, "ore_1");
						}
						else if(texture_variant == 1)
						{
							resources->render_texture(x - 8, y - 16, "ore_2");
						}
						else if(texture_variant == 2)
						{
							resources->render_texture(x - 8, y - 16, "ore_3");
						}
						else if(texture_variant == 3)
						{
							resources->render_texture(x - 8, y - 16, "ore_4");
						}
						else if(texture_variant == 4)
						{
							resources->render_texture(x - 8, y - 16, "ore_5");
						}
						else if(texture_variant == 5)
						{
							resources->render_texture(x - 8, y - 16, "ore_6");
						}
					}
					break;

				default :
					break;
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
				case buildingtype::town_hall :
					resources->render_texture(x * 16, y * 16, "wall_large_base1");
					resources->render_texture((x * 16) + 16, y * 16, "wall_large_base2");
					resources->render_texture(x * 16, (y * 16) - 16, "wall_large_middle1");
					resources->render_texture((x * 16) + 16, (y * 16) - 16, "wall_large_middle2");
					resources->render_texture(x * 16, (y * 16) - 48, "roof_large_back1");
					resources->render_texture((x * 16) + 16, (y * 16) - 48, "roof_large_back2");
					resources->render_texture(x * 16, (y * 16) - 32, "roof_large_middle1");
					resources->render_texture((x * 16) + 16, (y * 16) - 32, "roof_large_middle2");
					resources->render_texture(x * 16, (y * 16) - 16, "roof_large_front1");
					resources->render_texture((x * 16) + 16, (y * 16) - 16, "roof_large_front2");
					resources->render_texture(x * 16, y * 16, "door_large1");
					resources->render_texture((x * 16) + 16, y * 16, "door_large2");
					break;

				case buildingtype::house :
					resources->render_texture(x * 16, y * 16, "wall_large_base1");
					resources->render_texture((x * 16) + 16, y * 16, "wall_large_base2");
					resources->render_texture(x * 16, (y * 16) - 16, "wall_large_middle1");
					resources->render_texture((x * 16) + 16, (y * 16) - 16, "wall_large_middle2");
					resources->render_texture(x * 16, (y * 16) - 48, "roof_large_back1");
					resources->render_texture((x * 16) + 16, (y * 16) - 48, "roof_large_back2");
					resources->render_texture(x * 16, (y * 16) - 32, "roof_large_middle1");
					resources->render_texture((x * 16) + 16, (y * 16) - 32, "roof_large_middle2");
					resources->render_texture(x * 16, (y * 16) - 16, "roof_large_front1");
					resources->render_texture((x * 16) + 16, (y * 16) - 16, "roof_large_front2");
					resources->render_texture((x * 16) + 8, y * 16, "door_small");
					break;

				case buildingtype::house_small :
					resources->render_texture(x * 16, y * 16, "wall_small_base");
					resources->render_texture(x * 16, (y * 16) - 16, "roof_small_back");
					resources->render_texture(x * 16, (y * 16) - 8, "roof_small_front");
					break;

				case buildingtype::farmhouse :
					resources->render_texture(x * 16, y * 16, "wall_large_base1");
					resources->render_texture((x * 16) + 16, y * 16, "wall_large_base2");
					resources->render_texture(x * 16, (y * 16) - 16, "wall_large_middle1");
					resources->render_texture((x * 16) + 16, (y * 16) - 16, "wall_large_middle2");
					resources->render_texture(x * 16, (y * 16) - 48, "roof_large_back1_alt");
					resources->render_texture((x * 16) + 16, (y * 16) - 48, "roof_large_back2_alt");
					resources->render_texture(x * 16, (y * 16) - 32, "roof_large_middle1");
					resources->render_texture((x * 16) + 16, (y * 16) - 32, "roof_large_middle2");
					resources->render_texture(x * 16, (y * 16) - 16, "roof_large_front1_alt");
					resources->render_texture((x * 16) + 16, (y * 16) - 16, "roof_large_front2_alt");
					resources->render_texture((x * 16) + 8, y * 16, "door_small_alt");
					break;

				case buildingtype::blacksmith :
					resources->render_texture(x * 16, y * 16, "wall_large_base1");
					resources->render_texture((x * 16) + 16, y * 16, "wall_large_base2");
					resources->render_texture(x * 16, (y * 16) - 16, "roof_flat_back1");
					resources->render_texture((x * 16) + 16, (y * 16) - 16, "roof_flat_back2");
					resources->render_texture((x * 16) + 8, y * 16, "blacksmith_forge");
					break;

				case buildingtype::stall :
					{
						int texture_variant = std::hash<building*>{}(*iterator) % 4;

						if(texture_variant <= 1)
						{
							resources->render_texture(x * 16, y * 16, "stall_base1");
							resources->render_texture((x * 16) + 16, y * 16, "stall_base2");
						}
						else if(texture_variant <= 3)
						{
							resources->render_texture(x * 16, y * 16, "stall_base1_alt");
							resources->render_texture((x * 16) + 16, y * 16, "stall_base2_alt");
						}

						if(texture_variant == 0 || texture_variant == 2)
						{
							resources->render_texture(x * 16, (y * 16) - 16, "stall_roof1");
							resources->render_texture((x * 16) + 16, (y * 16) - 16, "stall_roof2");
						}
						else if(texture_variant == 1 || texture_variant == 3)
						{
							resources->render_texture(x * 16, (y * 16) - 16, "stall_roof1_alt");
							resources->render_texture((x * 16) + 16, (y * 16) - 16, "stall_roof2_alt");
						}
					}
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
			int texture_variant = std::hash<villager*>{}(*iterator) % 2;

			if(texture_variant == 0)
			{
				resources->render_texture(x - 12, y - 24, "villager");
			}
			else if(texture_variant == 1)
			{
				resources->render_texture(x - 12, y - 24, "villager_alt");
			}
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
