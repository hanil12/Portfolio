#pragma once
class ItemIconButton
{
public:
	ItemIconButton();
	~ItemIconButton();

	void Update();
	void Render();

	void SetPostion(Vector2 pos) { _sprite->GetTransform()->SetPosition(pos); }
	void SetScale(Vector2 scale) { _sprite->GetTransform()->SetScale(scale); }
	void SetItem(const ItemInfo& info) { _sprite->SetCurFrame(Vector2(info.frameX,info.frameY)); }
	shared_ptr<Button> GetButton() { return _button; }

private:
	shared_ptr<Sprite> _sprite;
	shared_ptr<Button> _button;
};

