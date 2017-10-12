#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Map.h"
#include "p2Log.h"


j1Player::j1Player()
{
	name.create("player");
	position.x = 0;
	position.y = 1600;
	int row = 0;

	sprite_distance.x = 548;
	sprite_distance.y = 482;

	// dying
	for (int i = 0; i < 10; i++)
		death.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * row, 547, 481 });

	death.loop = false;
	row++;

	// idle animation
	for (int i = 0; i < 10; i++)
		idle.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * row, 547, 481 });
	
	idle.speed = 0.06;
	row++;

	// running
	for (int i = 0; i < 8; i++)
		run.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * row, 547, 481 });

	row++;

	// sliding
	for (int i = 0; i < 10; i++)
		slide.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * row, 547, 481 });

	row++;

	// jumping
	for (int i = 0; i < 8; i++)
		jump.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * row, 547, 481 });

	jump.loop = false;
	row++;

	// falling
	for (int i = 0; i < 7; i++)
		fall.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * row, 547, 481 });

	fall.loop = false;
	row++;
}

j1Player::~j1Player()
{}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->tex->Load("textures/SpriteSheet.png");

	SDL_Rect r;
	r.w = 481;
	r.h = 547;
	r.x = 0;
	r.y = 0;

	SDL_Rect ground{ r.x, r.y+ 1408, r.w * 20, 100 };

	SDL_Rect collider_rect;
	collider_rect.x = 0;
	collider_rect.y = 0;
	collider_rect.w = r.w* 0.2;
	collider_rect.h = r.h * 0.2;

	collider = App->collision->AddCollider(collider_rect, COLLIDER_PLAYER);
	colliderground = App->collision->AddCollider(ground, COLLIDER_WALL);
	return ret;
}

// Update: draw background
bool j1Player::PostUpdate()
{

	player_x_displacement = App->map->data.player_starting_value.x - position.x;
	current_animation = &idle;
	int speed = 0;

	// Moving right
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		current_animation = &run;
		speed = 1;
		flip = false;
	}

	// Moving left
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		current_animation = &run;
		speed = -1;
		flip = true;
	}

	// Sliding
	//if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//{
	//	current_animation = &slide;
	//}

	// Jumping
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (collider != nullptr && colliderground != nullptr && collider->CheckCollision(colliderground->rect))
			jumping = true;
	}

	Jump();
	position.x += speed;

	if (collider != nullptr && colliderground != nullptr && !collider->CheckCollision(colliderground->rect))
		position.y += 1;

	// Draw everything --------------------------------------

	App->render->Blit(graphics, position.x, position.y,0.3, &current_animation->GetCurrentFrame(), flip);

	// Set camera to follow the player
	App->render->camera.x = -position.x + 400;
	App->render->camera.y = -position.y + 400;

	//Put collider next to player
	if (collider != nullptr)
	{
		collider->SetPos(position.x + 30, position.y+30);
	}

	return true;
}

void j1Player::Jump()
{
	if (jumping && allowtime)
	{
		time = SDL_GetTicks();
		allowtime = false;
	}

	if (jumping && SDL_GetTicks() - time <= 500)
	{
		current_animation = &jump;
		position.y -= 2;
	}
	else
	{
		jumping = false;
		allowtime = true;
		jump.Reset();
	}
}

bool j1Player::Load(pugi::xml_node& data)
{
	position.x = data.child("player_position").attribute("x").as_float();
	position.y = data.child("player_position").attribute("y").as_float();

	return true;
}

// Save Game State
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node player = data.append_child("player_position");

	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;

	return true;
}