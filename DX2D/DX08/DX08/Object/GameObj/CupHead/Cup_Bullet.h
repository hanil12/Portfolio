#pragma once
class Cup_Bullet
{
public:
	Cup_Bullet();
	~Cup_Bullet();

	void Update();
	void Render();

	void SetPostion(Vector2 pos) { _sprite->GetTransform()->SetPosition(pos); }
	void Fire(Vector2 dir);
	void Enable();
	void Disable();

	bool Collision(shared_ptr<Collider> col);

	bool isActive;
private:
	void CreateAction();

	shared_ptr<Sprite> _sprite;
	shared_ptr<Action> _action;

	shared_ptr<Collider> _collider;

	float _speed = 400.0f;
	Vector2 _direction;

	float _delay = 0.0f;
	float _lifeTime = 3.0f;
};

