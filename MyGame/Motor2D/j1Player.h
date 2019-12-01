#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Entity.h"
#include "p2List.h"
#include "SDL\include\SDL_rect.h"

struct SDL_Texture;
struct SDL_Rect;


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
			if(loop == false)
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

class j1Player : public j1Entity
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

	void ApplyGravity(float dt);
	void Jump(float dt);
	SDL_Rect* CheckCollisions(Direction direction);
	bool HasPlayerDied();
	bool HasPlayerWon();
	bool Draw(float dt);
	void GodMode(float dt);

public:
	fPoint player_position;
	bool god_mode;
	float gravity;
	fPoint player_speed;
	
private:
	p2SString player_folder;
	SDL_Texture* player_texture;
	SDL_Texture* die_texture;
	SDL_Rect player_rect;

	float frame_time = 0.0f;
	bool flip = true;
	bool dead = false;

	AnimationStruct idle_animation;
	AnimationStruct run_animation;
	AnimationStruct jump_animation;
	AnimationStruct* current_animation;
	AnimationStruct die_animation;

	void ChooseAnimation(float dt);
};

#endif __j1PLAYER_H__