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

	bool Awake(pugi::xml_node& conf);

	bool Start();
	bool PostUpdate();
	

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void WallSlide();
	void Jump();
	void Slide();

public:

	Collider* collider;

	SDL_Rect rect_after_sliding;
	int frames = 0;
	int time = 0;
	bool allowtime = true;

	float player_height_before_sliding;
	bool jumping = false;
	bool walljumping = false;
	bool flip = false;
	bool StickToWall = false;
	bool dead = false;
	bool sliding = false;
	bool win = false;
	int jcontact = 0; // for Jump() function

	iPoint contact; // y = 0 -> none, y = 1 -> ground, y = 2 -> ceiling /// x = 0 -> none, x = 1 -> left, x = 2 -> right
	fPoint speed;
	fPoint position;
	iPoint sprite_distance;
	fPoint speed_modifier;
	float gravity;
	float player_x_displacement;
	int jump_time;
	int slide_time;
	int walljump_time;

public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation;
	Animation idle;
	Animation run;
	Animation jump;
	Animation fall;
	Animation death;
	Animation slide;
	Animation wallslideright;
	Animation wallslideleft;

};

#endif //__j1PLAYER_H__