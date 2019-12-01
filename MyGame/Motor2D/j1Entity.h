#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"

struct SDL_Texture;

enum Direction
{
	TOP,
	LEFT,
	BOTTOM,
	RIGHT
};

enum Type
{
	PLAYER,
	GROUNDED_ENEMY,
	FLYING_ENEMY
};

class j1Entity
{
public:
	// Constructor
	j1Entity() {};

	// Destructor
	virtual ~j1Entity() {};
	
	// Called before render is available
	virtual bool Awake(pugi::xml_node& module_node)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	//virtual load/save function
	virtual bool Load(pugi::xml_node& module_node)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node& module_node)const
	{
		return true;
	}

	virtual p2SString TypeToString()
	{
		switch (type)
		{
		case PLAYER:
			return "player";
		case GROUNDED_ENEMY:
			return"grounded_enemy";
		case FLYING_ENEMY:
			return"flying_enemy";
		default:
			return "default";
		}
	}

public:
	Type type;
};

#endif __j1ENTITY_H__