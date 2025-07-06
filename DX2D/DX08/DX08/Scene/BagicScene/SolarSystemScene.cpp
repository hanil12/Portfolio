#include "framework.h"
#include "SolarSystemScene.h"

SolarSystemScene::SolarSystemScene()
{
	_sun = make_shared<Planet>(L"Resource/Texture/sun.png");
	_sun->SetPos(Vector2(CENTER_X, CENTER_Y));
	_sun->SetScale(Vector2(0.3f, 0.3f));

	_earth = make_shared<Planet>(L"Resource/Texture/earth.png");
	_earth->SetParent(_sun->GetVirtualTransform());
	_earth->SetScale(Vector2(0.1f, 0.1f));
	_earth->SetPos(Vector2(900, 0));
}

SolarSystemScene::~SolarSystemScene()
{
}

void SolarSystemScene::Init()
{
	_sun->SetPos(Vector2(CENTER_X, CENTER_Y));
}

void SolarSystemScene::Update()
{
	Vector2 lerp = LERP(_sun->GetTransform()->GetPos(), MOUSE_POS, DELTA_TIME);
	_sun->SetPos(lerp);

	_sun->Update();
	_earth->Update();
}

void SolarSystemScene::Render()
{
	_sun->Render();
	_earth->Render();
}

void SolarSystemScene::PostRender()
{
	if (ImGui::Button("NextScene", { 100,100 }))
	{
		SCENE->SetScene("TutorialScene");
	}
}
