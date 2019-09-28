#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"

#include "SDL/include/SDL.h"


j1Window::j1Window() : j1Module()
{
	window = NULL;
	screen_surface = NULL;
	name.create("window");
}

// Destructor
j1Window::~j1Window()
{
}

// Called before render is available
bool j1Window::Awake(pugi::xml_node& module_node)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_SHOWN;

		game_title = module_node.child("title").attribute("game_title").value();
		
		width = module_node.child("dimensions").attribute("width").as_uint();
		height = module_node.child("dimensions").attribute("height").as_uint();
		scale = module_node.child("dimensions").attribute("scale").as_uint();

		full_screen = module_node.child("options").attribute("full_screen").as_bool();
		borderless = module_node.child("options").attribute("borderless").as_bool();
		resizable = module_node.child("options").attribute("resizable").as_bool();
		full_screen_windowed = module_node.child("options").attribute("full_screen_windowed").as_bool();

		if(full_screen)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(borderless)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(resizable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(full_screen_windowed)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(game_title.GetString(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);

		}
	}

	return ret;
}

// Called before quitting
bool j1Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Set new window title
void j1Window::SetTitle(const char* new_title)
{
	SDL_SetWindowTitle(window, new_title);
}

void j1Window::GetWindowSize(uint& width, uint& height) const
{
	width = this->width;
	height = this->height;
}

uint j1Window::GetScale() const
{
	return scale;
}

bool j1Window::Load(pugi::xml_node& module_node)
{
	return(true);
}

bool j1Window::Save(pugi::xml_node& module_node) const
{
	return(true);
}