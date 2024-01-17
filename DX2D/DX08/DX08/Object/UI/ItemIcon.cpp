#include "framework.h"
#include "ItemIcon.h"

ItemIconButton::ItemIconButton()
{
	_sprite = make_shared<Sprite>(L"Resource/Texture/Item_11x5.png", Vector2(11, 5), Vector2(55,55));

	_button = make_shared<Button>(Vector2(55, 55));
	_button->SetParent(_sprite->GetTransform());
	// TODO : CallBack Àû¿ë
}

ItemIconButton::~ItemIconButton()
{
}

void ItemIconButton::Update()
{
	_sprite->Update();
	_button->Update();
}

void ItemIconButton::Render()
{
	_sprite->Render();
	_button->PostRender();
}
