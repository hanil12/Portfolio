#include "framework.h"
#include "BowScene.h"

int BowScene::_hp = 10;
BowScene::BowScene()
{
	_bow = make_shared<Bow_Bow>();

	_quad = make_shared<Quad>(L"Resource/Texture/monster.png");
	_quad->GetTransform()->GetScale() *= 0.7f;
	_quad->GetTransform()->SetPosition({ CENTER_X + 200, CENTER_Y + 200 });

	_col = make_shared<CircleCollider>(100);
	_col->GetTransform()->SetParent(_quad->GetTransform());

	_bow->SetTarget(_col);
}

BowScene::~BowScene()
{
}

void BowScene::Update()
{
	_bow->GetTransform()->GetAngle() = (MOUSE_POS - _bow->GetTransform()->GetPos()).Angle();

	_bow->Update();

	// Boss
	_quad->Update();
	_col->Update();
}

void BowScene::Render()
{
	// Boss
	_quad->Render();
	_col->Render();

	_bow->Render();
}

void BowScene::PostRender()
{
	//ImGui::SliderFloat("BowX", &_bow->GetTransform()->GetPos().x, 0, WIN_WIDTH);
	//ImGui::SliderFloat("BowY", &_bow->GetTransform()->GetPos().y, 0, WIN_HEIGHT);

	ImGui::SliderInt("hp", &_hp, 0, 10);
}
