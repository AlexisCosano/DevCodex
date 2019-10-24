#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
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
	player_position.x = player_position.y = 0;
	player_speed.x = 0.0f;
	player_speed.y = 0.5f;
	player_texture = App->tex->Load(PATH(player_folder.GetString(), "player.png"));
	
	player_rect = { 0, 0, 14, 15 };
	
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

// Update ------------------------------------
bool j1Player::Update(float dt)
{
	bool ret = true;
	
	player_rect.x = player_position.x;
	player_rect.y = player_position.y;

	ApplyGravity();

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