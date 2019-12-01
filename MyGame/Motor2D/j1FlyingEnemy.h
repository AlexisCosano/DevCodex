#ifndef __j1FLYING_ENEMY_H__
#define __j1FLYING_ENEMY_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2List.h"
#include "j1Entity.h"
#include "SDL\include\SDL_rect.h"

struct SDL_Texture;
struct SDL_Rect;

class j1FlyingEnemy : public j1Entity
{
public:

	j1FlyingEnemy();

	// Destructor
	virtual ~j1FlyingEnemy();

	// Called before the first frame
	bool Start();

	// Called before render is available
	bool Awake(pugi::xml_node& module_node);

	// Update
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Save & Load
	bool Save(pugi::xml_node& module_node) const;
	bool Load(pugi::xml_node& module_node);

	void Draw(float dt);

public:


private:

};

#endif __j1FLYING_ENEMY_H__