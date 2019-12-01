#include "j1EntityManager.h"
#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Player.h"
#include "j1GroundedEnemy.h"
#include "j1FlyingEnemy.h"

j1EntityManager::j1EntityManager() : j1Module()
{
	name = "entities";
}

j1EntityManager::~j1EntityManager()
{
}

bool j1EntityManager::Awake(pugi::xml_node& module_node)
{
	this->module_node = module_node;
	bool ret = true;

	return ret;
}

bool j1EntityManager::Start()
{
	bool ret = true;

	return ret;
}

bool j1EntityManager::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->PreUpdate();

		item = item->next;
	}

	return ret;
}

bool j1EntityManager::Update(float dt)
{
	bool ret = true;
	p2List_item<j1Entity*>* item;
	item = entities.start;

	while (item != NULL && item->data != NULL && ret == true)
	{
		ret = item->data->Update(dt);
		item = item->next;
	}

	item = entities.start;
	while (item != NULL && ret == true)
	{
		ret = item->data->Draw(dt);
		item = item->next;
	}

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	bool ret = true;
	p2List_item<j1Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->PostUpdate();
		item = item->next;
	}

	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;
	p2List_item<j1Entity*>* item;
	item = entities.start;

	while (item != NULL)
	{
		ret = item->data->CleanUp();
		RELEASE(item->data);
		item = item->next;
	}

	entities.clear();

	return ret;
}

void j1EntityManager::ResetEntities()
{
	p2List_item<j1Entity*>* item;
	item = entities.start;

	while (item != NULL)
	{
		item->data->Reset();
		item = item->next;
	}
}

bool j1EntityManager::Load(pugi::xml_node& module_node)
{
	bool ret = true;
	p2List_item<j1Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Load(module_node);
		item = item->next;
	}

	return ret;
}

bool j1EntityManager::Save(pugi::xml_node& module_node) const
{
	bool ret = true;
	p2List_item<j1Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Save(module_node);
		item = item->next;
	}

	return ret;
}

j1Entity * j1EntityManager::CreateEntity(Type type)
{
	j1Entity* ret = nullptr;
	switch (type) 
	{
	case PLAYER:
		ret = new j1Player(); 
		break;
	case GROUNDED_ENEMY:
		ret = new j1GroundedEnemy();
		break;
	case FLYING_ENEMY:
		ret = new j1FlyingEnemy();
		break;
	}
	ret->type = type;
	ret->Awake(module_node.child(ret->TypeToString().GetString()));
	ret->Start();

	if (ret != nullptr)
		entities.add(ret);

	return ret;
}

bool j1EntityManager::DestroyEntity(j1Entity * entity)
{
	bool ret = false;
	p2List_item<j1Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == false)
	{
		if(item->data == entity)
		{
			ret = item->data->CleanUp();
			RELEASE(item->data);
			entities.del(item);
			return ret;
		}

		item = item->next;
	}

	return ret;
}
