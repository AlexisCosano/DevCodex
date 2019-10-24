#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2List.h"
#include "SDL\include\SDL_rect.h"

struct SDL_Texture;
struct SDL_Rect;

class j1Player : public j1Module
{
public:

	j1Player();

	// Destructor
	virtual ~j1Player();

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

	void ApplyGravity();

	void Draw();

public:
	p2Point<float> player_position;

	float gravity = 9.8f;
	p2Point<float> player_speed;
	
private:
	p2SString player_folder;
	SDL_Texture* player_texture;
	SDL_Rect player_rect;
};
#endif //__j1PLAYER_H__