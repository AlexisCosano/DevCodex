#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"


// ----------------------------------------------------
struct TileSet
{
	p2SString map_name;
	uint tile_height;
	uint tile_width;
	uint first_gid;	
	uint spacing;
	uint margin;
	
	p2SString image_source;
	uint image_height;
	uint image_width;
};

enum Orientation
{
	ORIENTATION_UNKNOWN = 0,
	ORTHOGONAL,
	ISOMETRIC,
	STAGGERED
};

enum RenderOrder
{
	RENDER_ORDER_UNKNOWN = 0,
	RIGHT_DOWN,
	LEFT_DOWN,
	RIGHT_UP,
	LEFT_UP
};

struct MapNode
{
	RenderOrder render_order;
	Orientation map_orientation;
	
	uint next_object_id;
	uint tile_height;
	uint tile_width;
	uint height;
	uint width;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	bool LoadMap();

private:

	MapNode loaded_map;
	pugi::xml_document map_file;
	p2SString folder;
	bool map_loaded;
};

#endif // __j1MAP_H__