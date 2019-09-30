#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map


	map_file.reset();

	return true;
}

bool j1Map::LoadMap()
{
	bool ret = true;

	pugi::xml_node map_parent_node = map_file.child("map");

	if (map_parent_node == nullptr)
	{
		LOG("|////////////////////////////////////////////////////|");
		LOG("ERROR: map tag is nullptr.");
		LOG("|////////////////////////////////////////////////////|");

		ret = false;
	}
	else
	{
		loaded_map.next_object_id = map_parent_node.attribute("nextobjectid").as_int();
		loaded_map.tile_height = map_parent_node.attribute("tileheight").as_int();
		loaded_map.tile_width = map_parent_node.attribute("tilewidth").as_int();
		loaded_map.height = map_parent_node.attribute("height").as_int();
		loaded_map.width = map_parent_node.attribute("width").as_int();

		p2SString orientation = map_parent_node.attribute("orientation").as_string();

		if (orientation == "orthogonal")
			loaded_map.map_orientation = ORTHOGONAL;
		else if (orientation == "isometric")
			loaded_map.map_orientation = ISOMETRIC;
		else if (orientation == "staggered")
			loaded_map.map_orientation = STAGGERED;
		else
			loaded_map.map_orientation = ORIENTATION_UNKNOWN;

		p2SString rendering_order = map_parent_node.attribute("renderorder").as_string();

		if (rendering_order == "right-down")
			loaded_map.render_order = RIGHT_DOWN;
		else if (rendering_order == "left-down")
			loaded_map.render_order = LEFT_DOWN;
		else if (rendering_order == "right-up")
			loaded_map.render_order = RIGHT_UP;
		else if (rendering_order == "left-up")
			loaded_map.render_order = LEFT_UP;
		else
			loaded_map.render_order = RENDER_ORDER_UNKNOWN;
	}

	return(ret);
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if (ret == true)
	{
		// TODO 3: Create and call a private function to load and fill
		// all your map data

	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!


	if (ret == true)
	{
		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
	}

	map_loaded = ret;

	return ret;
}