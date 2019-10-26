#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collisions.h"
#include "j1Player.h"
#include "j1Map.h"
#include <math.h>


inline uint Layer::Get(int x, int y) const
{
	return(gid[(y * width) + x]);
}

SDL_Rect Tileset::GetRect(uint gid) const
{
	uint tile_id = gid - first_gid;
	SDL_Rect rect;
	
	rect.h = tile_height;
	rect.w = tile_width;

	rect.x = margin + ((rect.w + spacing) * (tile_id - num_tiles_width * (tile_id / num_tiles_width)));
	rect.y = margin + ((rect.h + spacing) * (tile_id / num_tiles_width));

	return(rect);
}

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& module_node)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(module_node.child("folder").child_value());

	return ret;
}

iPoint j1Map::MapToWorld(uint x, uint y) const
{
	iPoint world_position;

	if(loaded_map.map_orientation == ORTHOGONAL)
	{ 
		world_position.x = x * loaded_map.tile_width;
		world_position.y = y * loaded_map.tile_height;
	}

	else if (loaded_map.map_orientation == ISOMETRIC)
	{
		world_position.x = (x - y) * (loaded_map.tile_width * 0.5f);
		world_position.y = (x + y) * (loaded_map.tile_height * 0.5f);
	}

	else
	{
		LOG("|////////////////////////////////////////////////////|");
		LOG("ERROR: invalid map orientation: %d", loaded_map.map_orientation);
		LOG("|////////////////////////////////////////////////////|");

		world_position.x = x;
		world_position.y = y;
	}

	return(world_position);
}

iPoint j1Map::WorldToMap(uint x, uint y) const
{
	iPoint map_position;

	if (loaded_map.map_orientation == ORTHOGONAL)
	{
		map_position.x = x / loaded_map.tile_width;
		map_position.y = y / loaded_map.tile_height;
	}

	else if (loaded_map.map_orientation == ISOMETRIC)
	{
		map_position.x = uint(((x / (loaded_map.tile_width * 0.5f)) + (y / (loaded_map.tile_height * 0.5f)))) / 2;
		map_position.y = uint(((y / (loaded_map.tile_height * 0.5f)) - (x / (loaded_map.tile_width * 0.5f)))) / 2;
	}

	else
	{
		LOG("|////////////////////////////////////////////////////|");
		LOG("ERROR: invalid map orientation: %d", loaded_map.map_orientation);
		LOG("|////////////////////////////////////////////////////|");

		map_position.x = x;
		map_position.y = y;
	}
	
	return(map_position);
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;

	for (p2List_item<Layer*>* layer_iterator = loaded_map.map_layers.start; layer_iterator != nullptr; layer_iterator = layer_iterator->next)
	{
		Layer* layer_to_draw = layer_iterator->data;

		if (layer_to_draw->layer_properties.Get("NoDraw") != 0 && debug_mode_active == false)
			continue;

		for (uint y = 0; y < loaded_map.height; y++)
		{
			for (uint x = 0; x < loaded_map.width; x++)
			{
				uint tile_gid = layer_to_draw->Get(x, y);
				
				if (tile_gid > 0)
				{
					Tileset* tileset_used = GetTilesetFromTileId(tile_gid);
					SDL_Rect tile_rect = tileset_used->GetRect(tile_gid);
					iPoint world_position = MapToWorld((float)x, (float)y);
					
					if (layer_to_draw->layer_properties.Get("Parallax1") != 0)
					{
						App->render->Blit(tileset_used->tileset_texture, world_position.x + (float)(App->player->player_position.x - current_spawn_point.x) * 0.3f, world_position.y + (float)(App->player->player_position.y - current_spawn_point.y) * 0.3f, &tile_rect);
					}
					else if (layer_to_draw->layer_properties.Get("Parallax2") != 0)
					{
						App->render->Blit(tileset_used->tileset_texture, world_position.x + (float)(App->player->player_position.x - current_spawn_point.x) * 0.2f, world_position.y + (float)(App->player->player_position.y - current_spawn_point.y) * 0.3f, &tile_rect);
					}
					else if (layer_to_draw->layer_properties.Get("Parallax3") != 0)
					{
						App->render->Blit(tileset_used->tileset_texture, world_position.x + (float)(App->player->player_position.x - current_spawn_point.x) * 0.1f, world_position.y + (float)(App->player->player_position.y - current_spawn_point.y) * 0.3f, &tile_rect);
					}
					else
					{
						App->render->Blit(tileset_used->tileset_texture, world_position.x, world_position.y, &tile_rect);
					}
				}
			}
		}
	}
}

void j1Map::NoWalkable(Layer* collision_layer)
{
	for (int y = 0; y < loaded_map.height; ++y)
	{
		for (int x = 0; x < loaded_map.width; ++x)
		{
			int tile_id = collision_layer->Get(x, y);
			if (tile_id > 0)
			{
				Tileset* tileset = loaded_map.map_tilesets.start->data;

				SDL_Rect r = tileset->GetRect(tile_id);
				iPoint pos = MapToWorld(x, y);

				r.x = pos.x;
				r.y = pos.y;

				App->collisions->NoWalkable(r);
			}
		}
	}
}

