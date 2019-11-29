#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class j1Collisions;
class j1Player;
class j1PathFinding;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

private:

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load and Save
	bool Load();
	bool Save() const;


public:

	// Modules
	j1Window* win;
	j1Input* input;
	j1Render* render;
	j1Textures* tex;
	j1Audio* audio;
	j1Scene* scene;
	j1Map* map;
	j1Collisions* collisions;
	j1Player* player;
	j1PathFinding* pathfinding;

	mutable bool request_save = false;
	bool request_load = false;


private:

	p2List<j1Module*> modules;
	
	uint frames;
	float dt;
	float last_frame_time;

	pugi::xml_document config_file;
	pugi::xml_node config_parent_node;

	pugi::xml_document save_file;
	pugi::xml_node save_file_parent_node;

	pugi::xml_node app_parent_node;


	int	argc;
	char** args;
	p2SString title;
	p2SString organization;

	// Frame rate
	int	frames_capped;
	j1Timer	frame_time;
	int max_frame_rate_cap;
	uint64 frame_count = 0;
	j1PerfTimer perfect_timer;
	j1Timer last_sec_frame_time;
	j1Timer	startup_time;
	uint32 last_sec_frame_count = 0;
	uint32 prev_last_sec_frame_count = 0;
};

extern j1App* App; 

#endif