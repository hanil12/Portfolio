#pragma once
class Bow_Bow
{
public:
	Bow_Bow();
	~Bow_Bow();

	void Update();
	void Render();

	void Fire();

	shared_ptr<Transform> GetTransform() { return _quad->GetTransform(); }
	vector<shared_ptr<Bow_Bullet>>& GetBullets() { return _bullets; }
	void SetTarget(shared_ptr<Collider> collider);

private:
	shared_ptr<Quad> _quad;
	vector<shared_ptr<Bow_Bullet>> _bullets;
};

