#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1Collisions.h"
#include "p2Point.h"

struct SDL_Texture;

class j1Player : public j1Module
{
public:

	j1Player();
	~j1Player();

	bool Start();
	bool PostUpdate();

	//void PrepareAnimation(Animation animation, int sprites, int sprite_distance_x, int sprite_distance_y, int row, int margin = 0, float speed = 0.1);

public:

	Collider* collider;
	Collider* colliderground;
	bool collider_added = false;

public:

	SDL_Texture* graphics = nullptr;
	Animation idle;
	Animation forward;
	Animation backwards;
	Animation jump;
	Animation death;
	iPoint position;
	iPoint sprite_distance;

};

#endif //__j1PLAYER_H__