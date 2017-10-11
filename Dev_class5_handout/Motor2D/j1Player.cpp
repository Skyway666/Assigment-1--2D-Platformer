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
	return ret;
}

// Update: draw background
bool j1Player::PostUpdate()
{
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
	SDL_Rect r = current_animation->GetCurrentFrame();
	SDL_Rect ground{r.x, r.y + 200, r.w * 20, r.y / 2};
	if (!collider_added)
	{
		collider = App->collision->AddCollider(r, COLLIDER_PLAYER);
		colliderground = App->collision->AddCollider(ground, COLLIDER_WALL);
		collider_added = true;
	}

		App->render->Blit(graphics, position.x, position.y - r.h, &r, flip);

	if (collider != nullptr)
	{
		collider->SetPos(position.x, position.y - r.h);
		collider->SetSize(r.w, r.h);
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