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
}

// Destructor ---------------------------------
j1FlyingEnemy::~j1FlyingEnemy()
{}

bool j1FlyingEnemy::Start()
{
	return false;
}

bool j1FlyingEnemy::Awake(pugi::xml_node & module_node)
{
	return false;
}

bool j1FlyingEnemy::Update(float dt)
{
	return false;
}

bool j1FlyingEnemy::CleanUp()
{
	return false;
}

bool j1FlyingEnemy::Save(pugi::xml_node & module_node) const
{
	return false;
}

bool j1FlyingEnemy::Load(pugi::xml_node & module_node)
{
	return false;
}

void j1FlyingEnemy::Draw(float dt)
{
}
