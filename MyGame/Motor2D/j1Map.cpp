#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>


inline uint Layer::Get(int x, int y) const
{
	return(gid[y*width + x]);
}

SDL_Rect Tileset::GetRect(uint gid) const
{
	uint tile_id = gid - first_gid;
	SDL_Rect* rect = nullptr;

	uint tileset_width = image_width / tile_width;
	
	rect->x = margin + ((rect->w + spacing) * (tile_id - tileset_width * (tile_id / tileset_width)));
	rect->y = margin + ((rect->h + spacing) * (tile_id / tileset_width));
	
	rect->h = tile_height;
	rect->w = tile_width;

	return(*rect);
}

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

iPoint j1Map::MapToWorld(uint x, uint y) const
{
	iPoint world_position;

	world_position.x = x * loaded_map.tile_width;
	world_position.y = y * loaded_map.tile_height;

	return(world_position);
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;

	for (p2List_item<Layer*>* layer_iterator = loaded_map.map_layers.start; layer_iterator != loaded_map.map_layers.end; layer_iterator = layer_iterator->next)
	{
		Layer* layer_to_draw = layer_iterator->data;

		for (uint y = 0; y < loaded_map.height; y++)
		{
			for (uint x = 0; x < loaded_map.width; x++)
			{
				uint tile_gid = layer_to_draw->Get(x, y);

				if (tile_gid != 0)
				{
					//Draw
				}
			}
		}
	}

	// TODO 9: Complete the draw function

}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	for (p2List_item<Tileset*>* iterator = loaded_map.map_tilesets.start; iterator != nullptr; iterator = iterator->next)
	{
		RELEASE(iterator->data);
	}

	for (p2List_item<Layer*>* iterator = loaded_map.map_layers.start; iterator != nullptr; iterator = iterator->next)
	{
		RELEASE(iterator->data);
	}

	loaded_map.map_tilesets.clear();
	loaded_map.map_layers.clear();
	map_file.reset();

	return true;
}

bool j1Map::LoadLayers(pugi::xml_node& layer_node, Layer* layer)
{
	bool ret = true;

	if (layer_node == nullptr)
	{
		LOG("|////////////////////////////////////////////////////|");
		LOG("ERROR: layer tag is nullptr.");
		LOG("|////////////////////////////////////////////////////|");

		ret = false;
	}
	else
	{
		layer->layer_name = layer_node.attribute("name").as_string();

		layer->height = layer_node.attribute("height").as_int();
		layer->width = layer_node.attribute("width").as_int();

		layer->gid = new uint[layer->height*layer->width];
		memset(layer->gid, 0, sizeof(uint)*(layer->height*layer->width));

		int iterator = 0;

		pugi::xml_node current_tile = layer_node.child("data").child("tile");

		while (current_tile != nullptr)
		{
			layer->gid[iterator] = current_tile.attribute("gid").as_uint();
			layer->gids.add(&layer->gid[iterator]);
			current_tile = current_tile.next_sibling("tile");
			iterator++;
		}
	}

	return(ret);
}

bool j1Map::LoadTilesets(pugi::xml_node& tileset_node, Tileset* tileset)
{
	bool ret = true;

	if (tileset_node == nullptr)
	{
		LOG("|////////////////////////////////////////////////////|");
		LOG("ERROR: tileset tag is nullptr.");
		LOG("|////////////////////////////////////////////////////|");

		ret = false;
	}
	else
	{
		tileset->tileset_name = tileset_node.attribute("name").as_string();
		tileset->image_source = tileset_node.child("image").attribute("source").as_string();

		tileset->tile_height = tileset_node.attribute("tileheight").as_int();
		tileset->tile_width = tileset_node.attribute("tilewidth").as_int();
		tileset->first_gid = tileset_node.attribute("firstgid").as_int();
		tileset->spacing = tileset_node.attribute("spacing").as_int();
		tileset->margin = tileset_node.attribute("margin").as_int();
	}

	return(ret);
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
		ret = LoadMap();

	for (pugi::xml_node actual_tileset = map_file.child("map").child("tileset"); actual_tileset; actual_tileset = actual_tileset.next_sibling("tileset"))
	{
		Tileset* tileset_to_load = new Tileset();

		if (ret == true)
			ret = LoadTilesets(actual_tileset, tileset_to_load);

		if (ret == false)
		{
			LOG("|////////////////////////////////////////////////////|");
			LOG("Tileset '%s' could not be loaded.", actual_tileset.attribute("name").as_string());
			LOG("|////////////////////////////////////////////////////|");
		}
		else
			loaded_map.map_tilesets.add(tileset_to_load);
	}

	if (ret == true)
	{
		LOG("|////////////////////////////////////////////////////|");
		LOG("Succesfully parsed map file: %s", file_name);
		LOG("Map width: %d    Map height: %d", loaded_map.width, loaded_map.height);
		LOG("|////////////////////////////////////////////////////|");
		for (p2List_item<Tileset*>* iterator = loaded_map.map_tilesets.start; iterator != nullptr; iterator = iterator->next)
		{
			LOG("Tileset's name: %s    firstgid: %d", iterator->data->tileset_name.GetString(), iterator->data->first_gid);
			LOG("Tiles' area: %dx%d", iterator->data->tile_width, iterator->data->tile_height);
			LOG("Spacing: %d    Margin: %d", iterator->data->spacing, iterator->data->margin);
			LOG("|////////////////////////////////////////////////////|");
		}
	}

	for (pugi::xml_node actual_layer = map_file.child("map").child("layer"); actual_layer; actual_layer = actual_layer.next_sibling("layer"))
	{
		Layer* layer_to_load = new Layer();

		if (ret == true)
			ret = LoadLayers(actual_layer, layer_to_load);

		if (ret == false)
		{
			LOG("|////////////////////////////////////////////////////|");
			LOG("Layer '%s' could not be loaded.", actual_layer.attribute("name").as_string());
			LOG("|////////////////////////////////////////////////////|");
		}
		else
			loaded_map.map_layers.add(layer_to_load);
	}

	if (ret == true)
	{
		for (p2List_item<Layer*>* iterator = loaded_map.map_layers.start; iterator != nullptr; iterator = iterator->next)
		{
			LOG("Layer's name: %s     %dx%d", iterator->data->layer_name.GetString(), iterator->data->width, iterator->data->height);
			LOG("|////////////////////////////////////////////////////|");
		}
	}

	map_loaded = ret;

	return ret;
}