void j1Map::SpawnPoint(Layer* collision_layer)
{
	for (int y = 0; y < loaded_map.height; ++y)
	{
		for (int x = 0; x < loaded_map.width; ++x)
		{
			int tile_id = collision_layer->Get(x, y);
			if (tile_id > 0)
			{
				Tileset* tileset = loaded_map.map_tilesets.start->data;

				iPoint pos = MapToWorld(x, y);

				current_spawn_point = pos;
			}
		}
	}
}

void j1Map::Death(Layer* collision_layer)
{
	for (int y = 0; y < loaded_map.height; ++y)
	{
		for (int x = 0; x < loaded_map.width; ++x)
		{
			int tile_id = collision_layer->Get(x, y);
			if (tile_id > 0)
			{
				Tileset* tileset = loaded_map.map_tilesets.start->data;

				SDL_Rect r = tileset->GetRect(tile_id);
				iPoint pos = MapToWorld(x, y);

				r.x = pos.x;
				r.y = pos.y;

				App->collisions->TriggerDeath(r);
			}
		}
	}
}

void j1Map::Win(Layer* collision_layer) 
{
	for (int y = 0; y < loaded_map.height; ++y)
	{
		for (int x = 0; x < loaded_map.width; ++x)
		{
			int tile_id = collision_layer->Get(x, y);
			if (tile_id > 0)
			{
				Tileset* tileset = loaded_map.map_tilesets.start->data;

				SDL_Rect r = tileset->GetRect(tile_id);
				iPoint pos = MapToWorld(x, y);

				r.x = pos.x;
				r.y = pos.y;

				App->collisions->TriggerWin(r);
			}
		}
	}
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	for (p2List_item<Tileset*>* iterator = loaded_map.map_tilesets.start; iterator != nullptr; iterator = iterator->next)
	{
		App->tex->UnLoad(iterator->data->tileset_texture);
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

		LoadProperties(layer_node.child("properties"), layer->layer_properties);

		int iterator = 0;

		pugi::xml_node current_tile = layer_node.child("data").child("tile");

		while (current_tile != nullptr)
		{
			layer->gid[iterator] = current_tile.attribute("gid").as_uint();
			layer->gids.add(&layer->gid[iterator]);
			current_tile = current_tile.next_sibling("tile");
			iterator++;
		}

		if (layer->layer_properties.Get("NoWalkable") != 0)
		{
			NoWalkable(layer);
		}

		if (layer->layer_properties.Get("Spawn") != 0)
		{
			SpawnPoint(layer);
		}

		if (layer->layer_properties.Get("Death") != 0)
		{
			Death(layer);
		}

		if (layer->layer_properties.Get("End") != 0)
		{
			Win(layer);
		}
	}

	return(ret);
}

int Properties::Get(const char* given_name, int given_value) const
{
	p2List_item<Property*>* property_to_check = properties_list.start;

	while (property_to_check)
	{
		if (property_to_check->data->property_name == given_name)
			return property_to_check->data->bool_value;

		property_to_check = property_to_check->next;
	}

	return(given_value);
}

bool j1Map::LoadProperties(pugi::xml_node& properties_node, Properties& properties)
{
	bool ret = false;

	if (properties_node == nullptr)
	{
		LOG("|////////////////////////////////////////////////////|");
		LOG("ERROR: properties tag is nullptr.");
		LOG("|////////////////////////////////////////////////////|");

		ret = false;
	}
	else
	{
		for (pugi::xml_node properties_iterator = properties_node.child("property"); properties_iterator != nullptr; properties_iterator = properties_iterator.next_sibling("property"))
		{
			Properties::Property* property_to_load = new Properties::Property();

			property_to_load->property_name = properties_iterator.attribute("name").as_string();
			property_to_load->bool_value = properties_iterator.attribute("value").as_int();

			properties.properties_list.add(property_to_load);
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

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, Tileset* tileset)
{
	bool ret = true;
	pugi::xml_node image_node = tileset_node.child("image");

	if (image_node == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		tileset->tileset_texture = App->tex->Load(PATH(folder.GetString(), image_node.attribute("source").as_string()));
		
		int w, h;
		SDL_QueryTexture(tileset->tileset_texture, NULL, NULL, &w, &h);
		tileset->image_width = image_node.attribute("width").as_int();

		if (tileset->image_width <= 0)
		{
			tileset->image_width = w;
		}

		tileset->image_height = image_node.attribute("height").as_int();

		if (tileset->image_height <= 0)
		{
			tileset->image_height = h;
		}

		tileset->num_tiles_width = tileset->image_width / tileset->tile_width;
		tileset->num_tiles_height = tileset->image_height / tileset->tile_height;
	}

	return ret;
}

Tileset* j1Map::GetTilesetFromTileId(uint given_id) const
{
	Tileset* tileset_to_use = nullptr;

	for (p2List_item<Tileset*>* tileset_iterator = loaded_map.map_tilesets.start; tileset_iterator != nullptr; tileset_iterator = tileset_iterator->next)
	{
		if (given_id < tileset_iterator->data->first_gid)
			break;
		else
			tileset_to_use = tileset_iterator->data;
	}

	return(tileset_to_use);
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

		if (ret == true)
			ret = LoadTilesetImage(actual_tileset, tileset_to_load);

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