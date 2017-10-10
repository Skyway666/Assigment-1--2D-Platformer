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

	// idle animation
	idle.PushBack({ 0, 0, 450, 480 });
	idle.PushBack({ 570, 0, 450, 480 });
	idle.speed = 0.2f;

	// running forward
	forward.PushBack({ 78, 131, 60, 88 });
	forward.PushBack({ 162, 128, 64, 92 });
	forward.speed = 0.1f;

	// running backwards
	backwards.PushBack({ 78, 131, 60, 88 });
	backwards.PushBack({ 162, 128, 64, 92 });
	backwards.speed = 0.1f;

	// jumping
	jump.PushBack({});
	jump.PushBack({});
}

j1Player::~j1Player()
{}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->tex->Load("maps/SpriteSheet.png");
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

	// Draw everything --------------------------------------
	SDL_Rect r = current_animation->GetCurrentFrame();

	App->render->Blit(graphics, position.x, position.y - r.h, &r);

	return true;
}