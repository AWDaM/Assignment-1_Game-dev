#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1SceneChange.h"
#include "j1SceneSwitch.h"
#include "j1IntroScene.h"
#include "j1Gui.h"

j1IntroScene::j1IntroScene()
{
	name.create("introscene");
}


j1IntroScene::~j1IntroScene()
{
}

bool j1IntroScene::Awake()
{
	return true;
}

bool j1IntroScene::Start()
{
	p2SString name("gui/background.png");
	SDL_Rect rect = { 0,0,0,0 };
	App->gui->AddImage_From_otherFile(rect, { 0,0 }, name);
	return true;
}

bool j1IntroScene::PreUpdate()
{
	return true;
}

bool j1IntroScene::Update(float dt)
{
	
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->sceneswitch->SwitchScene(App->scene,this);
	}
		

	return true;
}

bool j1IntroScene::PostUpdate()
{
	return true;
}

bool j1IntroScene::CleanUp()
{
	return true;
}

bool j1IntroScene::OnEvent(UIElement * element, int eventType)
{
	return true;
}