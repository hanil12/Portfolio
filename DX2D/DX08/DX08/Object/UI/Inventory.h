#pragma once
class Inventory
{
public:
	Inventory();
	virtual ~Inventory();

	virtual void Update();
	virtual void Render();
	virtual void PostRender();

	void SetCurIndex(int value);

	void Set();
	void SetPannelPos(Vector2 pos) { _pannel->GetTransform()->SetPosition(pos); Update(); Set(); }

	virtual bool AddItem(string name) final;
	virtual void SellItem(string name) final;
	virtual void SellItem() final;
	virtual bool AddMoney(UINT amout) final;
	virtual bool SubMoney(UINT amout) final;

	bool ValidIndex() { return _curIndex >= 0 && _curIndex < 9; }

protected:
	shared_ptr<Quad> _pannel;

	Vector2 _slotOffset;
	vector<shared_ptr<Slot>> _slots;
	vector<ItemInfo> _itemDates;

	vector<shared_ptr<ItemIconButton>> _icons;

	int _curIndex = -1;

private:
	int _money = 10000;
};

