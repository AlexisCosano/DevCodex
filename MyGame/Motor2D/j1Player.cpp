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
	player_position = App->map->current_spawn_point;
	player_speed.x = 0;
	player_speed.y = 0;
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

void j1Player::GodMode(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		player_speed.y = 3 + gravity * dt;

		player_rect.y -= player_speed.y;

		SDL_Rect* collider = CheckCollisions(TOP);

		if (collider == nullptr)
		{
			player_position.y = player_rect.y;
		}
		else
		{
			player_rect.y = collider->y + collider->h;
			player_position.y = player_rect.y;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		player_speed.y = 3 + gravity * dt;

		player_rect.y += player_speed.y;

		SDL_Rect* collider = CheckCollisions(RIGHT);

		if (collider == nullptr)
		{
			player_position.y = player_rect.y;
		}
		else
		{
			player_rect.y = collider->y - player_rect.h;
			player_position.y = player_rect.y;
		}
	}

}

void j1Player::ApplyGravity(float dt)
{
	player_rect.y += player_speed.y;
	SDL_Rect* collider = CheckCollisions(BOTTOM);

	if (collider == nullptr)
	{
		player_position.y = player_rect.y;
	}
	else
	{
		player_rect.y = player_position.y + (collider->y - (player_position.y + player_rect.h));
		player_position.y = player_rect.y;
		player_speed.y = 0.0f;
	}
}

void j1Player::Jump(float dt)
{
	player_rect.y += player_speed.y;
	SDL_Rect* collider = CheckCollisions(TOP);

	
	if (collider == nullptr)
	{
		player_position.y = player_rect.y;
	}
	else
	{
		player_position.y = player_rect.y;
	}
	
}

bool j1Player::HasPlayerDied()
{
	bool ret = false;
	p2List_item<SDL_Rect>* item = App->collisions->death_triggers.start;

	for (item; item != App->collisions->death_triggers.end; item = item->next)
	{
		ret = App->collisions->CheckCollision(player_rect, item->data);

		if (ret)
			return ret;
	}

	return ret;
}

bool j1Player::HasPlayerWon()
{
	bool ret = false;
	p2List_item<SDL_Rect>* item = App->collisions->win_triggers.start;

	for (item; item != App->collisions->win_triggers.end; item = item->next)
	{
		ret = App->collisions->CheckCollision(player_rect, item->data);

		if (ret)
			return ret;
	}

	return ret;
}

SDL_Rect* j1Player::CheckCollisions(CollisionDirection direction)
{
	bool ret = false;
	p2List_item<SDL_Rect>* item = App->collisions->no_walkable_tiles.start;

	for (item; item != App->collisions->no_walkable_tiles.end; item = item->next)
	{
		ret = App->collisions->CheckCollision(player_rect, item->data);
		
		if (!ret)
			continue;
		
		switch (direction)
		{
		case BOTTOM:
			if (player_rect.y + player_rect.h > item->data.y)
				return &item->data;
			break;
		
		case TOP:
			if (player_rect.y < item->data.y + item->data.h)
				return &item->data;
			break;

		case RIGHT:
			if (player_rect.x + player_rect.w > item->data.x)
				return &item->data;
			break;

		case LEFT:
			if (player_rect.x < item->data.x + item->data.w)
				return &item->data;
			break;
		}
	}

	return nullptr;
}

// Update ------------------------------------
bool j1Player::Update(float dt)
{
	bool ret = true;
	
	player_rect.x = player_position.x + 23;
	player_rect.y = player_position.y;
		
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		player_speed.x = 3 + gravity * dt;

		player_rect.x -= player_speed.x;

		SDL_Rect* collider = CheckCollisions(LEFT);

		if (collider == nullptr)
		{
			player_position.x = player_rect.x - 23;
		}
		else
		{
			player_rect.x = collider->x + collider->w;
			player_position.x = player_rect.x - 23;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		player_speed.x = 3 + gravity * dt;

		player_rect.x += player_speed.x;

		SDL_Rect* collider = CheckCollisions(RIGHT);

		if (collider == nullptr)
		{
			player_position.x = player_rect.x - 23;
		}
		else
		{
			player_rect.x = collider->x - player_rect.w;
			player_position.x = player_rect.x - 23;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		player_rect.y += 1;
		SDL_Rect* collider = CheckCollisions(BOTTOM);
		player_rect.y = player_position.y;
		if(collider != nullptr)
		{
			player_speed.y = -6;
		}
	}

	if (HasPlayerWon() == true)
	{
		LOG("UUUUUUUUUUUUUUUUUH");
		player_speed.y = -10;
	}

	if (player_speed.y < 0)
		Jump(dt);
	else
	{
		if (god_mode)
		{
			GodMode(dt);
			return ret;
		}
		ApplyGravity(dt);
	}

	player_speed.y += gravity * dt;

	if (HasPlayerDied() == true)
	{
		player_position = App->map->current_spawn_point;
		player_speed.SetToZero();
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