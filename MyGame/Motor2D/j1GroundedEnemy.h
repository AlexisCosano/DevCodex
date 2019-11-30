#ifndef __j1GROUNDED_ENEMY_H__
#define __j1GROUNDED_ENEMY_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2List.h"
#include "j1Entity.h"
#include "SDL\include\SDL_rect.h"

struct SDL_Texture;
struct SDL_Rect;


enum CollisionDirection
{
	GE_BOTTOM = 0,
	GE_TOP,
	GE_RIGHT,
	GE_LEFT
};

struct AnimationStruct
{
	p2SString name;
	SDL_Rect animation_rect[7];
	int current_frame = 0;
	int max_frame;
	bool loop = true;
	float max_frame_time = 0.14f;

	int GetNextFrame()
	{
		current_frame++;

		if (current_frame >= max_frame)
		{
			if (loop == false)
			{
				current_frame = max_frame - 1;
				return current_frame;
			}
			current_frame = 0;
		}

		return current_frame;
	}

	SDL_Rect GetCurrentFrame()
	{
		return animation_rect[current_frame];
	}
};

class j1GroundedEnemy : public j1Entity
{
public:

	j1GroundedEnemy();

	// Destructor
	virtual ~j1GroundedEnemy();

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

	void ApplyGravity(float dt);
	SDL_Rect* CheckCollisions(CollisionDirection direction);
	void Draw(float dt);

public:
	iPoint entity_position;
	fPoint entity_speed;
	float gravity;

private:
	p2SString grounded_enemy_folder;
	SDL_Texture* grounded_enemy_texture;
	SDL_Texture* die_texture;
	SDL_Rect grounded_enemy_rect;

	float frame_time = 0.0f;
	bool flip = true;
	bool dead = false;

	AnimationStruct idle_animation;
	AnimationStruct run_animation;
	AnimationStruct jump_animation;
	AnimationStruct* current_animation;
	AnimationStruct die_animation;

	void ChooseAnimation();
};

#endif __j1GROUNDED_ENEMY_H__