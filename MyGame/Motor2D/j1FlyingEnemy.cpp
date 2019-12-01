#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "j1Player.h"
#include "j1Pathfinding.h"
#include "j1FlyingEnemy.h"


j1FlyingEnemy::j1FlyingEnemy() : j1Entity()
{
	rect = { 0, 0, 40, 29 };
}

// Destructor ---------------------------------
j1FlyingEnemy::~j1FlyingEnemy()
{}

bool j1FlyingEnemy::Awake(pugi::xml_node & module_node)
{
	return true;
}

bool j1FlyingEnemy::Start()
{
	texture = App->tex->Load(PATH("textures/", "flying_enemy.png"));

	return true;
}


bool j1FlyingEnemy::Update(float dt)
{

	if (App->map->player->player_position.DistanceTo(position) < 250)
	{
  		iPoint pos;
		pos.x = position.x;
		pos.y = position.y;

		iPoint player_pos;
		player_pos.x = App->map->player->player_position.x;
		player_pos.y = App->map->player->player_position.y;
		App->map->pathfinding->CreatePath(App->map->WorldToMap(pos.x, pos.y), App->map->WorldToMap(player_pos.x, player_pos.y));
		p2DynArray<iPoint>* path = App->map->pathfinding->GetLastPath();
		iPoint next_pos;

		if (App->map->debug_mode_active)
		{
			for (int i = 0; i < path->Count(); i++)
			{
				App->render->DrawQuad({ App->map->MapToWorld(path->At(i)->x, path->At(i)->y).x + 16, App->map->MapToWorld(path->At(i)->x, path->At(i)->y).y + 16, 10, 10 }, 20, 220, 60, 255, false, true);
			}
		}

		if (!path->Pop(next_pos))
		{
			return true;
		}
		

		fPoint fnext_pos;
		fnext_pos.x = App->map->MapToWorld(next_pos.x, next_pos.y).x + 1.0f;
		fnext_pos.y = App->map->MapToWorld(next_pos.x, next_pos.y).y + 1.0f;

		fnext_pos = position - fnext_pos;

		fnext_pos.x /= fnext_pos.Magnitude();
		fnext_pos.y /= fnext_pos.Magnitude();

		position -= fnext_pos * (80.0f * dt);
	}


	return true;
}

bool j1FlyingEnemy::PostUpdate()
{
	return true;
}

bool j1FlyingEnemy::CleanUp()
{
	return true;
}

bool j1FlyingEnemy::Save(pugi::xml_node & module_node) const
{
	return true;
}

bool j1FlyingEnemy::Load(pugi::xml_node & module_node)
{
	return true;
}

bool j1FlyingEnemy::Draw(float dt)
{
	App->render->Blit(texture, position.x, position.y, &rect);

	return true;
}

void j1FlyingEnemy::SetPosition(iPoint position)
{
	this->position.x = position.x;
	this->position.y = position.y;

	spawn_point = position;
}

void j1FlyingEnemy::Reset()
{
	this->position.x = spawn_point.x;
	this->position.y = spawn_point.y;
}