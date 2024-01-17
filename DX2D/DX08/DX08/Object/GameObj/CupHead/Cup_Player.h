#pragma once
class Cup_Player
{
public:
	enum State
	{
		CUP_IDLE,
		CUP_RUN,
		CUP_SHOT,
		JUMP,
		NONE
	};

	Cup_Player();
	~Cup_Player();

	virtual void Update();
	virtual void Render();

	void SetRight();
	void SetLeft();

	void Input();
	void Jump();
	void Ground();
	void Falling();

	void SetAction(State state);
	void SetIDLE();

	void Damaged(float amount);
	const float& GetHp() { return _hp; }

	shared_ptr<Transform> GetTransform() { return _transform; }
	shared_ptr<CircleCollider> GetBodyCollider() { return _collider; }

protected:
	void CreateAction(string name, Action::Type type);

	State _curState = CUP_IDLE;
	State _oldState = NONE ;

	shared_ptr<Transform> _transform;
	shared_ptr<CircleCollider> _collider;

	vector<shared_ptr<Sprite>> _sprites;
	vector<shared_ptr<Action>> _actions;

	float _speed = 300.0f;
	float _jumpPower = 700.0f;

	bool _isRight = true;
	float _hp = 100.0f;
};

