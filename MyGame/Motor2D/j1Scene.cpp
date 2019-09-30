#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
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
	//img = App->tex->Load("textures/test.png");
	//App->audio->PlayMusic("audio/music/music_sadpiano.ogg");
	App->map->Load("maps/hello2.tmx");
	
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
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 10;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 10;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 10;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 10;

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		App->request_load = true;

		LOG("|////////////////////////////////////////////////////|");
		LOG("User requests to load | request_load = %d", App->request_load);
		LOG("|////////////////////////////////////////////////////|");
	}
		
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		App->request_save = true;

		LOG("|////////////////////////////////////////////////////|");
		LOG("User requests to save | request_save = %d", App->request_save);
		LOG("|////////////////////////////////////////////////////|");
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

	//App->render->Blit(img, 0, 0);
	App->map->Draw();

	// TODO 7: Set the window title like
	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	p2SString title("Map: %dx%d  Tiles: %dx%d  Tilesets: %d  Layers: %d",
		App->map->loaded_map.width, App->map->loaded_map.height,
		App->map->loaded_map.tile_height, App->map->loaded_map.tile_width,
		App->map->loaded_map.map_tilesets.count(), App->map->loaded_map.map_layers.count());
	
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

bool j1Scene::Load(pugi::xml_node& module_node)
{
	return(true);
}

bool j1Scene::Save(pugi::xml_node& module_node) const
{
	return(true);
}