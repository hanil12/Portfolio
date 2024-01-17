#pragma once
class Cup_Monster
{
public:
	Cup_Monster();
	~Cup_Monster();

	void Update();
	void Render();

	shared_ptr<Transform> GetTransform() { return _transform; }
	shared_ptr<Collider> GetBodyCollider() { return _collider; }

	void CreateBullet();

	void Shot(Vector2 pos);

	vector<shared_ptr<Cup_Bullet>>& GetBullets() { return _bullets; }

	void Dameged(int amount);

private:
	void CreateAction();

	shared_ptr<Transform> _transform;
	shared_ptr<RectCollider> _collider;

	shared_ptr<Sprite> _sprite;
	shared_ptr<Action> _action;

	vector<shared_ptr<Cup_Bullet>> _bullets;

	float _shotDelay = 1.0f;
	float _shotCheck = 0.0f;

	float _hp = 100.0f;
};

