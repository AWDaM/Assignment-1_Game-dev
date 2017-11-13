#include "j1App.h"
#include "Entity.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1SceneChange.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"


Entity::Entity(entityType type) : type(type)
{
}

Entity::~Entity()
{
}

fPoint Entity::SpeedBoundaries(fPoint originalvec)
{
	if (direction_x > 0)
	{
		if (originalvec.x > maxSpeed.x)
			originalvec.x = maxSpeed.x;
	}

	else
	{
		if (originalvec.x < direction_x*maxSpeed.x)
			originalvec.x = direction_x*maxSpeed.x;
	}

	if (originalvec.y > maxSpeed.y)
	{
		originalvec.y = maxSpeed.y;
	}
	return originalvec;
}

fPoint Entity::Collider_Overlay(fPoint originalvec)
{
	grounded = false;

	SDL_Rect CastCollider;
	CastCollider = Collider;
	CastCollider.x += originalvec.x;
	CastCollider.y += originalvec.y;

	SDL_Rect result;

	fPoint newvec = originalvec;
	for (p2List_item<ObjectsGroup*>* obj = App->map->data.objLayers.start; obj; obj = obj->next)
	{
		if (obj->data->name == ("Collisions"))
		{
			for (p2List_item<ObjectsData*>* objdata = obj->data->objects.start; objdata; objdata = objdata->next)
			{
				if (objdata->data->name == ("Floor"))
				{
					if (SDL_IntersectRect(&CastCollider, &CreateRect_FromObjData(objdata->data), &result))
					{
						newvec = AvoidCollision(newvec, result, objdata);
					}
				}
				else if (objdata->data->name == ("BGFloor")) //Only collides if the player is above the platform
				{
					if (position.y + Collider.h + colOffset.y <= objdata->data->y)
						if (SDL_IntersectRect(&CastCollider, &CreateRect_FromObjData(objdata->data), &result))
							if (result.h <= result.w || position.x + Collider.w + colOffset.x >= objdata->data->x)
								newvec.y -= result.h, BecomeGrounded();
				}
				else if (objdata->data->name == ("Dead")) //Detects when the player falls
				{
					if (SDL_IntersectRect(&CastCollider, &CreateRect_FromObjData(objdata->data), &result))
						alive = false;
				}
				else if (objdata->data->name == ("End")) //Detects when the player has finished the level
				{
					if (SDL_IntersectRect(&CastCollider, &CreateRect_FromObjData(objdata->data), &result) && !App->scenechange->fading)
						App->scene->to_end = true;
				}
				//The new trajectory of the player is adjousted for the next collision check
				if (SDL_IntersectRect(&CastCollider, &CreateRect_FromObjData(objdata->data), &result) && !App->scenechange->fading)
				{
					CastCollider.x -= (originalvec.x - newvec.x);
					CastCollider.y -= (originalvec.y - newvec.y);
				}
			}
		}
	}

	return newvec;
}

fPoint Entity::AvoidCollision(fPoint newvec, const SDL_Rect result, p2List_item<ObjectsData*>* objdata)
{
	if (newvec.y > 0)
	{
		if (position.y + Collider.h + colOffset.y <= objdata->data->y)
		{
			if (newvec.x > 0)
			{
				if (result.h <= result.w || position.x + Collider.w + colOffset.x >= objdata->data->x)
					newvec.y -= result.h, BecomeGrounded();
				else
					newvec.x -= result.w;
			}
			else if (newvec.x < 0)
			{
				if (result.h <= result.w || position.x + colOffset.x >= objdata->data->x + objdata->data->width)
					newvec.y -= result.h, BecomeGrounded();
				else
					newvec.x += result.w;
			}
			else
				newvec.y -= result.h, BecomeGrounded();
		}
		else
		{
			if (newvec.x > 0)
				newvec.x -= result.w;
			else
				newvec.x += result.w;
		}

	}
	else if (newvec.y < 0)
	{
		if (position.y + colOffset.y >= objdata->data->y + objdata->data->height)
		{
			if (newvec.x > 0)
			{
				if (result.h <= result.w ||	position.x + Collider.w + colOffset.x >= objdata->data->x)
					newvec.y += result.h;
				else
					newvec.x -= result.w;
			}
			else if (newvec.x < 0)
			{
				if (result.h <= result.w || position.x + colOffset.x <= objdata->data->x + objdata->data->width)
					newvec.y += result.h;
				else
					newvec.x += result.w;
			}
			else
				newvec.y += result.h;
		}
		else
		{
			if (newvec.x > 0)
				newvec.x -= result.w;
			else if (newvec.x < 0)
				newvec.x += result.w;
			else
				newvec.y += result.h;
		}
	}
	else
	{
		if (newvec.x > 0)
			newvec.x -= result.w;
		else if (newvec.x < 0)
			newvec.x += result.w;
	}
	return newvec;
}

SDL_Rect Entity::CreateRect_FromObjData(ObjectsData * data)
{
	SDL_Rect ret;
	ret.x = data->x;
	ret.y = data->y;
	ret.h = data->height;
	ret.w = data->width;
	return ret;
}

void Entity::AddSFX(int channel, int repeat, uint volume)
{
	App->audio->PlayFx(channel, repeat, volume);
}