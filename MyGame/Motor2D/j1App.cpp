#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "j1Player.h"
#include "j1App.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();
	map = new j1Map();
	collisions = new j1Collisions();
	player = new j1Player();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(player);
	AddModule(collisions);
	AddModule(scene);

	// render last to swap buffer
	AddModule(render);
}

// Destructor
j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
	config_file.reset();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool j1App::Awake()
{
	pugi::xml_parse_result loading_result = config_file.load_file("config");

	if (loading_result == true)
	{
		LOG("|////////////////////////////////////////////////////|");
		LOG("Loading result: %s", loading_result.description());
		config_parent_node = config_file.first_child();
		LOG("Config parent node: %s", config_parent_node.name());
		LOG("|////////////////////////////////////////////////////|");
	}
	else
	{
		LOG("|////////////////////////////////////////////////////|");
		LOG("Loading result: %s", loading_result.description());
		LOG("|////////////////////////////////////////////////////|");
	}

	bool ret = true;

	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Awake(config_parent_node.child(item->data->name.GetString()));
		item = item->next; 
	}

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	if (request_save == true)
	{
		Save();
	}

	if (request_load == true)
	{
		Load();
	}
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;
	dt = (SDL_GetTicks() - last_frame_time)/1000.0f;
	last_frame_time = SDL_GetTicks();

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}

// ---------------------------------------
bool j1App::Load()
{
	LOG("Loading...");
	LOG("|////////////////////////////////////////////////////|");

	pugi::xml_parse_result loading_result = save_file.load_file("save_file");

	if (loading_result == true)
	{
		LOG("Loading result: %s", loading_result.description());
		save_file_parent_node = save_file.first_child();
		LOG("Config parent node: %s", save_file_parent_node.name());
		LOG("|////////////////////////////////////////////////////|");
	}
	else
	{
		LOG("Loading result: %s", loading_result.description());
		LOG("|////////////////////////////////////////////////////|");
	}

	p2List_item<j1Module*>* item;
	item = modules.start;

	while (item != NULL && loading_result == true)
	{
		item->data->Load(save_file_parent_node.child(item->data->name.GetString()));
		item = item->next;
	}

	request_load = false;

	LOG("Game loaded. | request_load = %d", request_load);
	LOG("|////////////////////////////////////////////////////|");

	return(loading_result);
}

// ---------------------------------------
bool j1App::Save() const
{
	LOG("Saving...");
	LOG("|////////////////////////////////////////////////////|");

	pugi::xml_document new_save_file;
	pugi::xml_node new_save_file_parent_node;

	new_save_file_parent_node = new_save_file.append_child("save_file");

	bool ret = true;

	p2List_item<j1Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		item->data->Save(new_save_file_parent_node.append_child(item->data->name.GetString()));
		item = item->next;
	}

	new_save_file.save_file("./save_file");

	request_save = false;

	LOG("Game saved. | request_save = %d", request_save);
	LOG("|////////////////////////////////////////////////////|");

	return(true);
}