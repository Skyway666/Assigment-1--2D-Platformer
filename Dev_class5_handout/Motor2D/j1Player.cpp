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
	idle.PushBack({ 7, 14, 60, 90 });
	idle.PushBack({ 95, 15, 60, 89 });
	idle.PushBack({ 184, 14, 60, 90 });
	idle.PushBack({ 276, 11, 60, 93 });
	idle.PushBack({ 366, 12, 60, 92 });
	idle.speed = 0.2f;

	// running forward
	forward.PushBack({ 78, 131, 60, 88 });
	forward.PushBack({ 162, 128, 64, 92 });
	forward.PushBack({ 259, 128, 63, 90 });
	forward.PushBack({ 352, 128, 54, 91 });
	forward.PushBack({ 432, 131, 50, 89 });
	forward.speed = 0.1f;

	// running backwards
	backwards.PushBack({ 78, 131, 60, 88 });
	backwards.PushBack({ 162, 128, 64, 92 });
	backwards.PushBack({ 259, 128, 63, 90 });
	backwards.PushBack({ 352, 128, 54, 91 });
	backwards.PushBack({ 432, 131, 50, 89 });
	backwards.speed = 0.1f;
}

j1Player::~j1Player()
{}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->tex->Load("SpriteSheet.png");
	return ret;
}

// Update: draw background
bool j1Player::Update()
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