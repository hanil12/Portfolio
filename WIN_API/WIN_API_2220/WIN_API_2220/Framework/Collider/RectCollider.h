#pragma once
class RectCollider : public Collider
{
public:
	RectCollider(Vector2 center, Vector2 size);
	~RectCollider();

	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	Vector2& GetSize() { return size; }

	float Left() { return center.x - size.x * 0.5f; }
	float Top() { return center.y - size.y * 0.5f; }
	float Right() { return center.x + size.x * 0.5f; }
	float Bottom() { return center.y + size.y * 0.5f; }

	virtual bool IsCollision(const Vector2& pos) override;
	virtual bool IsCollision(const shared_ptr<RectCollider> rect) override;
	virtual bool IsCollision(const shared_ptr<CircleCollider> circle) override;

private:
	Vector2 size = { 0.0f, 0.0f };
};

