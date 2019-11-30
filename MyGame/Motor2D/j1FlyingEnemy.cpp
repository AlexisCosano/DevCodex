#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "j1FlyingEnemy.h"


j1FlyingEnemy::j1FlyingEnemy() : j1Entity()
{
	entity_name.create("flying_enemy");
}

// Destructor ---------------------------------
j1FlyingEnemy::~j1FlyingEnemy()
{}

// Called before the first frame
bool j1FlyingEnemy::Start()
{
	entity_position = App->map->current_spawn_point;
	flying_enemy_texture = App->tex->Load(PATH(flying_enemy_folder.GetString(), "axolotl.png"));
	die_texture = App->tex->Load(PATH(flying_enemy_folder.GetString(), "death.png"));

	idle_animation.name = "idle";
	idle_animation.max_frame = 5;

	for (int i = 0; i < 5; i++)
	{
		idle_animation.animation_rect[i] = { 36 * i, 0, 36, 38 };
	}

	jump_animation.name = "jump";
	jump_animation.max_frame = 4;
	jump_animation.loop = false;

	for (int i = 0; i < 4; i++)
	{
		jump_animation.animation_rect[i] = { 36 * i, 38, 36, 38 };
	}

	run_animation.name = "run";
	run_animation.max_frame = 6;

	for (int i = 0; i < 6; i++)
	{
		run_animation.animation_rect[i] = { 36 * i, 38 * 2, 36, 38 };
	}

	die_animation.name = "die";
	die_animation.max_frame = 7;
	die_animation.loop = false;
	die_animation.max_frame_time = 0.07f;

	for (int i = 0; i < 7; i++)
	{
		die_animation.animation_rect[i] = { 40 * i, 0, 40, 44 };
	}

	current_animation = &idle_animation;

	LOG("This second?");

	return true;
}

// Called before render is available ----------
bool j1FlyingEnemy::Awake(pugi::xml_node& module_node)
{
	LOG("Init player");
	bool ret = true;

	flying_enemy_folder.create(module_node.child("folder").child_value());

	entity_speed.x = module_node.child("speed").attribute("x").as_uint();
	entity_speed.y = module_node.child("speed").attribute("y").as_uint();

	gravity = module_node.child("attributes").attribute("gravity").as_float();

	flying_enemy_rect.x = module_node.child("rect").attribute("x").as_uint();
	flying_enemy_rect.y = module_node.child("rect").attribute("y").as_uint();
	flying_enemy_rect.w = module_node.child("rect").attribute("w").as_uint();
	flying_enemy_rect.h = module_node.child("rect").attribute("h").as_uint();

	return ret;
}

void j1FlyingEnemy::Draw(float dt)
{
	SDL_Texture* texture = flying_enemy_texture;
	if (dead)
	{
		texture = die_texture;
	}

	App->render->Blit(texture, entity_position.x, entity_position.y, &current_animation->GetCurrentFrame(), flip);

	if (App->map->debug_mode_active)
	{
		App->render->DrawQuad(flying_enemy_rect, 20, 220, 20, 255, false, true);
	}

	frame_time += dt;

	if (frame_time > current_animation->max_frame_time)
	{
		frame_time = 0.0f;
		current_animation->GetNextFrame();
	}
}

void j1FlyingEnemy::ChooseAnimation()
{
	if (entity_speed.y > 1.0f || entity_speed.y < -1.0f)
	{
		current_animation = &jump_animation;
		return;
	}

	if (entity_speed.x > 0.001f || entity_speed.x < -0.001f)
	{
		current_animation = &run_animation;
		return;
	}

	current_animation = &idle_animation;
}

void j1FlyingEnemy::ApplyGravity(float dt)
{
	flying_enemy_rect.y += entity_speed.y;
	SDL_Rect* collider = CheckCollisions(FE_BOTTOM);

	if (collider == nullptr)
	{
		entity_position.y = flying_enemy_rect.y;
	}
	else
	{
		flying_enemy_rect.y = entity_position.y + (collider->y - (entity_position.y + flying_enemy_rect.h));
		entity_position.y = flying_enemy_rect.y;
		entity_speed.y = 0.0f;
	}
}

SDL_Rect* j1FlyingEnemy::CheckCollisions(CollisionDirection direction)
{
	bool ret = false;
	p2List_item<SDL_Rect>* item = App->collisions->no_walkable_tiles.start;

	for (item; item != App->collisions->no_walkable_tiles.end; item = item->next)
	{
		ret = App->collisions->CheckCollision(flying_enemy_rect, item->data);

		if (!ret)
			continue;

		switch (direction)
		{
		case FE_BOTTOM:
			if (flying_enemy_rect.y + flying_enemy_rect.h > item->data.y)
				return &item->data;
			break;

		case FE_TOP:
			if (flying_enemy_rect.y < item->data.y + item->data.h)
				return &item->data;
			break;

		case FE_RIGHT:
			if (flying_enemy_rect.x + flying_enemy_rect.w > item->data.x)
				return &item->data;
			break;

		case FE_LEFT:
			if (flying_enemy_rect.x < item->data.x + item->data.w)
				return &item->data;
			break;
		}
	}

	return nullptr;
}

// Update ------------------------------------
bool j1FlyingEnemy::Update(float dt)
{
	bool ret = true;

	entity_speed.y += gravity * dt;
	entity_speed.x = 0.0f;

	return ret;
}

// Called before quitting --------------------
bool j1FlyingEnemy::CleanUp()
{
	LOG("Destroying player");

	return true;
}

// Save & Load ------------------------------
bool j1FlyingEnemy::Save(pugi::xml_node& module_node) const
{
	pugi::xml_node child_flying_enemy_position = module_node.append_child("position");

	child_flying_enemy_position.append_attribute("x").set_value(entity_position.x);
	child_flying_enemy_position.append_attribute("y").set_value(entity_position.y);

	return true;
}

bool j1FlyingEnemy::Load(pugi::xml_node& module_node)
{
	entity_position.x = module_node.child("position").attribute("x").as_uint();
	entity_position.y = module_node.child("position").attribute("y").as_uint();

	entity_speed.SetToZero();
	return true;
}