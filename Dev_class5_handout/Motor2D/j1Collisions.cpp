#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1Player.h"

j1Collisions::j1Collisions()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_BONE][COLLIDER_BONE] = false;
	matrix[COLLIDER_BONE][COLLIDER_DEADLY] = false;
	matrix[COLLIDER_BONE][COLLIDER_PLAYER] = true;

	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_BONE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_DEADLY] = true;

	matrix[COLLIDER_DEADLY][COLLIDER_DEADLY] = false;
	matrix[COLLIDER_DEADLY][COLLIDER_BONE] = false;
	matrix[COLLIDER_DEADLY][COLLIDER_PLAYER] = true;
}

// Destructor
j1Collisions::~j1Collisions()
{}

bool j1Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

// Called before render is available
bool j1Collisions::PostUpdate()
{
	Collider* c;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
			// skip empty and player colliders
			if (colliders[i] == nullptr || colliders[i]->type == COLLIDER_NONE || colliders[i]->type == COLLIDER_PLAYER)
				continue;

			if (colliders[i]->type == COLLIDER_BONE || colliders[i]->type == COLLIDER_DEADLY)
			{
				c = colliders[i];

				if (App->player->collider->CheckCollision(c->rect) == true)
				{

					if (matrix[App->player->collider->type][c->type])
						App->player->collider->OnCollision(App->player->collider, c);


					if (matrix[c->type][App->player->collider->type])
						c->OnCollision(c, App->player->collider);

				}
			}
			else if (colliders[i]->type == COLLIDER_GROUND && App->player->collider->rect.y + App->player->collider->rect.h > colliders[i]->rect.y)
			{
				colliders[i]->type = COLLIDER_WALL;
			}
			else if (colliders[i]->type == COLLIDER_WALL && App->player->collider->rect.y + App->player->collider->rect.h < colliders[i]->rect.y + colliders[i]->rect.h && App->player->collider->rect.y + App->player->collider->rect.h < colliders[i]->rect.y)
			{
				colliders[i]->type = COLLIDER_GROUND;
			}
	}

	DebugDraw();

	return true;
}

void j1Collisions::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha, false);
			break;
		case COLLIDER_PLAYER: // cian
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha, false);
			break;
		case COLLIDER_WALL: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha, false);
			break;
		case COLLIDER_GROUND: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha, false);
			break;
		case COLLIDER_DEADLY: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha, false);
			break;
		}
	}
}

// Called before quitting
bool j1Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Collisions::AddCollider(SDL_Rect rect, COLLIDER_TYPE type)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type);
			break;
		}
	}

	return ret;
}

void j1Collisions::Erase_Non_Player_Colliders()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->type != COLLIDER_PLAYER)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	if (r.y + r.h > rect.y && r.y < rect.y + rect.h && r.x + r.w > rect.x && r.x < rect.x + rect.w)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool Collider::WillCollideX(const SDL_Rect& r, int speed_x) const
{
	if (r.y + r.h > rect.y && r.y < rect.y + rect.h && r.x + r.w > rect.x + speed_x && r.x < rect.x + rect.w + speed_x)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool Collider::WillCollideY(const SDL_Rect& r, int speed_y) const
{
	if (r.y + r.h > rect.y - speed_y && r.y < rect.y + rect.h + speed_y && r.x + r.w > rect.x && r.x < rect.x + rect.w)
	{
		return true;
	}

	else
	{
		return false;
	}
}