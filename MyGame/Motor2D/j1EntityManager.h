#ifndef __j1ENTITY_MANAGER_H__
#define __j1ENTITY_MANAGER_H__

#include "j1Module.h"
#include "p2List.h"
#include "j1Entity.h"

class j1EntityManager : public j1Module
{
public:
	// Constructor
	j1EntityManager();

	// Destructor
	virtual ~j1EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node& module_node);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before all Updates
	bool CleanUp();

	void ResetEntities();

	bool Load(pugi::xml_node& module_node);
	bool Save(pugi::xml_node& module_node) const;

	j1Entity* CreateEntity(Type type);
	bool DestroyEntity(j1Entity* entity);

public:
	p2List<j1Entity*> entities;

private:
	pugi::xml_node module_node;
};

#endif __j1ENTITY_MANAGER_H__