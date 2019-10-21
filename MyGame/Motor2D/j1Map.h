#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"


// TODO 5: Create a generic structure to hold properties
// ----------------------------------------------------
struct Properties
{
	bool not_drawn = false;
	bool navigation = false;

// TODO 7: Our custom properties should have one method
// to ask for the value of a custom property
};

// ----------------------------------------------------
struct Layer
{
	p2SString layer_name;
	uint width;
	uint height;

	unsigned int* gid;

	p2List<uint*> gids;

	Properties layer_properties;

	inline uint Get(int x, int y) const;
};

struct Tileset
{
	p2SString tileset_name;
	uint tile_height;
	uint tile_width;
	uint first_gid;	
	uint spacing;
	uint margin;

	SDL_Texture* tileset_texture;
	p2SString image_source;
	uint num_tiles_height;
	uint num_tiles_width;
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

	bool LoadTilesetImage(pugi::xml_node& tileset_node, Tileset* tileset);

	bool LoadProperties(pugi::xml_node& properties_node, Properties& properties);
	
	bool LoadMap();

	iPoint MapToWorld(uint x, uint y) const;
	iPoint WorldToMap(uint x, uint y) const;

	Tileset* GetTilesetFromTileId(uint given_id) const;

public:
	MapNode loaded_map;

private:
	pugi::xml_document map_file;
	p2SString folder;
	bool map_loaded;
};

#endif // __j1MAP_H__