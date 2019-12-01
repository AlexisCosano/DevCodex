#include "j1EntityManager.h"
#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"

j1EntityManager::j1EntityManager() : j1Module()
{
	name = "entities";
}

j1EntityManager::~j1EntityManager()
{
}

bool j1EntityManager::Awake(pugi::xml_node& module_node)
{
	return true;
}

bool j1EntityManager::Start()
{
	return true;
}

bool j1EntityManager::PreUpdate()
{
	return true;
}

bool j1EntityManager::Update(float dt)
{
	return true;
}

bool j1EntityManager::PostUpdate()
{
	return true;
}

bool j1EntityManager::Load(pugi::xml_node& module_node)
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node& module_node) const
{
	return true;
}