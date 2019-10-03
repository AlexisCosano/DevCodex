#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"


// ----------------------------------------------------
struct Layer
{
	p2SString layer_name;
	uint width;
	uint height;

	unsigned int* gid;

	p2List<uint*> gids;

	// TODO 6: Short function to get the value of x,y
	inline uint Get(int x, int y) const;

};

struct Tileset
{
	// TODO 7: Create a method that receives a tile id and returns it's Rect

	p2SString tileset_name;
	uint tile_height;
	uint tile_width;
	uint first_gid;	
	uint spacing;
	uint margin;
	
	p2SString image_source;
	uint image_height;
	uint image_width;

	SDL_Rect GetRect(uint gid) const;
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
	p2SString map_name;

	RenderOrder render_order;
	Orientation map_orientation;

	p2List<Tileset*> map_tilesets;
	p2List<Layer*> map_layers;
	
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

	bool LoadLayers(pugi::xml_node& layer_node, Layer* layer);

	bool LoadTilesets(pugi::xml_node& tileset_node, Tileset* tileset);
	
	bool LoadMap();

	// TODO 8: Create a method that translates x,y coordinates from map positions to world positions


public:
	MapNode loaded_map;

private:
	pugi::xml_document map_file;
	p2SString folder;
	bool map_loaded;
};

#endif // __j1MAP_H__