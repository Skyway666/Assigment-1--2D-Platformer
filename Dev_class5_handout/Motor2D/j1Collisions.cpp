#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1Player.h"
#include "j1Map.h"

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
bool j1Collisions::Update(float dt)
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
					{
						if (c->type == COLLIDER_DEADLY)
						{
							App->player->dead = true;
						}
						if (c->type == COLLIDER_BONE)
						{
							if(App->map->map == 0)
							{ 
								//App->map->Load("Level 2 final"); //Map 2 still not done
								App->map->map = 1;
							}
							if (App->map->map == 1)
							{
								//The player would win.
							}
						}
					}

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
			
			if (colliders[i]->type == COLLIDER_GROUND && colliders[i]->WillCollideGround(App->player->collider->rect, 1))
				App->player->contact.y = 1;

			if (colliders[i]->type == COLLIDER_WALL && colliders[i]->WillCollideCeiling(App->player->collider->rect, 1))
				App->player->contact.y = 2;
			
			if (colliders[i]->type == COLLIDER_WALL && colliders[i]->WillCollideLeft(App->player->collider->rect, 1))
				App->player->contact.x = 1;

			if (colliders[i]->type == COLLIDER_WALL && colliders[i]->WillCollideRight(App->player->collider->rect, 1))
				App->player->contact.x = 2;
	}

	DebugDraw();

	return true;
}

void j1Collisions::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
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
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha, true);
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

bool Collider::WillCollideLeft(const SDL_Rect& r, int distance) const
{
	if (r.y + r.h > rect.y && r.y < rect.y + rect.h && r.x < rect.x + rect.w + distance && r.x + r.w > rect.x)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool Collider::WillCollideRight(const SDL_Rect& r, int distance) const
{
	if (r.y + r.h > rect.y && r.y < rect.y + rect.h && r.x + r.w > rect.x - distance && r.x < rect.x + rect.w)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool Collider::WillCollideGround(const SDL_Rect& r, int distance) const
{
	if (r.y < rect.y + rect.h && r.y + r.h > rect.y - distance && r.x + r.w > rect.x && r.x < rect.x + rect.w)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool Collider::WillCollideCeiling(const SDL_Rect& r, int distance) const
{
	if (r.y + r.h > rect.y && r.y < rect.y + rect.h + distance && r.x + r.w > rect.x && r.x < rect.x + rect.w)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool j1Collisions::WillCollideAfterSlide(const SDL_Rect& r, int distance) const
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty and player colliders
		if (colliders[i] == nullptr || colliders[i]->type == COLLIDER_NONE || colliders[i]->type == COLLIDER_PLAYER)
			continue;

		if (colliders[i]->type == COLLIDER_WALL && colliders[i]->WillCollideCeiling(r, distance))
		{
			return true;
		}
	}

	return false;
}