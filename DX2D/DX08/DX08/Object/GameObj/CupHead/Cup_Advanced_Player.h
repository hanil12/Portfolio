#pragma once
class Cup_Advanced_Player : public Cup_Player
{
public:
	Cup_Advanced_Player();
	virtual ~Cup_Advanced_Player();

	virtual void Update() override;
	virtual void Render() override;

	void Shot();

	vector<shared_ptr<Cup_Bullet>>& GetBullets() { return _bullets; }

private:
	shared_ptr<Cup_Bullet> SeletBullet();

	shared_ptr<Transform> _muzzle;

	vector<shared_ptr<Cup_Bullet>> _bullets;

	const int _poolCount = 30;
};

