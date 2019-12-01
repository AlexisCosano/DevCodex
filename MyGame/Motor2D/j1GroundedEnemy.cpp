#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include "j1GroundedEnemy.h"


j1GroundedEnemy::j1GroundedEnemy() : j1Entity()
{
}

// Destructor ---------------------------------
j1GroundedEnemy::~j1GroundedEnemy()
{}

bool j1GroundedEnemy::Start()
{
	return false;
}

bool j1GroundedEnemy::Awake(pugi::xml_node & module_node)
{
	return false;
}

bool j1GroundedEnemy::Update(float dt)
{
	return false;
}

bool j1GroundedEnemy::CleanUp()
{
	return false;
}

bool j1GroundedEnemy::Save(pugi::xml_node & module_node) const
{
	return false;
}

bool j1GroundedEnemy::Load(pugi::xml_node & module_node)
{
	return false;
}

bool j1GroundedEnemy::Draw(float dt)
{
	return true;
}
