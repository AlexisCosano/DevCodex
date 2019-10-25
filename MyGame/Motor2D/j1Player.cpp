#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "j1Player.h"
#include "j1Scene.h"


j1Player::j1Player() : j1Module()
{
	name.create("player");
}

// Destructor ---------------------------------
j1Player::~j1Player()
{}

// Called before the first frame
bool j1Player::Start()
{
	player_position.x = 400;
	player_position.y = 100;
	player_speed.x = 4;
	player_speed.y = 1;
	player_texture = App->tex->Load(PATH(player_folder.GetString(), "axolotl.png"));
	
	player_rect = { 23, 0, 15, 34 };
	
	return true;
}

// Called before render is available ----------
bool j1Player::Awake(pugi::xml_node& module_node)
{
	LOG("Init player");
	bool ret = true;

	player_folder.create(module_node.child("folder").child_value());

	return ret;
}

void j1Player::Draw()
{
	App->render->Blit(player_texture, player_position.x, player_position.y);

	if (App->map->debug_mode_active)
	{
		App->render->DrawQuad(player_rect, 20, 220, 20, 255, false, true);
	}
}

void j1Player::ApplyGravity()
{
	player_position.y += gravity * player_speed.y;
}

bool j1Player::CheckCollisions()
{
	bool ret = false;
	p2List_item<SDL_Rect>* item = App->collisions->no_walkable_tiles.start;

	for (item; item != App->collisions->no_walkable_tiles.end; item = item->next)
	{
		ret = App->collisions->CheckCollision(player_rect, item->data);
		
		if (ret)
			return ret;
	}

	return ret;
}

// Update ------------------------------------
bool j1Player::Update(float dt)
{
	bool ret = true;
	
	player_rect.x = player_position.x + 23;
	player_rect.y = player_position.y;

	if (CheckCollisions() == false)
		ApplyGravity();
	else
		
	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		player_position.x -= 1 * player_speed.x;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		player_position.x += 1 * player_speed.x;
	}

	return ret;
}

// Called before quitting --------------------
bool j1Player::CleanUp()
{
	LOG("Destroying player");

	return true;
}

// Save & Load ------------------------------
bool j1Player::Save(pugi::xml_node& node) const
{
	return(true);
}

bool j1Player::Load(pugi::xml_node& node)
{
	return(true);
}