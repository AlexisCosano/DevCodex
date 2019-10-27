#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Collisions.h"
#include "j1Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	LoadMap(map_to_load);
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	App->render->camera.x = ((-App->player->player_position.x * App->win->GetScale()) + (150 * App->win->GetScale()));
	App->render->camera.y = ((-App->player->player_position.y * App->win->GetScale()) + (240 * App->win->GetScale()));

	if (App->render->camera.x > 0 * (int)App->win->GetScale())
	{
		App->render->camera.x = 0 * (int)App->win->GetScale();
	}

	if (App->render->camera.x < -2080 * (int)App->win->GetScale())
	{
		App->render->camera.x = -2080 * (int)App->win->GetScale();
	}

	if (App->render->camera.y > 32 * (int)App->win->GetScale())
	{
		App->render->camera.y = 32 * (int)App->win->GetScale();
	}

	if (App->render->camera.y < -170 * (int)App->win->GetScale())
	{
		App->render->camera.y = -170 * (int)App->win->GetScale();
	}
		
	if (App->input->GetKey(SDL_SCANCODE_PAGEUP) == KEY_DOWN)
	{
		if (App->audio->mbase_volume >= App->audio->mmin_volume && App->audio->mbase_volume < App->audio->mmax_volume)
			App->audio->mbase_volume++;

		LOG("|////////////////////////////////////////////////////|");
		LOG("Volume is now: %i", App->audio->mbase_volume);
		LOG("|////////////////////////////////////////////////////|");
	}

	if (App->input->GetKey(SDL_SCANCODE_PAGEDOWN) == KEY_DOWN)
	{
		if (App->audio->mbase_volume > App->audio->mmin_volume && App->audio->mbase_volume <= App->audio->mmax_volume)
			App->audio->mbase_volume--;

		LOG("|////////////////////////////////////////////////////|");
		LOG("Volume is now: %i", App->audio->mbase_volume);
		LOG("|////////////////////////////////////////////////////|");
	}

	//Debug
	
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		map_to_load = 1;
		LoadMap(map_to_load);
		App->player->player_position = App->map->current_spawn_point;
		App->player->player_speed.SetToZero();
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		map_to_load = 2;
		LoadMap(map_to_load);
		App->player->player_position = App->map->current_spawn_point;
		App->player->player_speed.SetToZero();
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		App->player->player_position = App->map->current_spawn_point;
		App->player->player_speed.SetToZero();
	}
		
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		App->request_save = true;

		LOG("|////////////////////////////////////////////////////|");
		LOG("User requests to save | request_save = %d", App->request_save);
		LOG("|////////////////////////////////////////////////////|");
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		App->request_load = true;

		LOG("|////////////////////////////////////////////////////|");
		LOG("User requests to load | request_load = %d", App->request_load);
		LOG("|////////////////////////////////////////////////////|");
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		App->map->debug_mode_active = !App->map->debug_mode_active;

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		App->player->god_mode = !App->player->god_mode;
	}

	App->map->Draw();
	App->player->Draw(dt);
	
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);

	p2SString title("Map: %dx%d  Tiles: %dx%d  Tilesets: %d  Layers: %d  Tile%d,%d FPS: %f",
		App->map->loaded_map.width, App->map->loaded_map.height,
		App->map->loaded_map.tile_height, App->map->loaded_map.tile_width,
		App->map->loaded_map.map_tilesets.count(), App->map->loaded_map.map_layers.count(),
		map_coordinates.x, map_coordinates.y,
		1.0f/dt
	);
	
	App->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::LoadMap(int given_map)
{
	if(is_map_loaded == true)
	{
		App->map->CleanUp();
		App->collisions->ClearColliders();
	}


	if (given_map == 1)
	{
		App->map->Load("Final1.tmx");
	}

	if (given_map == 2)
	{
		App->map->Load("Final2.tmx");
	}

	is_map_loaded = true;
}

void j1Scene::WinnerWinner()
{
	if (map_to_load == 1)
	{
		map_to_load = 2;
		LoadMap(map_to_load);
		App->player->player_position = App->map->current_spawn_point;
		App->player->player_speed.SetToZero();
		return;
	}

	map_to_load = 1;
	LoadMap(map_to_load);
	App->player->player_position = App->map->current_spawn_point;
	App->player->player_speed.SetToZero();
}

bool j1Scene::Save(pugi::xml_node& module_node) const
{
	pugi::xml_node child_map = module_node.append_child("current_map");

	child_map.append_attribute("value").set_value(map_to_load);

	return(true);
}

bool j1Scene::Load(pugi::xml_node& module_node)
{
	map_to_load = module_node.child("current_map").attribute("value").as_uint(1); 
	LoadMap(map_to_load);

	return(true);
}