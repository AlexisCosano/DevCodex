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
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1App.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(perfect_timer);

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
	entity_manager = new j1EntityManager();
	pathfinding = new j1PathFinding();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(collisions);
	AddModule(pathfinding);
	AddModule(scene);
	AddModule(player);
	AddModule(entity_manager);

	// render last to swap buffer
	AddModule(render);

	PERF_PEEK(perfect_timer);
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
	PERF_START(perfect_timer);

	pugi::xml_parse_result loading_result = config_file.load_file("config");

	if (loading_result == true)
	{
		LOG("|////////////////////////////////////////////////////|");
		LOG("Loading result: %s", loading_result.description());
		config_parent_node = config_file.first_child();
		LOG("Config parent node: %s", config_parent_node.name());
		LOG("|////////////////////////////////////////////////////|");

		app_parent_node = config_parent_node.child("app");
		LOG("App node: %s", app_parent_node.name());

		max_frame_rate_cap = app_parent_node.child("frames").attribute("frame_rate_cap").as_int();
		LOG("Framerate cap: %d", max_frame_rate_cap);

		frames_capped = 1000 / max_frame_rate_cap;
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

	PERF_PEEK(perfect_timer);

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	PERF_START(perfect_timer);

	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	startup_time.Start();

	PERF_PEEK(perfect_timer);

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
	frame_count++;
	last_sec_frame_count++;
	frame_time.Start();
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

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	if (are_frames_capped == true)
	{
		avg_fps = float(frame_count) / startup_time.ReadSec();
	}
	else
	{
		avg_fps = avg_fps; //Just for testing purposes
	}

	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char assignment_title[256];
	sprintf_s(assignment_title, 256, "[Monster Heights]   Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu ",
		avg_fps, last_frame_ms, frames_on_last_update, seconds_since_startup, frame_count);
	App->win->SetTitle(assignment_title);

	if (frames_capped > 0 && last_frame_ms < frames_capped)
	{
		SDL_Delay(frames_capped - last_frame_ms);
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
	PERF_START(perfect_timer);

	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	PERF_PEEK(perfect_timer);

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

	return loading_result;
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

	return true;
}