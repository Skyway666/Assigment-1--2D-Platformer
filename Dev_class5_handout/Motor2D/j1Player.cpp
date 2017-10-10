#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"

#include "p2Log.h"


j1Player::j1Player()
{
	position.x = 100;
	position.y = 220;

	sprite_distance.x = 548;
	sprite_distance.y = 482;

	// idle animation
	for (int i = 0; i < 10; i++)
		idle.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y, 547, 481 });

	idle.speed = 0.1;

	// running forward
	for (int i = 0; i < 8; i++)
		forward.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * 2, 547, 481 });

	forward.speed = 0.1;

	// running backwards
	for (int i = 0; i < 8; i++)
		backwards.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * 2, 547, 481 });

	backwards.speed = 0.1;

	// jumping
	for (int i = 0; i < 8; i++)
		jump.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * 4, 547, 481 });

	jump.speed = 0.1;
	jump.loop = false;
}

j1Player::~j1Player()
{}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->tex->Load("textures/SpriteSheet.png");
	return ret;
}

// Update: draw background
bool j1Player::PostUpdate()
{
	Animation* current_animation = &idle;

	int speed = 1;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		current_animation = &forward;
		position.x += speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		current_animation = &backwards;
		position.x -= speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		current_animation = &jump;
	}

	// Draw everything --------------------------------------
	SDL_Rect r = current_animation->GetCurrentFrame();

	if (!collider_added)
	{
		collider = App->collision->AddCollider(r, COLLIDER_PLAYER);
		collider_added = true;
	}

	App->render->Blit(graphics, position.x, position.y - r.h, &r);

	if (collider != nullptr)
	{
		collider->SetPos(position.x, position.y - r.h);
		collider->SetSize(r.w, r.h);
	}

	return true;
}

/*void j1Player::PrepareAnimation(Animation animation, int sprites, int sprite_distance_x, int sprite_distance_y, int row, int margin = 0, float speed = 0.1)
{
	for (int i = 0; i < sprites; i++)
		animation.PushBack({ margin + sprite_distance_x * i, margin + sprite_distance_y, 547, 481 });

	animation.speed = speed;
}*/