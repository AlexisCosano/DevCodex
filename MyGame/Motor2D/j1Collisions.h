#ifndef __j1COLLISIONS_H__
#define __j1COLLISIONS_H__

#include "j1Module.h"
#include "p2List.h"

struct SDL_Rect;



class j1Collisions : public j1Module
{
public:
	j1Collisions();

	// Destructor
	virtual ~j1Collisions();

	// Called before render is available
	bool Awake(pugi::xml_node& module_node);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	bool CheckCollision(SDL_Rect r1, SDL_Rect r2) const;
	void NoWalkable(SDL_Rect collider);
	void TriggerDeath(SDL_Rect death);
	void TriggerWin(SDL_Rect victory);
	void ClearColliders();

	// Save & Load
	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);

public:
	p2List<SDL_Rect> no_walkable_tiles;
	p2List<SDL_Rect> death_triggers;
	p2List<SDL_Rect> win_triggers;
};

#endif // __j1COLLISIONS_H__