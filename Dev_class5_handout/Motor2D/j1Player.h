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
	

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void Jump();

public:

	Collider* collider;
	Collider* colliderground;
	Collider* colliderground2;
	bool allowtime = true;
	Uint32 time = 0;
	bool jumping = false;
	bool flip = false;

public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation;
	Animation idle;
	Animation run;
	Animation jump;
	Animation fall;
	Animation death;
	Animation slide;
	iPoint position;
	iPoint sprite_distance;

};

#endif //__j1PLAYER_H__