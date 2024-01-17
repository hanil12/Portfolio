#pragma once
class Store : public Inventory
{
public:
	Store();
	virtual ~Store();

	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void SetInventory(shared_ptr<Inventory> inven) { _inventory = inven; }
	void SetStoreItems();

	void Buy();
	void Sell();

private:
	weak_ptr<Inventory> _inventory;
	shared_ptr<Button> _buyButton;
	shared_ptr<Button> _sellButton;
};

