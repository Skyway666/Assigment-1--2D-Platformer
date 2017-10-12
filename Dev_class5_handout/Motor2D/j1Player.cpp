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
	position.y = 0;
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

	// wall slide left

	wallslideleft.PushBack({ 1 + sprite_distance.x * 8, 1 + sprite_distance.y * row, 547, 481 });
	wallslideleft.PushBack({ 1 + sprite_distance.x * 9, 1 + sprite_distance.y * row, 547, 481 });

	wallslideleft.speed = 0.01;
	row++;

	// falling
	for (int i = 0; i < 7; i++)
		fall.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * row, 547, 481 });

	fall.loop = false;

	// wall slide right

	wallslideright.PushBack({ 1 + sprite_distance.x * 8, 1 + sprite_distance.y * row, 547, 481 });
	wallslideright.PushBack({ 1 + sprite_distance.x * 9, 1 + sprite_distance.y * row, 547, 481 });

	wallslideright.speed = 0.01;
}

j1Player::~j1Player()
{
	App->tex->UnLoad(graphics);
}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->tex->Load("textures/SpriteSheet.png");

	SDL_Rect r{ 0, 0, 481, 547 };


	SDL_Rect ground{ r.x, r.y + 950, r.w * 20, 100 };

	SDL_Rect collider_rect{ 0, 0, r.w * 0.2, r.h * 0.2 };

	contact.x = 0;
	contact.y = 0;

	collider = App->collision->AddCollider(collider_rect, COLLIDER_PLAYER);

	gravity = 1;

	return ret;
}

// Update: draw background
bool j1Player::PostUpdate()
{
	player_x_displacement = App->map->data.player_starting_value.x - position.x;
	current_animation = &idle;
	speed.x = 0;
	speed.y = 2;

	// Moving right
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		current_animation = &run;
		flip = false;

		if (contact.x != 2)
			speed.x = 1;
	}

	// Moving left
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		current_animation = &run;
		flip = true;

		if (contact.x != 1)
			speed.x = -1;
	}

	// Sliding
	//if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//{
	//	current_animation = &slide;
	//}

	// Jumping
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		if (contact.y == 1)
		{
			jumping = true;
		}
		else if (contact.x == 1 || contact.x == 2)
		{
			walljumping = true;
		}
	}

	WallSlide();
	Jump();

	if (!walljumping)
		position.x += speed.x;

	if (contact.y != 1 && StickToWall)
		position.y += gravity / 2;
	else if (contact.y != 1)
		position.y += gravity;

	StickToWall = false;
	contact.x = 0;
	contact.y = 0;

	// Draw everything --------------------------------------

	App->render->Blit(graphics, position.x, position.y, 0.3, &current_animation->GetCurrentFrame(), flip);

	// Set camera to follow the player
	App->render->camera.x = -position.x + 400;
	App->render->camera.y = -position.y + 400;

	//Put collider next to player
	if (collider != nullptr)
	{
		collider->SetPos(position.x + 30, position.y + 30);
	}

	return true;
}

void j1Player::WallSlide()
{
	if (contact.x == 2 && contact.y != 1)
	{
		StickToWall = true;
		current_animation = &wallslideright;
		flip = false;
	}
	else if (contact.x == 1 && contact.y != 1)
	{
		StickToWall = true;
		current_animation = &wallslideleft;
		flip = false;
	}
}

void j1Player::Jump()
{
	// jump
	if (jumping)
	{
		if (allowtime)
		{
			time = SDL_GetTicks();
			allowtime = false;
			contact.y = 0;
		}

		if (SDL_GetTicks() - time <= 500 && contact.y == 0)
		{
			current_animation = &jump;
			position.y -= speed.y;
		}
		else
		{
			jumping = false;
			allowtime = true;
			jump.Reset();
		}

		if (contact.y == 1 && (contact.x == 1 || contact.x == 2))
		{
			jumping = false;
			allowtime = true;
			jump.Reset();
		}
	}
	// wall jump
	else if (walljumping)
	{
		if (allowtime)
		{
			time = SDL_GetTicks();
			allowtime = false;
			jcontact = contact.x;
			contact.x = 0;
		}

		if (SDL_GetTicks() - time <= 500 && contact.x == 0)
		{
			current_animation = &jump;
			position.y -= speed.y;

			if (jcontact == 1)
			{
				position.x += 1;
				flip = true;
			}
			else if (jcontact == 2)
				position.x -= 1;
		}
		else
		{
			walljumping = false;
			allowtime = true;
			jump.Reset();
		}

		if (contact.y == 1 || contact.x == 1 || contact.x == 2)
		{
			walljumping = false;
			allowtime = true;
			jump.Reset();
		}
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