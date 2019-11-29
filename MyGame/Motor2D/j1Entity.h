#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"

struct SDL_Texture;

enum State
{
	IDLE,
	WALK_RIGHT,
	WALK_LEFT,
	FLY_RIGHT,
	FLY_LEFT
};

enum Direction
{
	E_UP,
	E_LEFT,
	E_DOWN,
	E_RIGHT
};

enum Type
{
	PLAYER,
	ENEMY_GROUND,
	ENEMY_FLY
};

class j1Entity
{
public:
	// Constructor
	j1Entity() {};

	// Destructor
	virtual ~j1Entity() {};
	
	// Called before render is available
	virtual bool Awake()
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

public:
	Type entity_type;
	State entity_state;
	iPoint entity_position;
	fPoint entity_speed;
	Direction entity_direction;
};

#endif __j1ENTITY_H__