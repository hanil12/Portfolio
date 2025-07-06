#include "framework.h"
#include "InventoryScene.h"

InventoryScene::InventoryScene()
{
	_inventory = make_shared<Inventory>();
	_inventory->SetPannelPos(Vector2(CENTER_X + 300,CENTER_Y));

	_store = make_shared<Store>();
	_store->SetPannelPos(Vector2(CENTER_X - 300, CENTER_Y));
	_store->SetInventory(_inventory);
}

InventoryScene::~InventoryScene()
{
}

void InventoryScene::Update()
{
	_inventory->Update();
	_store->Update();
}

void InventoryScene::Render()
{
}

void InventoryScene::PostRender()
{
	_inventory->Render();
	_inventory->PostRender();

	if (ImGui::Button("BUY", { 100,100 }))
	{
		_inventory->AddItem("Sword");
	}

	if (ImGui::Button("Sell", { 100,100 }))
	{
		_inventory->SellItem();
	}

	_store->Render();
	_store->PostRender();
}
