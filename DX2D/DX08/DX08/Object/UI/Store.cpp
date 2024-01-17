#include "framework.h"
#include "Store.h"

Store::Store()
{
	_buyButton = make_shared<Button>(L"Resource/Texture/UI/Button.png");
	_buyButton->SetParent(_pannel->GetTransform());
	_buyButton->SetPostion(Vector2(-50, -180));
	_buyButton->SetEvent(std::bind(&Store::Buy, this));

	_sellButton = make_shared<Button>(L"Resource/Texture/UI/Button.png");
	_sellButton->SetParent(_pannel->GetTransform());
	_sellButton->SetPostion(Vector2(50, -180));
	_sellButton->SetEvent(std::bind(&Store::Sell, this));

	SetStoreItems();
}

Store::~Store()
{
}

void Store::Update()
{
	Inventory::Update();
	_buyButton->Update();
	_sellButton->Update();
}

void Store::Render()
{
	Inventory::Render();
	_buyButton->PostRender();
	_sellButton->PostRender();
}

void Store::PostRender()
{
	if (_curIndex >= 0 && _curIndex < 9)
	{
		ItemInfo temp = _itemDates[_curIndex];
		ImGui::Text("Store Selected Item");
		ImGui::Text(temp.name.c_str());
		ImGui::SliderInt("index", &_curIndex, 0, 8);
	}
}

void Store::SetStoreItems()
{
	int index = 0;
	for (auto pair : DATA_M->GetItemTable())
	{
		if (index >= _itemDates.size())
			return;

		if (pair.second.name == "")
			continue;

		_itemDates[index] = pair.second;
		index++;
	}
}

void Store::Buy()
{
	if (!ValidIndex())
		return;

	if (!_inventory.expired())
	{
		if(_inventory.lock()->AddItem(_itemDates[_curIndex].name))
			_inventory.lock()->SubMoney(_itemDates[_curIndex].price);
	}
}

void Store::Sell()
{
	if (_inventory.expired())
		return;

	_inventory.lock()->SellItem();
}
