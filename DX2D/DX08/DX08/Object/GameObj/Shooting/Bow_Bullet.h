#pragma once
class Bow_Bullet
{
public:
	Bow_Bullet();
	~Bow_Bullet();

	void Update();
	void Render();

	bool isActive = false;

	shared_ptr<Collider> GetCollider() { return _col; }
	shared_ptr<Transform> GetTransform() { return _quad->GetTransform(); }

	void SetTarget(shared_ptr<Collider> col) { _target = col; }
	void SetDirection(Vector2 dir);

	void EnAble();
	void DisAble();

private:
	shared_ptr<Quad> _quad;

	shared_ptr<Collider> _col;

	float _delay = 3.0f;
	float _check = 0.0f;
	float _speed = 300.0f;
	Vector2 _dir = { 0,0 };

	weak_ptr<Collider> _target;
};

