#include "UIAnimation.h"
#include "UIClock.h"
#include "j1Render.h"


UIClock::UIClock()
{
}

UIClock::UIClock(SDL_Rect & pos, p2List<Animation>& animations,bool draggable): UIElement(pos,draggable)
{
	image=App->gui->GetAtlas();
	default = animations.At(0)->data;
	slowing = animations.At(1)->data;
	stopped = animations.At(2)->data;
	fastening = animations.At(3)->data;
	oncooldown = animations.At(4)->data;

	current = &default;
}


UIClock::~UIClock()
{
}

bool UIClock::Draw(float dt)
{
	int x = position.x;
	int y = position.y;
	iPoint tmp = App->render->ScreenToWorld(x, y);
	return App->render->Blit(image, tmp.x,tmp.y ,&current->GetCurrentFrame(dt));
}

bool UIClock::HandleAnimation(int newstate)
{
	switch (newstate)
	{
	case(NORMAL):
		current = &default;
		ResetAnimations();
		LOG("DEFAULT");
		break;
	case(SLOWING):
		current = &slowing;
		LOG("SLOWING");
		break;
	case(STOPPED):
		current = &stopped;
		LOG("STOPPED");
		break;
	case(FASTENING):
		current = &fastening;
		LOG("FASTENING");
		break;
	case(RECENTLY_STOPPED):
		LOG("COOLDOWN");
		current = &oncooldown;
		break;
	}
	return true;
}

void UIClock::ResetAnimations()
{
	default.Reset();
	slowing.Reset();
	stopped.Reset();
	fastening.Reset();
	oncooldown.Reset();
	current = &default;
}

bool UIClock::CleanUp()
{
	SDL_DestroyTexture(image);
	current = nullptr;
	return true;
}
