#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collisions.h"

j1Collisions::j1Collisions()
{
	name.create("collisions");
}

j1Collisions::~j1Collisions(){}

bool j1Collisions::Awake(pugi::xml_node& module_node)
{
	return true;
}

bool j1Collisions::Start()
{
	return true;
}

bool j1Collisions::PreUpdate()
{
	return true;
}

bool j1Collisions::CleanUp()
{
	ClearColliders();

	return true;
}

bool j1Collisions::CheckCollision(SDL_Rect r1, SDL_Rect r2) const
{
	return (r1.x < r2.x + r2.w &&
			r1.x + r1.w > r2.x &&
			r1.y < r2.y + r2.h &&
			r1.h + r1.y > r2.y);
}

void j1Collisions::NoWalkable(SDL_Rect collider)
{
	no_walkable_tiles.add(collider);
}

void j1Collisions::TriggerDeath(SDL_Rect death)
{
	death_triggers.add(death);
}

void j1Collisions::TriggerWin(SDL_Rect victory)
{
	win_triggers.add(victory);
}

void j1Collisions::ClearColliders()
{
	death_triggers.clear();
	win_triggers.clear();
	no_walkable_tiles.clear();
}

bool j1Collisions::Save(pugi::xml_node& module_node)
{
	return true;
}

bool j1Collisions::Load(pugi::xml_node& module_node)
{
	return true;
}