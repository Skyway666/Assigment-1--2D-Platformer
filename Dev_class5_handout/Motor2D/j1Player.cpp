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
		//PrepareAnimation(idle, 10, sprite_distance.x, sprite_distance.y, 1, 1);

	// running forward
	for (int i = 0; i < 8; i++)
		forward.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * 2, 547, 481 });

	forward.speed = 0.03;

	// running backwards
	for (int i = 0; i < 8; i++)
		backwards.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * 2, 547, 481 });

	backwards.speed = 0.03;

	// jumping
	for (int i = 0; i < 8; i++)
		jump.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * 4, 547, 481 });
	for (int i = 8; i < 7; i++)
		jump.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * 5, 547, 481 });

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

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		if (collider != nullptr && colliderground != nullptr && collider->CheckCollision(colliderground->rect))
			jumping = true;
	}

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

	/*if (collider != nullptr && colliderground != nullptr && collider->CheckCollision(colliderground->rect))
	{
		jumping = false;
		allowtime = true;
		jump.Reset();
	}*/

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

	App->render->Blit(graphics, position.x, position.y - r.h, &r);

	if (collider != nullptr)
	{
		collider->SetPos(position.x, position.y - r.h);
		collider->SetSize(r.w, r.h);
	}

	return true;
}

void j1Player::PrepareAnimation(Animation animation, int sprites, int sprite_distance_x, int sprite_distance_y, int row, int margin, float speed)
{
	for (int i = 0; i < sprites; i++)
		animation.PushBack({ margin + sprite_distance_x * i, margin + sprite_distance_y * row, 547, 481 });

	animation.speed = speed;
}