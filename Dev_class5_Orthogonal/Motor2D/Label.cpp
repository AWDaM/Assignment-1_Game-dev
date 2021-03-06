#include "Label.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "Window.h"
#include "j1Textures.h"


Label::Label(SDL_Rect& position, iPoint Labelrelativepos, p2SString fontPath, SDL_Color textColor, p2SString label, int size) : Labelrelativepos(Labelrelativepos), textColor(textColor)
{
	font = App->font->Load(fontPath.GetString(), size);
	fontTexture = App->font->Print(label.GetString(), textColor, font);

}

Label::~Label()
{
}

bool Label::LabelPreUpdate()
{

	return true;
}

bool Label::LabelUpdate(float dt)
{

	return true;
}

bool Label::LabelPostUpdate()
{

	return true;
}

bool Label::LabelDraw(float dt)
{
	bool ret = true;
	if (window != nullptr && !window->active)
		return true;

	iPoint tmp = App->render->ScreenToWorld(position.x + Labelrelativepos.x, position.y + Labelrelativepos.y);
	
	if (!In_window)
		ret = App->render->Blit(fontTexture, tmp.x, tmp.y);
	else
		ret = App->render->Blit(fontTexture, tmp.x + winElement->relativePosition.x, tmp.y + winElement->relativePosition.y);

}


bool Label::LabelCleanUp()
{
	App->font->CloseFont(font);
	SDL_DestroyTexture(fontTexture);
	return true;
}

void Label::ChangeText(p2SString* newText)
{
	SDL_DestroyTexture(fontTexture);
	fontTexture = App->font->Print(newText->GetString(), textColor, font);
}

Label::Label()
{
}


