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
			unsigned int accumulator = 0;
			timers.timescale = 1.0;

			// Hide the default cursor
			SDL_ShowCursor(0);

			// Loop until the user exits the application
			while(state.top() != appstate::exit)
			{
				accumulator += SDL_GetTicks() - timers.app;
				timers.app = SDL_GetTicks();

				while(accumulator >= UPDATE_TIME)
				{
					handle_input();

					// If running the simulation, update the AI
					if(state.top() == appstate::simulation)
					{
						update_simulation();
						simulation_ai->set_timescale(timers.timescale);
						simulation_ai->think();
					}

					accumulator -= UPDATE_TIME;
					timers.simulation_time += UPDATE_TIME * timers.timescale;
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
		if(renderer == nullptr || SDL_RenderSetLogicalSize(renderer, 800, 800) != 0)
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

		resources->load_texture("grave_1", "assets/images/resources/grave_1.png");
		resources->load_texture("grave_2", "assets/images/resources/grave_2.png");
		resources->load_texture("grave_3", "assets/images/resources/grave_3.png");

		// Load entity images
		resources->load_texture("villager", "assets/images/entities/villager.png");
		resources->load_texture("villager_alt", "assets/images/entities/villager_alt.png");
		resources->load_texture("villager_alt2", "assets/images/entities/villager_alt2.png");
		resources->load_texture("villager_alt3", "assets/images/entities/villager_alt3.png");

		// Enable linear texture filtering when loading UI elements
		// Not used for tile-based textures due to rendering artifacts (black lines, random gaps)
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

		// Load UI images
		resources->load_texture("cursor", "assets/images/ui/cursorHand_grey.png");
		resources->load_texture("iconMenu_brown", "assets/images/ui/iconMenu_brown.png");
		resources->load_texture("iconCross_brown", "assets/images/ui/iconCross_brown.png");
		resources->load_texture("arrowBeige_left", "assets/images/ui/arrowBeige_left.png");
		resources->load_texture("arrowBeige_right", "assets/images/ui/arrowBeige_right.png");
		resources->load_texture("buttonRound_brown", "assets/images/ui/buttonRound_brown.png");
		resources->load_texture("buttonLong_brown", "assets/images/ui/buttonLong_brown.png");
		resources->load_texture("buttonLong_brown_pressed", "assets/images/ui/buttonLong_brown_pressed.png");
		resources->load_texture("menuBar_brown", "assets/images/ui/menuBar_brown.png");

		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

		// Load fonts
		resources->load_font("KenPixel Blocks", "assets/fonts/kenpixel_blocks.ttf", 160);
		resources->load_font("KenPixel Square", "assets/fonts/kenpixel_square.ttf", 24);
		resources->load_font("KenPixel Square Medium", "assets/fonts/kenpixel_square.ttf", 20);
		resources->load_font("KenPixel Square Small", "assets/fonts/kenpixel_square.ttf", 16);
	}

	void app::load_ui()
	{
		// Load UI elements for main menu
		user_interface->add_element("Start Button", new ui_element(305, 625, 190, 49));
		user_interface->add_element("Quit Button", new ui_element(305, 700, 190, 49));

		// Load UI elements for simulation
		user_interface->add_element("Back Button", new ui_element(4, 4, 35, 35));
		user_interface->add_element("Left Button", new ui_element(733, 774, 22, 21));
		user_interface->add_element("Right Button", new ui_element(765, 774, 22, 21));
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
					case appstate::simulation :
					case appstate::simulation_end :
						if(event.key.keysym.sym == SDLK_PLUS || event.key.keysym.sym == SDLK_EQUALS)
						{
							if(timers.timescale == 0.25)
							{
								timers.timescale = 0.5;
							}
							else if(timers.timescale == 0.5)
							{
								timers.timescale = 1.0;
							}
							else if(timers.timescale == 1.0)
							{
								timers.timescale = 2.0;
							}
							else if(timers.timescale == 2.0)
							{
								timers.timescale = 4.0;
							}
						}
						else if(event.key.keysym.sym == SDLK_MINUS)
						{
							if(timers.timescale == 0.5)
							{
								timers.timescale = 0.25;
							}
							else if(timers.timescale == 1.0)
							{
								timers.timescale = 0.5;
							}
							else if(timers.timescale == 2.0)
							{
								timers.timescale = 1.0;
							}
							else if(timers.timescale == 4.0)
							{
								timers.timescale = 2.0;
							}
						}
						// No break as the simulation should inherit ESC and F11 key-presses
					case appstate::menu_main :
						if(event.key.keysym.sym == SDLK_ESCAPE)
						{
							// Revert to the previous application state
							state.pop();
						}
						else if(event.key.keysym.sym == SDLK_F11)
						{
							// Toggle fullscreen when F11 is pressed
							if(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP)
							{
								SDL_SetWindowFullscreen(window, 0);
							}
							else
							{
								SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
							}
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
								simulation_ai.reset(new ai_manager(simulation_map.get(), rng));

								timers.simulation_start = SDL_GetTicks();
							}
							else if(target == "Quit Button")
							{
								// Revert to the previous application state (quit state)
								state.pop();
							}
						}
						break;

					case appstate::simulation :
					case appstate::simulation_end :
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							std::string target = user_interface->click_at(x, y);

							if(target == "Back Button")
							{
								state.pop();
							}
							else if(target == "Left Button")
							{
								if(timers.timescale == 0.5)
								{
									timers.timescale = 0.25;
								}
								else if(timers.timescale == 1.0)
								{
									timers.timescale = 0.5;
								}
								else if(timers.timescale == 2.0)
								{
									timers.timescale = 1.0;
								}
								else if(timers.timescale == 4.0)
								{
									timers.timescale = 2.0;
								}
							}
							else if(target == "Right Button")
							{
								if(timers.timescale == 0.25)
								{
									timers.timescale = 0.5;
								}
								else if(timers.timescale == 0.5)
								{
									timers.timescale = 1.0;
								}
								else if(timers.timescale == 1.0)
								{
									timers.timescale = 2.0;
								}
								else if(timers.timescale == 2.0)
								{
									timers.timescale = 4.0;
								}
							}
							else
							{
								bool found = false;

								std::vector<villager*> villagers = simulation_map->get_villagers();

								// Loop through each villager in the vector
								for(std::vector<villager*>::const_iterator iterator = villagers.begin(); iterator != villagers.end(); ++iterator)
								{
									if((*iterator)->is_at(x, y))
									{
										switch((*iterator)->get_task()->get_type())
										{
											case tasktype::harvest :
												std::cout << "Current Task: Harvest" << std::endl;break;
											case tasktype::idle :
												std::cout << "Current Task: Idle" << std::endl;break;
											case tasktype::move :
												std::cout << "Current Task: Move" << std::endl;break;
											case tasktype::rest :
												std::cout << "Current Task: Rest . Duration: " << (int)((*iterator)->get_task()->get_data().time) << "ms" << std::endl;break;
											case tasktype::store_item :
												std::cout << "Current Task: Store Item" << std::endl;break;
											case tasktype::take_item :
												std::cout << "Current Task: Take Item" << std::endl;break;
											case tasktype::build :
												std::cout << "Current Task: Build" << std::endl;break;
										}
										std::cout << "Villager X: " << (*iterator)->get_x() << " . Villager Y: " << (*iterator)->get_y() << " . Target X: " << (*iterator)->get_task()->get_data().target_coords.first << " . Target Y: " << (*iterator)->get_task()->get_data().target_coords.second << std::endl;
										std::cout << "Task Count: " << (*iterator)->get_task_count() << " . Health: " << (*iterator)->get_health() << " . Fatigue: " << (*iterator)->get_fatigue() << " . Hunger: " << (*iterator)->get_hunger() << " . Thirst: " << (*iterator)->get_thirst() << " . Item Count: " << (*iterator)->get_inventory()->get_item_count() << "\n" << std::endl;
										found = true;
										break;
									}
								}

								if(found == false)
								{
									std::vector<building*> buildings = simulation_map->get_buildings();

									// Loop through each building in the vector
									for(std::vector<building*>::const_iterator iterator = buildings.begin(); iterator != buildings.end(); ++iterator)
									{
										if((*iterator)->is_at(x, y))
										{
											std::cout << "Building X: " << (*iterator)->get_x() << " . Building Y: " << (*iterator)->get_y() << " . Item Count: " << (*iterator)->get_inventory()->get_item_count() << "\n" << std::endl;
											found = true;
											break;
										}
									}
								}

								if(found == false)
								{
									std::vector<resource*> resources = simulation_map->get_resources();

									// Loop through each building in the vector
									for(std::vector<resource*>::const_iterator iterator = resources.begin(); iterator != resources.end(); ++iterator)
									{
										if((*iterator)->is_at(x, y))
										{
											std::cout << "Resource X: " << (*iterator)->get_x() << " . Resource Y: " << (*iterator)->get_y() << " . Item Count: " << (*iterator)->get_inventory()->get_item_count() << "\n" << std::endl;
											break;
										}
									}
								}
							}
						}
						else if(event.button.button == SDL_BUTTON_RIGHT)
						{
							if(simulation_map->get_villagers().size() > 0)
							{
								simulation_map->get_villagers()[0]->add_task(new task(tasktype::rest, taskdata(std::make_pair(x, y), 500)));
								//simulation_map->get_villagers()[0]->set_health(0);
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
	 * Updates the entities within the simulation.
	 * Handles time-based state changes.
	 */
	void app::update_simulation()
	{
		std::vector<villager*> villagers = simulation_map->get_villagers();

		// If no villagers remain, set the simulation to the end state
		if(villagers.empty())
		{
			timers.simulation_end = SDL_GetTicks();

			state.pop();
			state.push(appstate::simulation_end);
		}
		else
		{
			bool update_villager_health = false, update_villager_health_regen = false, update_villager_needs = false;

			if(timers.simulation_time > timers.villager_health + 1000)
			{
				timers.villager_health = timers.simulation_time;
				update_villager_health = true;
			}

			if(timers.simulation_time > timers.villager_health_regen + 30000)
			{
				timers.villager_health_regen = timers.simulation_time;
				update_villager_health_regen = true;
			}

			if(timers.simulation_time > timers.villager_needs + 15000)
			{
				timers.villager_needs = timers.simulation_time;
				update_villager_needs = true;
			}

			// Loop through each villager in the vector
			for(std::vector<villager*>::iterator iterator = villagers.begin(); iterator != villagers.end(); ++iterator)
			{
				// If the villager loses all health, create a grave resource containing all of its items
				if((*iterator)->get_health() <= 0)
				{
					resource* target = new resource((*iterator)->get_x(), (*iterator)->get_y(), resourcetype::grave);
					inventory* villager_inv = (*iterator)->get_inventory();

					while(villager_inv->get_item_count() > 0)
					{
						std::unique_ptr<item> target_item = villager_inv->take_item(villager_inv->get_items()[0]);

						if(target_item.get() != nullptr)
						{
							target->get_inventory()->add_item(std::move(target_item));
							target->set_harvestable(true);
						}
					}

					simulation_map->remove_villager(*iterator);
					simulation_map->add_resource(target);
				}
				else
				{
					if((*iterator)->get_health() > 100)
					{
						(*iterator)->set_health(100);
					}

					if((*iterator)->get_fatigue() < 0)
					{
						(*iterator)->set_fatigue(0);
					}

					if((*iterator)->get_hunger() < 0)
					{
						(*iterator)->set_hunger(0);
					}

					if((*iterator)->get_thirst() < 0)
					{
						(*iterator)->set_thirst(0);
					}

					// Reduce villager health by 1 every second if fatigue, hunger or thirst is above 100
					if(update_villager_health == true)
					{
						if((*iterator)->get_fatigue() >= 100 || (*iterator)->get_hunger() >= 100 || (*iterator)->get_thirst() >= 100)
						{
							(*iterator)->set_health((*iterator)->get_health() - 1);
						}
					}

					// Increase villager health by 1 every 30 seconds
					if(update_villager_health_regen == true)
					{
						(*iterator)->set_health((*iterator)->get_health() + 1);
					}

					// Increase villager fatigue, hunger and thirst by 1 every 15 seconds
					if(update_villager_needs == true)
					{
						(*iterator)->set_fatigue((*iterator)->get_fatigue() + 1);
						(*iterator)->set_hunger((*iterator)->get_hunger() + 1);
						(*iterator)->set_thirst((*iterator)->get_thirst() + 1);
					}
				}
			}

			std::vector<resource*> resources = simulation_map->get_resources();

			// Loop through each resource in the vector
			for(std::vector<resource*>::iterator iterator = resources.begin(); iterator != resources.end(); ++iterator)
			{
				// If the resource is on unpathable terrain, set it as unharvestable
				if(simulation_map->get_tile_at((*iterator)->get_x() / 16, (*iterator)->get_y() / 16)->get_pathable() == false)
				{
					(*iterator)->set_harvestable(false);
					(*iterator)->set_harvestable_time(0);
				}
				else
				{
					// Once the time has passed the resource timeout duration, set it as harvestable
					// Also resets the inventory with a new set of items (excluding graves)
					if((*iterator)->get_harvestable() == false && timers.simulation_time > (*iterator)->get_harvestable_time() && (*iterator)->get_harvestable_time() != 0)
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
									(*iterator)->get_inventory()->add_item(new item(itemtype::stone));
									(*iterator)->get_inventory()->add_item(new item(itemtype::ore));
								}
								break;

							default :
								break;
						}
					}

					// If all items have been harvested from the resource, set its harvestable state to false
					if((*iterator)->get_inventory()->get_item_count() == 0 && (*iterator)->get_harvestable() == true)
					{
						(*iterator)->set_harvestable(false);

						if((*iterator)->get_type() == resourcetype::grave)
						{
							(*iterator)->set_harvestable_time(0);
						}
						else
						{
							(*iterator)->set_harvestable_time(timers.simulation_time + 120000);
						}
					}
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
			case appstate::simulation_end :
				display_simulation();
				break;

			default :
				break;
		}

		// Get coords of mouse
		int x, y;
		SDL_GetMouseState(&x, &y);

		// Render the custom cursor at the mouse position
		resources->render_texture(x - 4, y - 4, "cursor");

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
			resources->render_texture(305, 629, "buttonLong_brown_pressed");
			resources->render_text(357, 632, "Start", "KenPixel Square", 24, {224, 224, 224});
		}
		else
		{
			resources->render_texture(305, 625, "buttonLong_brown");
			resources->render_text(357, 628, "Start", "KenPixel Square", 24, {224, 224, 224});
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

				case resourcetype::grave :
					{
						int texture_variant = std::hash<resource*>{}(*iterator) % 3;

						if(texture_variant == 0)
						{
							resources->render_texture(x - 8, y - 16, "grave_1");
						}
						else if(texture_variant == 1)
						{
							resources->render_texture(x - 8, y - 16, "grave_2");
						}
						else if(texture_variant == 2)
						{
							resources->render_texture(x - 8, y - 16, "grave_3");
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
					resources->render_texture(x, y - 16, "wall_large_base1");
					resources->render_texture(x + 16, y - 16, "wall_large_base2");
					resources->render_texture(x, y - 32, "wall_large_middle1");
					resources->render_texture(x + 16, y - 32, "wall_large_middle2");
					resources->render_texture(x, y - 64, "roof_large_back1");
					resources->render_texture(x + 16, y - 64, "roof_large_back2");
					resources->render_texture(x, y - 48, "roof_large_middle1");
					resources->render_texture(x + 16, y - 48, "roof_large_middle2");
					resources->render_texture(x, y - 32, "roof_large_front1");
					resources->render_texture(x + 16, y - 32, "roof_large_front2");
					resources->render_texture(x, y - 16, "door_large1");
					resources->render_texture(x + 16, y - 16, "door_large2");
					break;

				case buildingtype::house :
					resources->render_texture(x, y - 16, "wall_large_base1");
					resources->render_texture(x + 16, y - 16, "wall_large_base2");
					resources->render_texture(x, y - 32, "wall_large_middle1");
					resources->render_texture(x + 16, y - 32, "wall_large_middle2");
					resources->render_texture(x, y - 64, "roof_large_back1");
					resources->render_texture(x + 16, y - 64, "roof_large_back2");
					resources->render_texture(x, y - 48, "roof_large_middle1");
					resources->render_texture(x + 16, y - 48, "roof_large_middle2");
					resources->render_texture(x, y - 32, "roof_large_front1");
					resources->render_texture(x + 16, y - 32, "roof_large_front2");
					resources->render_texture(x + 8, y - 16, "door_small");
					break;

				case buildingtype::house_small :
					resources->render_texture(x, y - 16, "wall_small_base");
					resources->render_texture(x, y - 32, "roof_small_back");
					resources->render_texture(x, y - 24, "roof_small_front");
					break;

				case buildingtype::farmhouse :
					resources->render_texture(x, y - 16, "wall_large_base1");
					resources->render_texture(x + 16, y - 16, "wall_large_base2");
					resources->render_texture(x, y - 32, "wall_large_middle1");
					resources->render_texture(x + 16, y - 32, "wall_large_middle2");
					resources->render_texture(x, y - 64, "roof_large_back1_alt");
					resources->render_texture(x + 16, y - 64, "roof_large_back2_alt");
					resources->render_texture(x, y - 48, "roof_large_middle1");
					resources->render_texture(x + 16, y - 48, "roof_large_middle2");
					resources->render_texture(x, y - 32, "roof_large_front1_alt");
					resources->render_texture(x + 16, y - 32, "roof_large_front2_alt");
					resources->render_texture(x + 8, y - 16, "door_small_alt");
					break;

				case buildingtype::blacksmith :
					resources->render_texture(x, y - 16, "wall_large_base1");
					resources->render_texture(x + 16, y - 16, "wall_large_base2");
					resources->render_texture(x, y - 32, "roof_flat_back1");
					resources->render_texture(x + 16, y - 32, "roof_flat_back2");
					resources->render_texture(x + 8, y - 16, "blacksmith_forge");
					break;

				case buildingtype::stall :
					{
						int texture_variant = std::hash<building*>{}(*iterator) % 4;

						if(texture_variant <= 1)
						{
							resources->render_texture(x, y - 16, "stall_base1");
							resources->render_texture(x + 16, y - 16, "stall_base2");
						}
						else if(texture_variant <= 3)
						{
							resources->render_texture(x, y - 16, "stall_base1_alt");
							resources->render_texture(x + 16, y - 16, "stall_base2_alt");
						}

						if(texture_variant == 0 || texture_variant == 2)
						{
							resources->render_texture(x, y - 32, "stall_roof1");
							resources->render_texture(x + 16, y - 32, "stall_roof2");
						}
						else if(texture_variant == 1 || texture_variant == 3)
						{
							resources->render_texture(x, y - 32, "stall_roof1_alt");
							resources->render_texture(x + 16, y - 32, "stall_roof2_alt");
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
			int texture_variant = std::hash<villager*>{}(*iterator) % 40;

			if(texture_variant <= 10)
			{
				resources->render_texture(x - 12, y - 24, "villager");
			}
			else if(texture_variant <= 20)
			{
				resources->render_texture(x - 12, y - 24, "villager_alt");
			}
			else if(texture_variant <= 30)
			{
				resources->render_texture(x - 12, y - 24, "villager_alt2");
			}
			else if(texture_variant < 40)
			{
				resources->render_texture(x - 12, y - 24, "villager_alt3");
			}
		}

		// Render the UI
		std::stringstream ss;
		unsigned int simulation_time = 0;

		if(state.top() == appstate::simulation)
		{
			simulation_time = timers.simulation_time;
		}
		else if(state.top() == appstate::simulation_end)
		{
			simulation_time = timers.simulation_end - timers.simulation_start;
		}

		ss << std::setfill('0') << std::setw(2) << (int)(simulation_time / (1000 * 60 * 60)) % 60 << ":" << std::setfill('0') << std::setw(2) << (int)(simulation_time / (1000 * 60)) % 60 << ":" << std::setfill('0') << std::setw(2) << (int)(simulation_time / 1000) % 60;

		resources->render_texture(305, -10, "buttonLong_brown_pressed");
		resources->render_text(341, -4, ss.str(), "KenPixel Square", 24, {224, 224, 224});

		resources->render_texture(4, 4, "buttonRound_brown");
		resources->render_texture(14, 14, "iconCross_brown");

		resources->render_texture(4, 765, "menuBar_brown");

		resources->render_texture(8, 769, "villager");
		resources->render_text(32, 769, ">" + std::to_string(simulation_map->get_villagers().size()), "KenPixel Square Medium", 20, {224, 224, 224});

		resources->render_texture(83, 782, "wall_small_base");
		resources->render_texture(83, 766, "roof_small_back");
		resources->render_texture(83, 774, "roof_small_front");
		resources->render_text(107, 769, ">" + std::to_string(simulation_map->get_buildings().size()), "KenPixel Square Medium", 20, {224, 224, 224});

		resources->render_texture(158, 775, "food_1");
		resources->render_text(182, 769, ">" + std::to_string(simulation_map->get_resource_count(resourcetype::food)), "KenPixel Square Medium", 20, {224, 224, 224});

		resources->render_texture(233, 777, "tree_small");
		resources->render_text(258, 769, ">" + std::to_string(simulation_map->get_resource_count(resourcetype::tree)), "KenPixel Square Medium", 20, {224, 224, 224});

		resources->render_texture(308, 774, "stone_1");
		resources->render_text(332, 769, ">" + std::to_string(simulation_map->get_resource_count(resourcetype::stone)), "KenPixel Square Medium", 20, {224, 224, 224});

		resources->render_texture(383, 774, "ore_1");
		resources->render_text(407, 769, ">" + std::to_string(simulation_map->get_resource_count(resourcetype::ore)), "KenPixel Square Medium", 20, {224, 224, 224});

		resources->render_texture(733, 774, "arrowBeige_left");
		resources->render_texture(765, 774, "arrowBeige_right");
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
